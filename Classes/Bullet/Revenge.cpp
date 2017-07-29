#include "revenge.h"

bool Revenge::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	auto revenge = Sprite::createWithSpriteFrameName("revenge.png");
	addChild(revenge);
	this->setVisible(false);
	return true;
}

void Revenge::shoot()
{	
	this->setVisible(true);
	//添加淡出效果
	runAction(Sequence::create(FadeOut::create(1.0f),
		CallFuncN::create(CC_CALLBACK_0(Revenge::removeBullet,this,false)),
		NULL));
}
