#ifndef _REVENGE_H_
#define _REVENGE_H_

#include "cocos2d.h"
#include "Bullet.h"
#include "..\Classes\Monster\BaseMonster.h"

USING_NS_CC;

class Revenge : public Bullet
{
public:
    virtual bool init();
    CREATE_FUNC(Revenge);
	void shoot();
	Sprite* revenge;
};

#endif