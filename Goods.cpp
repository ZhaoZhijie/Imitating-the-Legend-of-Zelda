#include "Goods.h"

CGoods::CGoods()
{
}
bool CGoods::EnterGood(unsigned char id, unsigned short mx, unsigned short my, unsigned short w, unsigned short h)
{
	if(id>99||id==0)
		return false;
	GoodData g={mx,my,w,h};
	m_Goods[id]=g;
	return true;
}
//获取物品数据
GoodData CGoods::GetData(unsigned char id)
{
	return m_Goods[id];
}