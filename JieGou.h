#ifndef _JIE_GOU_H
#define _JIE_GOU_H
struct preGood
{//x,y��ָ��ͼ��������
	unsigned char id;//��Ʒid
	int x;
	int y;
	unsigned long code1;
	unsigned long code2;
};
//���ﵱǰ���ƶ�����Ʒ
struct pushGood
{
	unsigned char id;//��Ʒid
	float x;
	float y;
	unsigned char turn;
	unsigned long code1;
	unsigned long code2;
};
//�������Ʒ ��ʯ ���ٵ�
struct extGood
{
	unsigned char id;//������Ʒ��id
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
	int cf;//��ʯ���ֺ�����˶���֡
};
struct Good
{
	unsigned char id;//��Ʒid
	float x;
	float y;
};
struct gData
{
	unsigned char id;//��Ʒid
	float x;
	float y;
	unsigned long code1;//������Ϣ1
	unsigned long code2;//������Ϣ2
	gData * next;
};
//��ӵ����������е�ͼƬ��Ϣ
struct SinglePic
{
	unsigned short no;//ͼƬ���
	unsigned short x;
	unsigned short y;
	unsigned short order;
	SinglePic(unsigned short _no,unsigned short _x=0,unsigned short _y=0,unsigned short _order=0)
	{
		no=_no;x=_x;y=_y;order=_order;
	}
};
//����֡
struct ActData
{
	int curM;
	int curF;
	int curCF;
};
#endif