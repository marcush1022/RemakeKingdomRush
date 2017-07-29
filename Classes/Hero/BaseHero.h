#ifndef _BASE_HERO_H
#define _BASE_HERO_H

#include <cocos2d.h>

USING_NS_CC;

typedef enum{
	HeroStateNone=0,
	HeroStateRun,
	HeroStateHit,
	HeroStateDeath,
	HeroStateWait,
	HeroStateSkill1,
	HeroStateSkill2,
	HeroStateSkill3,
}HeroState;

typedef enum{
	WINDWALKER=0,
	BARBARIAN,
}HeroName;

class BaseHero : public Sprite
{
public:
	Sprite* BaseSprite;
	BaseMonster* nearestMonster;
	BaseHero();
	~BaseHero();
	CREATE_FUNC(BaseHero);
	virtual bool init() override;
	virtual void frozen();
	virtual void runToLocation(Point point);
	virtual void refrozen(float dt);
	
	//属性
	CC_SYNTHESIZE(std::vector<Point>, pointsVector, PointsVector);
	CC_SYNTHESIZE(HeroName, heroType, HeroType);
	CC_SYNTHESIZE(float, runSpeed, RunSpeed);
	CC_SYNTHESIZE(float, maxHp, MaxHp);
	CC_SYNTHESIZE(float, currHp, CurrHp);
	CC_SYNTHESIZE(float, force, Force);
	CC_SYNTHESIZE(float, armor, Armor);
	CC_SYNTHESIZE(float, hpPercentage, HpPercentage);
	CC_SYNTHESIZE(HeroState, state, State);
	CC_SYNTHESIZE_READONLY(ProgressTimer*, hpBar, HpBar);
	CC_SYNTHESIZE(Point, location, Location);
	
protected:
	Sprite* hpBgSprite;
	HeroState tempState;
	HeroState lastState;
	
	Point currPoint();
    Point nextPoint();
	Point tempNextPoint;
	//行走
    void runToLocation(Point point);
	virtual void lookingForMonsters(float dt);
	void checkNearestMonster();
	//判断方向
	void checkDirection();
	void checkDirectionForMonster();
	virtual void attack();
	void createAndSetHpBar();
	void birth(Point point);
	void stopHeroAnimation();
	float calculateTime(Point point);
	virtual void runToMonster();
	virtual void attackMonster(float dt);
};

#endif
