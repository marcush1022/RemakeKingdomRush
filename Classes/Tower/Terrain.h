#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "cocos2d.h"

using namespace std;

USING_NS_CC;


class MyTerrain : public Sprite{

public:
	virtual bool init(int type);
    static MyTerrain* createTerrain(int type);
	void showUpdateMenu();
    void hideUpdateMenu();
	Sprite* terrain;
	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchEnded(Touch* touch, Event* event);
	bool isUpdateMenuShown;
	void smokeEffect();
};

#endif