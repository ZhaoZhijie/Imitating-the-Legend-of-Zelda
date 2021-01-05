#ifndef _ROLE_H
#define _ROLE_H
#include "MakeMove.h"
#define _ROLE_HERO 0
#include <list>
#define _BULLET_GRASS 7		//��
#define _BULLET_STONE 6		//ʯͷ
class CTeXiao;
struct Bullet
{
	unsigned char _id;//�ӵ����� �� ʯͷ ը�� ������ ����
	float _x;
	float _y;
	float _z;
	float _vx;
	float _vy;
	float _vz;
	unsigned long code;//�ӵ�������Ϣ
};
class CRole
{
protected:
	int m_type;//����־
	unsigned char m_hurtMove[3];
	unsigned short m_no;//��ǰͼƬ�Ķ�Ӧ���
	unsigned char m_Life;			//����ֵ
	float m_x;//����
	float m_y;
	float m_z;
	float m_vx;
	float m_vy;
	float m_vz;
	int m_state;			//��ǰ״̬
	int m_statelast;//״̬��ʱ��
	//�������
	int m_CurM;		//��ǰ����
	int m_CurF;			//��ǰ֡
	int m_CurCF;		//����֡
	bool m_mirror;
	//Ӱ��
	int m_shade;
	//�ӵ�
	std::list<Bullet> m_Bullet;
public:
	CRole();
	virtual ~CRole();
	//���н�ɫ�߼� ������հ���  ���߹����AI
	virtual void Run();
	//�ı䶯��֡
	bool RunMoves();
	//��ȡ��ǰ֡ͼƬ���
	unsigned short GetNo();
	//��ȡ��ɫ����
	int GetType();
	//��ȡ��ɫ״̬
	int GetState();
	//���ý�ɫ״̬
	void SetState(int s);
	//��ȡ��ɫ����
	void GetXY(float * x=0, float * y=0, float *z=0);
	//���ý�ɫ����
	void SetXY(float x, float y,float z=0);
	//���ö���
	void SetMove(int m,int f);
	//��ȡ��ɫ�ٶ�
	void GetVXY(float * vx=0, float * vy=0, float * vz=0);
	//���ý�ɫ�ٶ�
	void SetVXY(float vx, float vy,float vz=0);
	//��ȡ��ɫ��ǰ������ǰ֡��ά��֡
	void GetAct(int *m=0, int *f=0, int *cf=0);
	//��ȡ��ɫ״̬��Ϣ
	int GetSite();
	//��ȡ������Ϣ
	bool GetMirror();
	//������ fx,fy������
	void Hit(float fx,float fy,int hurt=1);
	//��ȡӰ��״̬
	int GetShade();
	//��ȡ�ӵ�
	std::list<Bullet> * GetBullet();
};
#endif