#ifndef _GOODS_H
#define _GOODS_H
//��Ʒ
struct GoodData
{//ê��Ϳ��
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
#define _GOOD_GRASS 1		//��
#define _GOOD_GBOX 2		//���ӹ�
#define _GOOD_KBOX 3		//���ӿ�
#define _GOOD_GJG		4		//���ع�
#define _GOOD_KJG		5		//���ؿ�
#define _GOOD_YST		6		//Բʯͷ
#define _GOOD_GRASSBONE 7		//����Ĳ�
#define _GOOD_DFK	9		//��ʯ��
#define _GOOD_DST		10	//��Բʯͷ
#define _GOOD_XSK		11	//Сʯ��
#define _GOOD_BS_BLACK		12	//�ڱ�ʯ
#define _GOOD_BS_GREEN		13	//�̱�ʯ
#define _GOOD_BS_BLUE		14	//����ʯ
#define _GOOD_RB_RED		15	//�챦ʯ
#define _GOOD_YS		34	//Կ��
#define _GOOD_BLOCK 100 //�ϰ���
#define _GOOD_CD 101 //�ݵ�
#define _GOOD_XJ 102 //����
#define _GOOD_HY 105 //����
#define _GOOD_MD 106 //�Ŷ�

#endif