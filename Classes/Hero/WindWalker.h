#ifndef _WIND_WALKER_H_
#define _WIND_WALKER_H_

#include "baseHero.h"

class windWalker : public baseHero
{
public:
	int attackCnt;
	int critGap; //critical hit rate
	int dodgeRate;
	bool ableSummon; //flag of the abillty of summon
	Bear* bear; //summon creature
	bool init();
	CREATE_FUNC(windWalker);
	static windWalker* createWindWalker(Point point);
	void update(float dt);
	void attackMonster(float dt);
	void cureBear(float dt);
	void birth(Point point);
	void checkNearestMonster();
	bool checkDirectionForMonster();
	void runToMonster();
};

#endif
