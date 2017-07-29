#include "Barbarian.h"
#include "..\Data\GameManager.h"

Barbarian* Barbarian::createBarbarian(Point point)
{
	auto barbarian=new Barbarian();
	if(barbarian && barbarian->init())
	{
		barbarian->setLocation(point);
		barbarian->setMaxHp(1000);
		barbarian->setCurrHp(1000); 
		barbarian->setForce(50);
		barbarian->setState(HeroStateNone);
		barbarian->birth(point);
        barbarian->autorelease();
        return barbarian;
	}
	CC_SAFE_DELETE(barbarian);
	return NULL;
}

void Barbarian::birth(Point point)
{
	SoundManager::playBarbarianReady();
	setPosition(point);
	scheduleUpdate();
	schedule(schedule_selector(Barbarian::lookingForMonsters), 1.0f,-1,caculateTime(point));
}

bool Barbarian::init()
{
	if(!Barbarian::init())
		return false;
	baseSprite = Sprite::createWithSpriteFrameName("barbarian_C0_0001.png");
	addChild(baseSprite);
	createAndSetHpBar();
	//Revenge动画效果
	Revenge = Revenge::create();
	addChild(lava);
	lastState = heroStateNone;
	return true;
}

void Barbarian::attackMonster(float dt)
{
	if(nearestMonster!=NULL && nearestMonster->getCurrHp()>0){
		setState(HeroStateHit);
		if (!checkDirectionForMonster()){
			nearestMonster->setState(stateAttackLeft);
		}else{
			nearestMonster->setState(stateAttackRight);
		}
		auto monsterCurrHp = nearestMonster->getCurrHp();
		auto HeroHp = this->getCurrHp();
		
		if(monsterCurrHp <= 0){
			monsterCurrHp = 0;
		}
		
		//rage activited
		if((float)(HeroHp/this->getMaxHp()<0.3))
		{
			if(monsterCurrHp/nearestMonster->getMaxHp()>executeVal)
			{
				monsterCurrHp =  monsterCurrHp - this->getForce()-rageVal;
				HeroHp+=(this->getForce()+rageVal)*0.25;
				if(nearestMonster->getState()!=stateFrozen)
				{
					HeroHp-=nearestMonster->getForce();
					if(rand()%100<=revengeRate)
						shouldReven=true;
				}
			}
			else
				monsterCurrHp =  monsterCurrHp-99999;
		}
		else 
		{
			if(monsterCurrHp/nearestMonster->getMaxHp()>executeVal)
			{
				monsterCurrHp =  monsterCurrHp - this->getForce();
				if(nearestMonster->getState()!=stateFrozen)
				{
					HeroHp-=nearestMonster->getForce();
					if(rand()%100<=revengeRate)
						shouldReven=true;
				}
			}
			else
				monsterCurrHp =  monsterCurrHp-99999;
		}
		
		if(shouldReven==true)
		{	
			//创建revenge动画
			operation->runAction(Sequence::create(
				Animate::create(AnimationCache::getInstance()->getAnimation("Barbarian_Revenge")),
				CallFuncN::create(CC_CALLBACK_0(Revenge::shoot,revenge)),
				NULL));
				
			auto monsterVector = GameManager::getInstance()->monsterVector;
			auto heroPosition = this->getPosition();
			for (int j = 0; j < monsterVector.size(); j++)
			{
				auto monster = monsterVector.at(j);
				auto monsterPosition = monster->baseSprite->getPosition();
				if(heroPosition.distance(monsterPosition)<= 35 && monster->getAttackBySoldier() )
				{
					auto currHp = monster->getCurrHp();
					currHp =  currHp - this->getForce()*3;
						
					if(currHp <= 0){
						currHp = 0;
					}
					monster->setCurrHp( currHp );
					monster->getHpBar()->setPercentage((currHp/monster->getMaxHp())*100);
					}
				}
			}
		}
			
		if(HeroHp <= 0){
			HeroHp = 0;
		}
		if(monsterCurrHp <= 0){
			monsterCurrHp = 0;
		}
		
		nearestMonster->setCurrHp( monsterCurrHp );
		this->setCurrHp(HeroHp);

		nearestMonster->getHpBar()->setPercentage((monsterCurrHp/nearestMonster->getMaxHp())*100);
		this->getHpBar()->setPercentage((HeroHp/this->getMaxHp())*100);

		//death
		if(monsterCurrHp == 0){
			unschedule(schedule_selector(Barbarian::attackMonster));
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
			hpBgSprite->setVisible(false);
			if(nearestMonster != NULL && nearestMonster->getCurrHp()>0){
				nearestMonster->restartWalking();
				nearestMonster->setIsAttacking(false);
			}
			baseSprite->runAction(Sequence::create(
				Animate::create(AnimationCache::getInstance()->getAnimation("Barbarian_death"))
				,FadeOut::create(1.0f)
				,NULL));
		}
	}else{
		unschedule(schedule_selector(Barbarian::attackMonster));
		if(this->getCurrHp()>0)
				runToLocation(location);
	}
}

void Barbarian::update(float dt)
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
			auto action = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("Barbarian_run")));
			action->setTag(HeroStateRun);
			baseSprite->runAction(action);}
			break;
		case(HeroStateHit):{
			lastState = HeroStateHit;
			stopSoldierAnimation();
			auto action = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("Barbarian_attack")));
			action->setTag(HeroStateHit);
			baseSprite->runAction(action);}
			break;
		case(HeroStateWait):{
			lastState = HeroStateWait;
			stopHeroAnimation();
			baseSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Barbarian_C0_0001.png"));}
			break;
		}
	}
}

//与其他类一样
void Barbarian::checkNearestMonster()
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

void Barbarian::runToMonster()
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
	schedule(schedule_selector(Barbarian::attackMonster), 1.0f,-1,caculateTime(destination));
}

bool Barbarian::checkDirectionForMonster()
{
	if((nearestMonster->baseSprite->getPositionX() - this->getPositionX())>0){
		baseSprite->setFlippedX(false);
		return false;
	}else{
		baseSprite->setFlippedX(true);
		return true;
	}
}



