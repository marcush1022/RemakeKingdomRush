(Remake)KingdomRush
=====
>用cocos2dx3.0重制经典塔防游戏KingdomRush:Frontier<br>
>IDE为visual studio community 2017<br>
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
>* 塔模块 <br>
>  包括4种基础塔，箭塔(单体伤害)、法师塔(类似箭塔，升级后会有召唤物)、炮塔(范围伤害)和兵营塔(召唤士兵)，在此基础上
>  再派生出具体的塔
>* 怪物模块 <br>
>  即各种怪物和2种boss，boss只出现于特定关卡，与普通怪物不同的是boss有出场动画，通过地图模块重写方法waveEvent实现
>* 地图模块 <br>
>  地图模块一共分为3层，一层为地图层，用于设定每一关地图，并添加塔和敌人；按键层，用于监听技能与商店按键，显示玩家生
>  命与金币状态，监听暂停菜单和暂停按钮；触摸层，用于拦截触摸或点击事件(例如滚动地图)，监听技能释放，防御塔升级菜单
>* 子弹模块 <br>
>  分为4种:
>  1.基础的单体箭类(MageBolt、TotemAxe等)，落点与怪物位置重合就损血
>  2.区域攻击的炸弹类(FireBall、Bomb等)，落点范围内的怪物损血
>  3. 撼地者的地震波EarthquakeSmoke，以塔为中心的所有怪物都损血
>  4. 大法师塔的龙卷风Twister，这个比较复杂，需要添加一个寻找最近目标的方法和自身移动的方法
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
