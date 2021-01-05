#ifndef _GOODS_H
#define _GOODS_H
//物品
struct GoodData
{//锚点和宽高
	unsigned short _mx;
	unsigned short _my;
	unsigned short _w;
	unsigned short _h;
};
class CGoods
{
private:
	GoodData m_Goods[100];
	unsigned char m_len;
public:
	CGoods();
	bool EnterGood(unsigned char id, unsigned short mx, unsigned short my, unsigned short w, unsigned short h);
	GoodData GetData(unsigned char id);

};
#define _GOOD_GRASS 1		//草
#define _GOOD_GBOX 2		//箱子关
#define _GOOD_KBOX 3		//箱子开
#define _GOOD_GJG		4		//机关关
#define _GOOD_KJG		5		//机关开
#define _GOOD_YST		6		//圆石头
#define _GOOD_GRASSBONE 7		//举起的草
#define _GOOD_DFK	9		//大方石块
#define _GOOD_DST		10	//大圆石头
#define _GOOD_XSK		11	//小石块
#define _GOOD_BS_BLACK		12	//黑宝石
#define _GOOD_BS_GREEN		13	//绿宝石
#define _GOOD_BS_BLUE		14	//蓝宝石
#define _GOOD_RB_RED		15	//红宝石
#define _GOOD_YS		34	//钥匙
#define _GOOD_BLOCK 100 //障碍物
#define _GOOD_CD 101 //草地
#define _GOOD_XJ 102 //陷阱
#define _GOOD_HY 105 //火焰
#define _GOOD_MD 106 //门顶

#endif