#ifndef _TE_XIAO_H
#define _TE_XIAO_H
class CTeXiao
{
protected:
	int m_CurM;
	int m_CurF;
	int m_CurCF;
	float m_x;
	float m_y;
	float m_z;
	float m_vx;
	float m_vy;
	float m_vz;
public:
	CTeXiao(int id, float x,float y);
	bool Run();
	unsigned short GetNo();
	void GetXY(float * x, float * y);
};
#endif