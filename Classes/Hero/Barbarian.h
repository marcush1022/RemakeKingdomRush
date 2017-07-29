#ifndef _BARBARIAN_H_
#define _BARBARIAN_H_

#include "baseHero.h"

class Barbarian : public BaseHero
{
public:
	int revengeRate;
	float executeVal; //threshold of Hp to be executed
	float rageVal; //ammont of incressed force
	bool shouldReven; //flag of relase skill
	bool init();
	CREATE_FUNC(barbarian);
	static barbarian* createBarbarian(Point point);
	void update(float dt);
	void attackMonster(float dt);
	void birth(Point point);
	void checkNearestMonster();
	bool checkDirectionForMonster();
	void runToMonster();
};
