#ifndef _GOOD_MANAGER_H
#define _GOOD_MANAGER_H
#include <list>
#include "JieGou.h"

class CGoodManager
{
private:
	//һ��������һ�е�ͼ�����ϵ���Ʒ
	gData* m_goodlist[64];
public:
	CGoodManager();
	//�����Ʒ
	void Add(unsigned char id, int x, int y,unsigned long code1=0,unsigned long code2=0);
	//ɾ����Ʒ
	bool Del(int x,int y);
	//��ȡĳ�����ӵ���Ϣ
	gData* GetSingle(int x,int y);
	//��ȡ��Ʒ����
	gData** GetList();
};
#endif