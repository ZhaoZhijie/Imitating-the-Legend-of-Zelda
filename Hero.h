#ifndef _HERO_H
#define _HERO_H
#include "Role.h"
#include "PlayerMove.h"
#include "JieGou.h"
#include <math.h>
#include "TeXiao.h"
#define _STATE_AUTO 0	//自然状态
#define _STATE_ATTACK1	//攻击状态
#define _LIFT_NONE 0		//没有举东西
#define _LIFT_GRASS 7		//举起物为草
#define _LIFT_HERO 2		//举起物为人
#define _LIFT_STONE 3		//举起物为草
#define _WEAPON_DEF 0			//道具为盾牌
#define _WEAPON_BOW 1			//道具为弓箭
#define _WEAPON_WING 2			//道具为翅膀
#define _KISS_DOWN 0					//键盘处于按下状态
#define _KISS_UP 1							//键盘处于弹起状态
#define _K_UP 0						
#define _K_DOWN 1
#define _K_LEFT 2
#define _K_RIGHT 3
#define _K_A 4
#define _K_B 5
#define _K_L 6
#define _K_R 7
class CFirst;
class CHero : public CRole
{
protected:
	//state 0 普通 1受伤 2死亡
	unsigned char m_Weapon;	//道具类型
	unsigned char m_Lift;			//举起物
	bool m_GetKey;//是否获得了钥匙
	unsigned long m_LiftCode;//举起物包含的东西
	pushGood m_Push;//推动的物品
	unsigned char m_KeyState[8];		//按键状态	上 下 左 右 A B L R
	int  m_KStime[8];//当前按键状态持续帧数
	unsigned char m_turn;		//人的移动方向
	float m_walkspd;//行走速度
	CPlayerMove m_PM;//方向判断
	CFirst * m_gScene;
	CTeXiao *m_ShuiWen;
public:
	void Init(CFirst * fst,float x, float y, unsigned char m_Life);
	void Run();
	void KeyIn(unsigned char n, unsigned char ks);
	unsigned char GetLift();
	//处理被动碰撞事件，也就是走路时碰到一些东西
	void HandleCrash(gData* gd);
	//获取血量
	unsigned char GetLife();
	//获取钥匙
	void GetKey();
};
#endif
