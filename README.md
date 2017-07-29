(Remake)KingdomRush
=====
>用cocos2dx3.0设计实现经典塔防游戏KingdomRush:Frontier<br>
>IDE为visual studio community 2017<br>
>素材包(图片和声音等)来源于网络<br>
>实现原游戏大部分内容, 包括9种防御塔, 24种怪物, 2种boss, 7种士兵<br>
>关卡一共12关<br>
>另外英雄系统还在研究中...
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/img/hero.PNG" width="50%" height="50%" />
   
<br>

INTRODUCTION:
=====

I.截图：<br>
----
1.主界面 <br>
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/main.PNG" width="70%" height="70%" />

2.存档选择 <br>
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/slot.PNG" width="70%" height="70%" />

3.任务选择(大地图) <br>
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/2.PNG" width="70%" height="70%" />

4.任务详情 <br>
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/3.PNG" width="70%" height="70%" />

5.游戏界面 <br>
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/23.PNG" width="70%" height="70%" />

6.塔升级菜单 <br>
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/circle.PNG" width=312 height=312 />

II.流程图：<br>
----
![4](https://github.com/marcush1022/RemakeKingdomRush/blob/master/L1.PNG)

III.组件结构图：<br>
----
>主要分为4大模块: <br>
>* **塔模块** <br>
>  包括4种基础塔，箭塔(单体伤害)、法师塔(类似箭塔，升级后会有召唤物)、炮塔(范围伤害)和兵营塔(召唤士兵)，在此基础上
>  再派生出具体的塔
>* **怪物模块** <br>
>  即各种怪物和2种boss，boss只出现于特定关卡，与普通怪物不同的是boss有出场动画，通过地图模块重写方法waveEvent实现
>* **地图模块** <br>
>  地图模块一共分为3层，一层为地图层，用于设定每一关地图，并添加塔和敌人；按键层，用于监听技能与商店按键，显示玩家生
>  命与金币状态，监听暂停菜单和暂停按钮；触摸层，用于拦截触摸或点击事件(例如滚动地图)，监听技能释放，防御塔升级菜单
>* **子弹模块** <br>
>  分为4种:
>  1.基础的单体箭类(MageBolt、TotemAxe等)，落点与怪物位置重合就损血  
>  2.区域攻击的炸弹类(FireBall、Bomb等)，落点范围内的怪物损血  
>  3.撼地者的地震波EarthquakeSmoke，以塔为中心的所有怪物都损血  
>  4.大法师塔的龙卷风Twister，这个比较复杂，需要添加一个寻找最近目标的方法和自身移动的方法  
<br>

![5](https://github.com/marcush1022/RemakeKingdomRush/blob/master/Z1.PNG)

IV.类图: <br>
----
1. "塔" <br>
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/img/Tower3.png" />
2. "怪物" <br>
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/Monster%20(1).png" width="70%" height="70%" />
3. "子弹" <br>
<img src="https://github.com/marcush1022/RemakeKingdomRush/blob/master/Bullet%20(1).png" width="85%" height="85%" />

Update 07/29/2017
----

英雄完成了两个，然而英雄素材包还是没有，但思路已经确定了

英雄类似士兵类中的援兵类，可以由玩家控制移动位置，在游戏开始时就执行函数birth(Point p)<br>
每个英雄四个技能，3个英雄自己释放的技能和一个玩家控制释放的技能

第一个英雄为WindWalker

技能1: 每攻击n次，第n次攻击造成额外伤害<br>
技能2: 有n的概率闪避敌人攻击，并对正在攻击的敌人造成额外伤害<br>
```
//技能1和2都在windWalker类的attackMonster方法里
if(attackCnt%critGap==0)
{
	setState(HeroStateSkill1);
	monsterCurrHp =  monsterCurrHp - this->getForce() - 30; //暴击触发
	if(nearestMonster->getState()!=stateFrozen)
	//闪避触发，用srand
	if(rand()%100>dodgeRate)
			HeroHp=HeroHp-nearestMonster->getForce();
	++attackCnt;
}
```
技能3: 召唤一只熊(士兵类)，同理于兵营塔召唤兵<br>
```
//召唤熊
if(ableSummon)
	bear=Bear::createBear(Point(0, -40));
//治疗熊
if(bear->getState()==SoldierStateWait)//不在战斗状态
{
	bear->setCurrHp(bear->getMaxHp());
	bear->setHpPercentage(100);
	bear->getHpBar()->setPercentage(100);
}
```
技能4 (ArrowRain): 玩家释放的AOE技能<br>
将玩家的英雄选择存入UserDefault，在生成按钮层playStateMenu时读取对应所选英雄数字的资源文件生成sprite
并在触摸层touchLayer中为其添加技能释放触摸监听方法，然后创建子弹类ArrowRain
```
void TouchLayer::onArrowRainTouchEnded(Touch* touch, Event* event)
{
	SoundManager::playArrowRainUnleash();
	auto arrowRain = ArrowRain::create(); //创建子弹类ArrowRain
	this->getParent()->addChild(arrowRain);
	arrowRain->shoot(static_cast<TouchLayer*>(event->getCurrentTarget())->convertTouchToNodeSpace(touch));
	removeArrowRainTouchShield();
}
```

第二个英雄为Barbarian

技能1 (Revenge): 受到攻击有n的概率对周围所有敌人造成伤害<br>
技能2 (Execute): 斩杀生命值低于n的敌人<br>
技能3 (Rage): 在自己生命值低于n时触发，期间攻击造成额外伤害并回复造成伤害百分比的血量<br>
```
//技能123都在方法attackMonster中
if((float)(HeroHp/this->getMaxHp()<0.3)) //Rage触发
{
		if(monsterCurrHp/nearestMonster->getMaxHp()>executeVal)
		{
			monsterCurrHp =  monsterCurrHp - this->getForce()-rageVal;
			HeroHp+=(this->getForce()+rageVal)*0.25;
			if(nearestMonster->getState()!=stateFrozen)
			{
				HeroHp-=nearestMonster->getForce();
				if(rand()%100<=revengeRate)
					shouldReven=true; //Revenge触发
			}
		}
		else //Execute触发
			monsterCurrHp =  monsterCurrHp-99999;
}
```

```
if(shouldReven==true) //Execute触发
{	
		//创建revenge动画
		operation->runAction(Sequence::create(
		   Animate::create(AnimationCache::getInstance()->getAnimation("Barbarian_Revenge")),
			CallFuncN::create(CC_CALLBACK_0(Revenge::shoot,revenge)),NULL));
				
			auto monsterVector = GameManager::getInstance()->monsterVector;
			auto heroPosition = this->getPosition();
			for (int j = 0; j < monsterVector.size(); j++)
			{
				auto monster = monsterVector.at(j);
				auto monsterPosition = monster->baseSprite->getPosition();
            //范围内敌人损失血量，类似earthQuaker类
				if(heroPosition.distance(monsterPosition)<= 35 && monster->getAttackBySoldier() )
				{
					auto currHp = monster->getCurrHp();
					currHp =  currHp - this->getForce()*3;
						
					if(currHp <= 0){
						currHp = 0;
					}
					monster->setCurrHp( currHp );
					monster->getHpBar()->setPercentage((currHp/monster->getMaxHp())*100);
					}
				}
			}
}
```
技能4 (AncientHammer): 玩家释放的AOE技能，造成伤害并眩晕敌人，同理ArrowRain，创建一个子弹类AncientHammer
