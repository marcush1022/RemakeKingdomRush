#include "windWalker.h"
#include "..\Data\GameManager.h"

windWalker* windWalker::createWindWalker(Point point)
{
	auto windWalker=new windWalker();
	if(windWalker && windWalker->init())
	{
		windWalker->setLocation(point);
		windWalker->setMaxHp(1000);
		windWalker->setCurrHp(1000); 
		windWalker->setForce(50);
		windWalker->setState(HeroStateNone);
		windWalker->birth(point);
		windWalker->attackCnt=0;
		windWalker->critGap=3;
		windWalker->ableSummon=true;
		windWalker->dodgeRate=30;
        windWalker->autorelease();
        return windWalker;
	}
	CC_SAFE_DELETE(windWalker);
	return NULL;
}

void windWalker::birth(Point point)
{
	SoundManager::playWindWalkerReady();
	setPosition(point);
	scheduleUpdate();
	schedule(schedule_selector(windWalker::lookingForMonsters), 1.0f,-1,caculateTime(point));
}

bool windWalker::init()
{
	if(!windWalker::init())
		return false;
	baseSprite = Sprite::createWithSpriteFrameName("windWalker_C0_0001.png");
	addChild(baseSprite);
	createAndSetHpBar();
	lastState = heroStateNone;
	//summon a bear
	if(ableSummon)
		bear=Bear::createBear(Point(0, -40));
	return true;
}

void windWalker::cureBear(float dt)
{
	if(bear->getState()==SoldierStateWait)
	{
		bear->setCurrHp(bear->getMaxHp());
		bear->setHpPercentage(100);
		bear->getHpBar()->setPercentage(100);
	}
	else if(bear->getState()==SoldierStateDeath)
	{
		bear->removeFromParent();
		bear=NULL;
		bear=Bear::createBear(Point(0, -40));
		addChild(bear);
	}
}

void windWalker::attackMonster(float dt)
{
	if(nearestMonster!=NULL && nearestMonster->getCurrHp!=0)
	{
		if(checkDirectionForMonster())
			nearestMonster->setState(stateAttackLeft);
		else
			nearestMonster->setState(stateAttackRight);
	}
	auto monsterCurrHp = nearestMonster->getCurrHp();
	auto HeroHp = this->getCurrHp();

	if(attackCnt%critGap==0 && attackCnt<99)
	{
		setState(HeroStateSkill1);
		monsterCurrHp =  monsterCurrHp - this->getForce() - 30; //critical hit
		if(nearestMonster->getState()!=stateFrozen)
			//dodge
			if(rand()%100>dodgeRate)
				HeroHp=HeroHp-nearestMonster->getForce();
		++attackCnt;
	}
	else
	{
		setState(HeroStateHit);
		monsterCurrHp =  monsterCurrHp - this->getForce();
		if(nearestMonster->getState()!=stateFrozen)
			//dodge
			if(rand()%100>=dodgeRate)
				HeroHp=HeroHp-nearestMonster->getForce();
		++attackCnt;
	}

	if(monsterCurrHp <= 0)
		monsterCurrHp = 0;
	if(HeroHp <= 0)
		HeroHp = 0;
	nearestMonster->setCurrHp( monsterCurrHp );
	this->setCurrHp(HeroHp);
	
	nearestMonster->getHpBar()->setPercentage((monsterCurrHp/nearestMonster->getMaxHp())*100);
	this->getHpBar()->setPercentage((HeroHp/this->getMaxHp())*100);*/

	if(monsterCurrHp == 0){
		unschedule(schedule_selector(windWalker::attackMonster));
		//GameManager::getInstance()->monsterVector.eraseObject(nearestMonster);
		nearestMonster->death();
		if(this->getCurrHp()>0)
			runToLocation(location);
	}
	
	if(HeroHp == 0){
			lastState = HeroStateDeath;
			setState(HeroStateDeath);
			unscheduleAllCallbacks();
			stopAllActions();
			baseSprite->stopAllActions();
			//移除召唤物
			bear->death();
			
			if(nearestMonster != NULL && nearestMonster->getCurrHp()>0){
				nearestMonster->restartWalking();
				nearestMonster->setIsAttacking(false);
			}
			baseSprite->runAction(Sequence::create
				(CallFuncN::create(CC_CALLBACK_0(Assassin::setState, this,SoldierStateDeath))
				,Animate::create(AnimationCache::getInstance()->getAnimation("windWalker_dead"))
				,FadeOut::create(1.0f)
				,NULL));
		}
	}else{
		unschedule(schedule_selector(windWalker::attackMonster));
		if(this->getCurrHp()>0)
			runToLocation(location);
	}
}

void windWalker::update(float dt)
{
	//若状态更新
	if(lastState!=getState()){
		//根据状态判断
		switch (getState())
		{
		case(HeroStateRun):{
			lastState = HeroStateRun;
			//停止之前动画
			stopSoldierAnimation();
			auto action = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("windWalker_run")));
			action->setTag(HeroStateRun);
			baseSprite->runAction(action);}
			break;
		case(HeroStateHit):{
			lastState = HeroStateHit;
			stopSoldierAnimation();
			auto action = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("windWalker_attack")));
			action->setTag(HeroStateHit);
			baseSprite->runAction(action);}
			break;
		case(HeroStateWait):{
			lastState = HeroStateWait;
			stopHeroAnimation();
			baseSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("windWalker_C0_0001.png"));}
			break;
		}
	}
}

//与其他类一样
void windWalker::checkNearestMonster()
{
	auto instance = GameManager::getInstance();
    auto monsterVector = instance->monsterVector;
    nearestMonster = NULL;
	for(int i = 0; i < monsterVector.size(); i++)
	{
		auto monster = monsterVector.at(i);
		double distance = (this->getPosition()).getDistance(monster->baseSprite->getPosition());

		if (monster->getAttackBySoldier() && distance < 50 && (!monster->getIsAttacking())) {
            nearestMonster = monster;
			nearestMonster->stopWalking();
			nearestMonster->setIsAttacking(true);
			break;
		}
	}
}

void windWalker::runToMonster()
{
	Point destination;
	if (!checkDirectionForMonster()){//在右边
		destination.x = nearestMonster->baseSprite->getPositionX() + nearestMonster->baseSprite->getContentSize().width/2 - this->baseSprite->getContentSize().width/2;
		destination.y = nearestMonster->baseSprite->getPositionY() + this->baseSprite->getContentSize().height/4;
	}else{//左边
		destination.x = nearestMonster->baseSprite->getPositionX() - nearestMonster->baseSprite->getContentSize().width/2 + this->baseSprite->getContentSize().width/2;
		destination.y = nearestMonster->baseSprite->getPositionY() + this->baseSprite->getContentSize().height/4;
	}
	setState(HeroStateRun);
	runAction(Sequence::create(MoveTo::create(caculateTime(destination),destination) , NULL));
	schedule(schedule_selector(windWalker::attackMonster), 1.0f,-1,caculateTime(destination));
}

bool windWalker::checkDirectionForMonster()
{
	if((nearestMonster->baseSprite->getPositionX() - this->getPositionX())>0){
		baseSprite->setFlippedX(false);
		return false;
	}else{
		baseSprite->setFlippedX(true);
		return true;
	}
}



