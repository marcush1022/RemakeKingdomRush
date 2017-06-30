#ifndef _SIMPLE_PANLE_LAYER_H_
#define _SIMPLE_PANLE_LAYER_H_

#include "cocos2d.h"
#include "..\Classes\Sprite\Circle\Circle.h"
#include "..\Classes\Tower\BaseTower.h"
#include "..\Classes\Sprite\TowerIcon\SellIcon.h"

USING_NS_CC; 

class SimplePanleLayer: public Sprite
{
public:
    virtual bool init() override;
    CREATE_FUNC(SimplePanleLayer);
	void inAnimation();
	SellIcon* sellIcon;
	CC_SYNTHESIZE(BaseTower*, tower, Tower); 
private:  
	void addIcons();
	Sprite* planesprite; 
	Circle* thisLvlCircle;
	bool onTouchBegan(Touch *touch, Event *event);
    void onTouchEnded(Touch* touch, Event* event);
	bool isBuilt;
};

#endif