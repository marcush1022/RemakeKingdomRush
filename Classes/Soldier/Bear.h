#ifndef _BEAR_H_
#define _BEAR_H_

#include "BaseSoldier.h"

class Bear : public BaseSoldier
{
public:
	bool init();
	CREATE_FUNC(Bear);
	static Bear* createBear(Point point);
	void update(float dt);
	void attackMonster(float dt);
	void birth(Point point);
	void createAndSetHpBar();
};

#endif