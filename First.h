#ifndef _FIRST_H
#define _FIRST_H
#include "Game.h"
#include <string>
#include <list>
#include "MakeMove.h"
#include "Goods.h"
#include "PlayerMove.h"
#include <Windows.h>
#include "Role.h"
#include "Hero.h"
#include "TeXiao.h"
#include "JieGou.h"
#include "GoodManager.h"
class CFirst : public CGame
{
private:
	CHero m_p[2];
	std::list<Good> m_GoodsList;//物品列表
	std::list<extGood> m_extGood;//掉落的物品
	std::list<POINT> m_Fire;//地图上的所有火焰的坐标
	std::list<POINT> m_Door;//地图上门的坐标
	std::list<CRole*> m_Monster;//怪物
	CGoodManager m_GoodManager;//物品管理
	std::list<Bullet> m_Bullet;//敌方子弹
	std::list<CTeXiao> m_TX;
	std::list<SinglePic> m_Layer[3];//图片分三层绘制 
	int m_virX;
	int m_virY;
	int m_Money;
	static CMoves m_Moves;
	static CGoods m_Goods;
	int m_gameState;
	int m_count;//计数用
	ActData m_ActBack[2]; //0表示火焰 1表示河流
public:
	//构造函数
	CFirst(const char * name1, const char * name2);
	void Input(int id, int type, int p1, int p2);
	bool Output(int id, char ** data, int *len,int * myvirX,int *myvirY);
	void Run();

	std::list<Good> * GetGoodlist();
	std::list<CTeXiao>* GetTX();
	std::list<extGood>* GetExtGood();
	std::list<Bullet> * GetBullet();
	std::list<SinglePic>* GetLayer(int n);
	std::list<CRole*>* GetMonster();
	int GetPicSize();
	void LayerClear();
	CGoodManager * GetGoodManager();
	CHero * GetHero();
	void GetVirXY(int *x, int *y);
	void GetXY(float *x, float *y);
	//初始化所有角色要用到的动作
	static void InitData();
	static CMoves* GetMoves();
	static CGoods* GetGoods();
	bool SubMoney(int n);
	void Complete(bool wintag);
};
#endif