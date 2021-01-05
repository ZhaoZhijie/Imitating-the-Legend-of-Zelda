#include "TeXiao.h"
#include "First.h"
CTeXiao::CTeXiao(int id, float x,float y)
{
	m_CurM=id;
	m_CurF=0;
	m_CurCF=0;
	m_x=x;
	m_y=y;
}
unsigned short CTeXiao::GetNo()
{
	return MAKEWORD(m_CurF,m_CurM);
}
bool CTeXiao::Run()
{
	m_CurCF++;
	if(m_CurCF>=CFirst::GetMoves()->GetContinueFrame(m_CurM,m_CurF))
	{
		m_CurCF=0;
		m_CurF++;
	}
	if(m_CurF==CFirst::GetMoves()->GetFrameLen(m_CurM)-1&&m_CurCF==CFirst::GetMoves()->GetContinueFrame(m_CurM,m_CurF)-1)
	{
		m_CurF=0;
		return false;
	}
	else 
		return true;
}
void CTeXiao::GetXY(float * x, float * y)
{
	*x=m_x;
	*y=m_y;
}