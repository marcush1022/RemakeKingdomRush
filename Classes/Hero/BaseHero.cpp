#include "BaseHero.h"
#include "..\Data\GameManager.h"

bool BaseHero::init()
{
	if(!Sprite::init())
		return false;
	nearestMonster=NULL;
	return true;
}

//progressTimer
void BaseHero::createAndSetHpBar()
{
    hpBgSprite = Sprite::createWithSpriteFrameName("lifebar_bg_small.png");

    hpBgSprite->setPosition(Point(baseSprite->getContentSize().width / 2, baseSprite->getContentSize().height ));
    baseSprite->addChild(hpBgSprite);
    
	hpBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("lifebar_small.png"));
	hpBar->setType(ProgressTimer::Type::BAR);
	hpBar->setMidpoint(Point(0, 0.5f));
	hpBar->setBarChangeRate(Point(1, 0));
	hpBar->setPercentage(100);
    hpBar->setPosition(Point(hpBgSprite->getContentSize().width / 2, hpBgSprite->getContentSize().height / 2 ));
    hpBgSprite->addChild(hpBar);
}

void BaseHero::runToMonster()
{
	Point destination;
	if (!checkDirectionForMonster()){//右边
		destination.x = nearestMonster->baseSprite->getPositionX() + nearestMonster->baseSprite->getContentSize().width/2 - this->getParent()->getParent()->getPositionX()- this->baseSprite->getContentSize().width/2;
		destination.y = nearestMonster->baseSprite->getPositionY() - this->getParent()->getParent()->getPositionY() - this->baseSprite->getContentSize().height/4;
	}else{//左边
		destination.x = nearestMonster->baseSprite->getPositionX() - nearestMonster->baseSprite->getContentSize().width/2 - this->getParent()->getParent()->getPositionX()+ this->baseSprite->getContentSize().width/2;
		destination.y = nearestMonster->baseSprite->getPositionY() - this->getParent()->getParent()->getPositionY() - this->baseSprite->getContentSize().height/4;
	}
	setState(HeroStateRun);
	runAction(Sequence::create(MoveTo::create(caculateTime(destination),destination) , NULL));
	schedule(schedule_selector(BaseHero::attackMonster), 1.0f,-1,caculateTime(destination));
}

void BaseHero::runToLocation(Point point)
{
	if(getState()!=stateDeath){
		unscheduleAllCallbacks();
		scheduleUpdate();
		stopAllActions();
		if((point.x - this->getPositionX())>0){
			baseSprite->setFlippedX(false);//翻转，面向右边
		}else{
			baseSprite->setFlippedX(true);
		}
		setState(HeroStateRun);
		runAction(Sequence::create(MoveTo::create(caculateTime(point),point),
			CallFuncN::create(CC_CALLBACK_0(BaseHero::setState, this,HeroStateWait)),
			NULL));
		schedule(schedule_selector(BaseHero::lookingForMonsters), 1.0f,-1,caculateTime(point));
	}
}

void BaseHero::lookingForMonsters(float dt)
{
	setState(HeroStateWait);
	baseSprite->setFlippedX(!(baseSprite->isFlippedX()));
	checkNearestMonster();
	if(nearestMonster!=NULL && nearestMonster->getCurrHp()>0)
	{
		attack();
	}
}

void BaseHero::checkNearestMonster()
{
	auto instance = GameManager::getInstance();
    auto monsterVector = instance->monsterVector;
    nearestMonster = NULL;
	for(int i = 0; i < monsterVector.size(); i++)
	{
		auto monster = monsterVector.at(i);
		double distance = (this->getParent()->getParent()->getPosition()+this->getPosition()).getDistance(monster->baseSprite->getPosition());

		if (monster->getAttackBySoldier() && distance < 50 && (!monster->getIsAttacking())) {
            nearestMonster = monster;
			nearestMonster->stopWalking();
			nearestMonster->setIsAttacking(true);
			break;
		}
	}
}

bool BaseHero::checkDirectionForMonster()
{
	//通过坐标判断右边
	if((nearestMonster->baseSprite->getPositionX() - (this->getParent()->getParent()->getPositionX() + this->getPositionX()))>0){
		baseSprite->setFlippedX(false);
		return false;
	}else{//左边
		baseSprite->setFlippedX(true);
		return true;
	}
}

float BaseHero::caculateTime(Point point)
{
	return point.getDistance(this->getPosition())/100;
}

void BaseHero::stopHeroAnimation()
{
	for(int i = 1 ;i <= 8;i++)
	{
		baseSprite->stopActionByTag(i);
	}
}

void BaseHero::attack()
{
	unschedule(schedule_selector(BaseHero::lookingForMonsters));
	runToMonster();
}
