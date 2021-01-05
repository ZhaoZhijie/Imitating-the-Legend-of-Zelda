#ifndef _HERO_H
#define _HERO_H
#include "Role.h"
#include "PlayerMove.h"
#include "JieGou.h"
#include <math.h>
#include "TeXiao.h"
#define _STATE_AUTO 0	//��Ȼ״̬
#define _STATE_ATTACK1	//����״̬
#define _LIFT_NONE 0		//û�оٶ���
#define _LIFT_GRASS 7		//������Ϊ��
#define _LIFT_HERO 2		//������Ϊ��
#define _LIFT_STONE 3		//������Ϊ��
#define _WEAPON_DEF 0			//����Ϊ����
#define _WEAPON_BOW 1			//����Ϊ����
#define _WEAPON_WING 2			//����Ϊ���
#define _KISS_DOWN 0					//���̴��ڰ���״̬
#define _KISS_UP 1							//���̴��ڵ���״̬
#define _K_UP 0						
#define _K_DOWN 1
#define _K_LEFT 2
#define _K_RIGHT 3
#define _K_A 4
#define _K_B 5
#define _K_L 6
#define _K_R 7
class CFirst;
class CHero : public CRole
{
protected:
	//state 0 ��ͨ 1���� 2����
	unsigned char m_Weapon;	//��������
	unsigned char m_Lift;			//������
	bool m_GetKey;//�Ƿ�����Կ��
	unsigned long m_LiftCode;//����������Ķ���
	pushGood m_Push;//�ƶ�����Ʒ
	unsigned char m_KeyState[8];		//����״̬	�� �� �� �� A B L R
	int  m_KStime[8];//��ǰ����״̬����֡��
	unsigned char m_turn;		//�˵��ƶ�����
	float m_walkspd;//�����ٶ�
	CPlayerMove m_PM;//�����ж�
	CFirst * m_gScene;
	CTeXiao *m_ShuiWen;
public:
	void Init(CFirst * fst,float x, float y, unsigned char m_Life);
	void Run();
	void KeyIn(unsigned char n, unsigned char ks);
	unsigned char GetLift();
	//��������ײ�¼���Ҳ������·ʱ����һЩ����
	void HandleCrash(gData* gd);
	//��ȡѪ��
	unsigned char GetLife();
	//��ȡԿ��
	void GetKey();
};
#endif
