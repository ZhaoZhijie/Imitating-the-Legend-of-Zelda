#include "GoodManager.h"

CGoodManager::CGoodManager()
{
	//初始化物品链表 每个链表加一个头结点
	for(int i=0;i<64;i++)
	{
		gData * gd=new gData;
		gd->next=NULL;
		m_goodlist[i]=gd; 	
	}
}
//添加物品
void CGoodManager::Add(unsigned char id, int x, int y,unsigned long code1,unsigned long code2)
{
	gData * data=new gData;
	data->id=id;
	data->x=x*16;
	data->y=y*16;
	data->code1=code1;
	data->code2=code2;
	data->next=NULL;
	gData * pre=m_goodlist[y];
	gData * cur=m_goodlist[y]->next;
	//寻找插入点
	while(cur)
	{
		if(cur->x>x*16)
			break;
		pre=cur;
		cur=cur->next;
	}
	pre->next=data;
	data->next=cur;
}
//删除物品
bool CGoodManager::Del(int x,int y)
{
	gData * pre=m_goodlist[y];
	gData * cur=m_goodlist[y]->next;
	//寻找要删除的结点
	while(cur)
	{
		if(cur->x==x*16)
		{
			pre->next=cur->next;
			delete cur;
			return true;
		}
		else if(cur->x>x*16)
		{
			return false;
		}
		pre=cur;
		cur=cur->next;
	}
}
//获取某个格子的信息
gData* CGoodManager::GetSingle(int x,int y)
{
	if(y>63||y<0||x<0||x>63)
		return 0;
	gData * pre=m_goodlist[y];
	gData * cur=m_goodlist[y]->next;
	//寻找目标结点
	bool tag=false;
	while(cur)
	{
		if(cur->x==x*16)
		{
			return cur;
		}
		else if(cur->x>x*16)
			return 0;
		pre=cur;
		cur=cur->next;
	}
	return 0;
}

//获取物品链表
gData** CGoodManager::GetList()
{
	return m_goodlist;
}