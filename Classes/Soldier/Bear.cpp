#include "Bear.h"
#include "..\Data\GameManager"

Bear* Bear::createBear(Point point)
{
	auto bear=new Bear();
	if (bear && bear->init())
    {
        bear->setLocation(point);
		bear->setMaxHp(250);
		bear->setCurrHp(250); 
		bear->setForce(30);
		bear->setState(SoldierStateNone);
		bear->birth(point);
        bear->autorelease();
        return bear;
    }
	CC_SAFE_DELETE(bear);
    return NULL;
}

void Bear::birth(Point point)
{
	SoundManager::playBearReady();
	setPosition(point);

	baseSprite->runAction(Sequence::create(
		Animate::create(AnimationCache::getInstance()->getAnimation("Bear_birth"))
		,CallFuncN::create(CC_CALLBACK_0(Bear::setState, this,SoldierStateWait))
                , NULL));
	schedule(schedule_selector(Bear::lookingForMonsters), 1.0f,-1,caculateTime(point));
}

bool Bear::init()
{
	if(!Bear::init()){
		return false;
	}
	baseSprite = Sprite::createWithSpriteFrameName("Bear_0001.png");
	addChild(baseSprite);
	createAndSetHpBar();
	lastState = SoldierStateNone;

	return true;
}

void Bear::attackMonster(float dt)
{
	if(nearestMonster!=NULL && nearestMonster->getCurrHp()>0){
		setState(SoldierStateHit);
		if (!checkDirectionForMonster()){
			nearestMonster->setState(stateAttackLeft);
		}else{
			nearestMonster->setState(stateAttackRight);
		}
		auto monsterCurrHp = nearestMonster->getCurrHp();
		auto SoldierHp = this->getCurrHp();

		monsterCurrHp =  monsterCurrHp - this->getForce();

		if(nearestMonster->getState()!=stateFrozen)
			SoldierHp =  SoldierHp - nearestMonster->getForce();

		if(monsterCurrHp <= 0){
			monsterCurrHp = 0;
		}
		if(SoldierHp <= 0){
			SoldierHp = 0;
		}
		nearestMonster->setCurrHp( monsterCurrHp );
		this->setCurrHp(SoldierHp);

		nearestMonster->getHpBar()->setPercentage((monsterCurrHp/nearestMonster->getMaxHp())*100);
		this->getHpBar()->setPercentage((SoldierHp/this->getMaxHp())*100);

		if(monsterCurrHp == 0){
			unschedule(schedule_selector(DeathKnight::attackMonster));
			//GameManager::getInstance()->monsterVector.eraseObject(nearestMonster);
			nearestMonster->death();
			if(this->getCurrHp()>0)
				runToLocation(location);
		}
		if(SoldierHp == 0){
			lastState = SoldierStateDeath;
			setState(SoldierStateDeath);
			unscheduleAllCallbacks();
			stopAllActions();
			baseSprite->stopAllActions();
			hpBgSprite->setVisible(false);
			if(nearestMonster != NULL && nearestMonster->getCurrHp()>0){
				nearestMonster->restartWalking();
				nearestMonster->setIsAttacking(false);
			}
			baseSprite->runAction(Sequence::create(
				Animate::create(AnimationCache::getInstance()->getAnimation("Bear_death"))
				,FadeOut::create(1.0f)
				,NULL));
		}
	}else{
		unschedule(schedule_selector(DeathKnight::attackMonster));
		if(this->getCurrHp()>0)
				runToLocation(location);
	}
}

void Bear::update(float dt)
{
	//若状态更新
	if(lastState!=getState()){
		//根据状态判断
		switch (getState())
		{
		case(SoldierStateRun):{
			lastState = SoldierStateRun;
			//停止之前动画
			stopSoldierAnimation();
			auto action = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("Bear_run")));
			action->setTag(SoldierStateRun);
			baseSprite->runAction(action);}
			break;
		case(SoldierStateHit):{
			lastState = SoldierStateHit;
			stopSoldierAnimation();
			auto action = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("Bear_attack")));
			action->setTag(SoldierStateHit);
			baseSprite->runAction(action);}
			break;
		case(SoldierStateWait):{
			lastState = SoldierStateWait;
			stopSoldierAnimation();
			baseSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Bear_0001.png"));}
			break;
		}
	}
}

void Bear::createAndSetHpBar()
{
    hpBgSprite = Sprite::createWithSpriteFrameName("lifebar_bg_small.png");

    hpBgSprite->setPosition(Point(baseSprite->getContentSize().width / 2, baseSprite->getContentSize().height));
    baseSprite->addChild(hpBgSprite);
    
	hpBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("lifebar_small.png"));
	hpBar->setType(ProgressTimer::Type::BAR);
	hpBar->setMidpoint(Point(0, 0.5f));
	hpBar->setBarChangeRate(Point(1, 0));
	hpBar->setPercentage(100);
    hpBar->setPosition(Point(hpBgSprite->getContentSize().width / 2, hpBgSprite->getContentSize().height / 2 ));
    hpBgSprite->addChild(hpBar);
}