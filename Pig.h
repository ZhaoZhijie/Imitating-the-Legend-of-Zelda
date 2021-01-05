#ifndef _PIG_H
#define _PIG_H
#include "Role.h"
class CFirst;
class CPig : public CRole
{
protected:
	CFirst * m_gScene;
	float m_walkspd;//猪移动速度
	float m_zdspd;//子弹移动速度
public:
	CPig(CFirst * fst,float x,float y);
	void Run();
};
#endif