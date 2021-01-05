#include "Role.h"
#include <Windows.h>
#include "First.h"
CRole::CRole()
{
	m_shade=0;
	m_Bullet.clear();
	m_state=0;
	m_statelast=0;
}
CRole::~CRole()
{}
//运行角色逻辑 比如接收按键  或者怪物的AI
void CRole::Run()
{}
//改变动作帧
bool CRole::RunMoves()
{
		m_CurCF++;
		if(m_CurCF>=CFirst::GetMoves()->GetContinueFrame(m_CurM,m_CurF))
		{
			m_CurCF=0;
			m_CurF++;
			if(m_CurF>=CFirst::GetMoves()->GetFrameLen(m_CurM))
			{
				m_CurF=0;
				int move=CFirst::GetMoves()->GetNextMove(m_CurM);
				if(move<0)
					m_CurF=-move;
				else
					m_CurM=move;
			}
		}
		if(m_shade++>3)
			m_shade=0;
		return true;
}
//获取当前帧图片编号
unsigned short CRole::GetNo()
{
	return MAKEWORD(m_CurF,m_CurM*(m_mirror?-1:1));
}
//获取角色类型
int CRole::GetType()
{
	return m_type;
}
//获取角色坐标
void CRole::GetXY(float * x, float * y, float * z)
{
	if(x!=0)
		*x=m_x;
	if(y!=0)
		*y=m_y;
	if(z!=0)
		*z=m_z;
}
//设置角色坐标
void CRole::SetXY(float x, float y,float z)
{
	m_x=x;
	m_y=y;
	m_z=z;
}
//获取角色速度
void CRole::GetVXY(float * vx, float * vy,float * vz)
{
	if(vx!=0)
		*vx=m_vx;
	if(vy!=0)
		*vy=m_vy;
	if(vz!=0)
		*vz=m_vz;
}
//设置角色速度
void CRole::SetVXY(float vx, float vy,float vz)
{
	m_vx=vx;
	m_vy=vy;
	m_vz=vz;
}
//获取角色状态
int CRole::GetState()
{
	return m_state;
}
//设置角色状态
void CRole::SetState(int s)
{
	m_state=s;
}
//设置动作
void CRole::SetMove(int m,int f)
{
	m_CurM=m;
	m_CurF=f;
}
//被攻击
void CRole::Hit(float fx,float fy,int hurt)
{
	float len=sqrt(fx*fx+fy*fy);
	m_vx=1.8*fx/len;
	m_vy=1.8*fy/len;
	int site=CFirst::GetMoves()->GetSite(m_CurM, m_CurF); 
	int preface=site/100;//还没考虑镜像时的面向
	if(hurt!=0)
	{
		 if(preface==2)
		 {
			m_CurM=m_hurtMove[0];
		 }
		 else if(preface==8)
		 {
			m_CurM=m_hurtMove[1];	 
		 }
		 else if(preface==4)
		 {
			m_CurM=m_hurtMove[2];		 
		 }
		 m_CurF=0;
		 m_CurCF=0;
	}
	 m_state=1;
	 m_statelast=0;
	 if(m_Life>=hurt)
		m_Life-=hurt;
	 else 
		 m_Life=0;
}
//获取角色当前动作当前帧和维持帧
void CRole::GetAct(int *m, int *f, int *cf)
{
	if(m!=NULL)
		*m=m_CurM;
	if(f!=NULL)
		*f=m_CurF;
	if(cf!=NULL)
		*cf=m_CurCF;
}
//获取角色状态信息
int CRole::GetSite()
{
	return CFirst::GetMoves()->GetSite(m_CurM, m_CurF);
}
//获取镜像信息
bool CRole::GetMirror()
{
	return m_mirror;
}
//获取影子状态
int CRole::GetShade()
{
	return m_shade;
}
//获取子弹
std::list<Bullet> * CRole::GetBullet()
{
	return &m_Bullet;
}