#ifndef _GOOD_MANAGER_H
#define _GOOD_MANAGER_H
#include <list>
#include "JieGou.h"

class CGoodManager
{
private:
	//一个链表保存一行地图格子上的物品
	gData* m_goodlist[64];
public:
	CGoodManager();
	//添加物品
	void Add(unsigned char id, int x, int y,unsigned long code1=0,unsigned long code2=0);
	//删除物品
	bool Del(int x,int y);
	//获取某个格子的信息
	gData* GetSingle(int x,int y);
	//获取物品链表
	gData** GetList();
};
#endif