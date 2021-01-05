#ifndef _ROLE_H
#define _ROLE_H
#include "MakeMove.h"
#define _ROLE_HERO 0
#include <list>
#define _BULLET_GRASS 7		//草
#define _BULLET_STONE 6		//石头
class CTeXiao;
struct Bullet
{
	unsigned char _id;//子弹类型 草 石头 炸弹 回旋镖 弓箭
	float _x;
	float _y;
	float _z;
	float _vx;
	float _vy;
	float _vz;
	unsigned long code;//子弹附加信息
};
class CRole
{
protected:
	int m_type;//类别标志
	unsigned char m_hurtMove[3];
	unsigned short m_no;//当前图片的对应编号
	unsigned char m_Life;			//生命值
	float m_x;//坐标
	float m_y;
	float m_z;
	float m_vx;
	float m_vy;
	float m_vz;
	int m_state;			//当前状态
	int m_statelast;//状态计时器
	//动作相关
	int m_CurM;		//当前动作
	int m_CurF;			//当前帧
	int m_CurCF;		//持续帧
	bool m_mirror;
	//影子
	int m_shade;
	//子弹
	std::list<Bullet> m_Bullet;
public:
	CRole();
	virtual ~CRole();
	//运行角色逻辑 比如接收按键  或者怪物的AI
	virtual void Run();
	//改变动作帧
	bool RunMoves();
	//获取当前帧图片编号
	unsigned short GetNo();
	//获取角色类型
	int GetType();
	//获取角色状态
	int GetState();
	//设置角色状态
	void SetState(int s);
	//获取角色坐标
	void GetXY(float * x=0, float * y=0, float *z=0);
	//设置角色坐标
	void SetXY(float x, float y,float z=0);
	//设置动作
	void SetMove(int m,int f);
	//获取角色速度
	void GetVXY(float * vx=0, float * vy=0, float * vz=0);
	//设置角色速度
	void SetVXY(float vx, float vy,float vz=0);
	//获取角色当前动作当前帧和维持帧
	void GetAct(int *m=0, int *f=0, int *cf=0);
	//获取角色状态信息
	int GetSite();
	//获取镜像信息
	bool GetMirror();
	//被攻击 fx,fy代表方向
	void Hit(float fx,float fy,int hurt=1);
	//获取影子状态
	int GetShade();
	//获取子弹
	std::list<Bullet> * GetBullet();
};
#endif