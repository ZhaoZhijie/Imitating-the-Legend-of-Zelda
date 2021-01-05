#ifndef _JIE_GOU_H
#define _JIE_GOU_H
struct preGood
{//x,y是指地图格子坐标
	unsigned char id;//物品id
	int x;
	int y;
	unsigned long code1;
	unsigned long code2;
};
//人物当前所推动的物品
struct pushGood
{
	unsigned char id;//物品id
	float x;
	float y;
	unsigned char turn;
	unsigned long code1;
	unsigned long code2;
};
//掉落的物品 宝石 加速等
struct extGood
{
	unsigned char id;//掉落物品的id
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
	int cf;//宝石出现后持续了多少帧
};
struct Good
{
	unsigned char id;//物品id
	float x;
	float y;
};
struct gData
{
	unsigned char id;//物品id
	float x;
	float y;
	unsigned long code1;//附加信息1
	unsigned long code2;//附加信息2
	gData * next;
};
//添加到绘制链表中的图片信息
struct SinglePic
{
	unsigned short no;//图片编号
	unsigned short x;
	unsigned short y;
	unsigned short order;
	SinglePic(unsigned short _no,unsigned short _x=0,unsigned short _y=0,unsigned short _order=0)
	{
		no=_no;x=_x;y=_y;order=_order;
	}
};
//动作帧
struct ActData
{
	int curM;
	int curF;
	int curCF;
};
#endif