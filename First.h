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
	std::list<Good> m_GoodsList;//��Ʒ�б�
	std::list<extGood> m_extGood;//�������Ʒ
	std::list<POINT> m_Fire;//��ͼ�ϵ����л��������
	std::list<POINT> m_Door;//��ͼ���ŵ�����
	std::list<CRole*> m_Monster;//����
	CGoodManager m_GoodManager;//��Ʒ����
	std::list<Bullet> m_Bullet;//�з��ӵ�
	std::list<CTeXiao> m_TX;
	std::list<SinglePic> m_Layer[3];//ͼƬ��������� 
	int m_virX;
	int m_virY;
	int m_Money;
	static CMoves m_Moves;
	static CGoods m_Goods;
	int m_gameState;
	int m_count;//������
	ActData m_ActBack[2]; //0��ʾ���� 1��ʾ����
public:
	//���캯��
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
	//��ʼ�����н�ɫҪ�õ��Ķ���
	static void InitData();
	static CMoves* GetMoves();
	static CGoods* GetGoods();
	bool SubMoney(int n);
	void Complete(bool wintag);
};
#endif