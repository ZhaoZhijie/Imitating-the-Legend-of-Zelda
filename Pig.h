#ifndef _PIG_H
#define _PIG_H
#include "Role.h"
class CFirst;
class CPig : public CRole
{
protected:
	CFirst * m_gScene;
	float m_walkspd;//���ƶ��ٶ�
	float m_zdspd;//�ӵ��ƶ��ٶ�
public:
	CPig(CFirst * fst,float x,float y);
	void Run();
};
#endif