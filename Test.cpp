#include "Test.h"
//¹¹Ôìº¯Êý
CTest::CTest(const char * name1, const char * name2)
{
	m_p[0]._name=name1;
	m_p[1]._name=name2;
	m_p[0]._x=100;
	m_p[0]._y=100;
	m_p[1]._x=200;
	m_p[1]._y=200;
}
void CTest::Input(int id, int type, int p1, int p2)
{
	if(p2=_KS_DH)
	{
		switch(p1)
		{
		case _GI_K_W:
			{
				m_p[id]._y--;
				break;
			}
		case _GI_K_S:
			{
				m_p[id]._y++;
				break;
			}
		case _GI_K_A:
			{
				m_p[id]._x--;
				break;
			}
		case _GI_K_D:
			{
				m_p[id]._x++;
				break;
			}
		}
	}
}
bool CTest::Output(int id, char ** data, int *len)
{
	*data=new char[34];
	*len=34;
	char unit[12];
	for(int i=0;i<2;i++)
	{
		*(int*)&unit[0]=i;
		*(int*)&unit[4]=m_p[i]._x;
		*(int*)&unit[8]=m_p[i]._y;
		memcpy(*data+i*12+10,unit,12);
	}
	return true;
}
void CTest::Run()
{}