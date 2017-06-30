#ifndef _ARROW_H_
#define _ARROW_H_

#include "cocos2d.h"
#include "..\Classes\Bullet\Bullet.h"

USING_NS_CC;

class Arrow : public Bullet
{
public:
    virtual bool init();
    CREATE_FUNC(Arrow);
	void shoot();
	void removeBullet();
};

#endif