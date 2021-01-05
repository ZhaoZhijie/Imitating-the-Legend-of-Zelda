#include "Pig.h"
#include "First.h"
#include "Tools.h"
CPig::CPig(CFirst * fst,float x,float y)
{
	m_x=x;
	m_y=y;
	m_CurM=47;
	m_CurF=0;
	m_CurCF=0;
	m_mirror=false;
	m_gScene=fst;
	m_walkspd=1.3;
	m_zdspd=1;
	m_Life=100;
	m_hurtMove[0]=47;
	m_hurtMove[1]=48;
	m_hurtMove[2]=49;
	m_vx=m_vy=m_vz=0;
}
void CPig::Run()
{
	RunMoves();
	m_statelast++;//状态计数增加
	int site=CFirst::GetMoves()->GetSite(m_CurM, m_CurF); 
	int preface=site/100;//还没考虑镜像时的面向
	int act=site%100;//动作	
	if(m_state==1)
	{
		float len=sqrt(m_vx*m_vx+m_vy*m_vy);
		if(len>0)
		{
			m_vx-=0.1*m_vx/len;
			m_vy-=0.1*m_vy/len;
		}
		if(m_statelast>30)
		{
			//if(preface==2)
			//	m_CurM=1;
			//else if(preface==8)
			//	m_CurM=3;
			//else if(preface==4)
			//	m_CurM=5;
			//m_CurF=0;
			//m_CurCF=0;
			m_state=0;
			m_statelast=0;
		}
	}
	if(m_state!=1&&act!=2)
	{
		if(rand()%300==1)
		{
			int a=rand()%100;
			if(a>=0&&a<15&&m_CurM!=44)
			{//向下
				m_CurM=44;
				m_CurF=0;
				m_CurCF=0;
				m_vx=0;
				m_vy=m_walkspd;
			}
			else if(a>=15&&a<30&&m_CurM!=45)
			{//向上
				m_CurM=45;
				m_CurF=0;
				m_CurCF=0;
				m_vx=0;
				m_vy=-m_walkspd;
			}
			else if(a>=30&&a<45&&(m_CurM!=46||(m_CurM==46&&m_mirror)))
			{//向左
				m_CurM=46;
				m_CurF=0;
				m_CurCF=0;
				m_mirror=false;
				m_vx=-m_walkspd;
				m_vy=0;
			}
			else if(a>=45&&a<60&&(m_CurM!=46||(m_CurM==46&&!m_mirror)))
			{//向右
				m_CurM=46;
				m_CurF=0;
				m_CurCF=0;
				m_mirror=true;	
				m_vx=m_walkspd;
				m_vy=0;
			}
			else if(a>=60)
			{//不动
				if(preface==2)
					m_CurM=47;
				else if(preface==8)
					m_CurM=48;
				else if(preface==4)
					m_CurM=49;
				m_CurF=0;
				m_CurCF=0;
				m_vx=0;
				m_vy=0;
			}
		}
	}
	m_x+=m_vx;
	m_y+=m_vy;
	CGoodManager *gm=m_gScene->GetGoodManager();

	//判断怪物与周围障碍物的碰撞
	int gx1=(m_x-8)/16;
	int gy1=(m_y-8)/16;
	int gx2=(m_x+8)/16;
	int gy2=(m_y+8)/16;
	int zb[4][2]={{gx1,gy1},{gx2,gy1},{gx1,gy2},{gx2,gy2}};
	//根据速度方向 调整碰撞顺序 以防止在连续障碍处卡住
	int order[4]={0,1,2,3};
	if(m_vx<0)
	{
		int tmp;
		tmp=order[0];
		order[0]=order[1];
		order[1]=tmp;
		tmp=order[2];
		order[2]=order[3];
		order[3]=tmp;
	}
	if(m_vy<0)
	{
		int tmp;
		tmp=order[0];
		order[0]=order[2];
		order[2]=tmp;
		tmp=order[1];
		order[1]=order[3];
		order[3]=tmp;		
	}

	for(int k=0;k<4;k++)
	{
		bool tag=false;
		for(int n=0;n<k;n++)
		{
			if(zb[order[n]][0]==zb[order[k]][0]&&zb[order[n]][1]==zb[order[k]][1])
			{
				tag=true;
				break;
			}
		}
		if(tag)
			continue;
		gData* gd=gm->GetSingle(zb[order[k]][0],zb[order[k]][1]);
		if(gd)
		{
			fRect r;
			fRect rRole={m_x-8,m_y-8,m_x+8,m_y+8};
			fRect rBlock={zb[order[k]][0]*16, zb[order[k]][1]*16,rBlock._left+16,rBlock._top+16};
			if(CTools::RectCrash(&r,&rRole,&rBlock))
			{
				if(gd->id!=101)
				{
					CTools::CrashAdjust(rRole, rBlock, &m_x, &m_y, m_vx, m_vy);
				}
			} 
		}
	}


	if(act==2&&m_CurF==1&&m_CurCF==0)
	{//嘴伸出时吐子弹
		Bullet b={255,0,0,-6,0,0,0,MAKEWORD(0,0)};
		if(preface==2)
		{
			b._vy=m_zdspd;
			b._x=m_x;
			b._y=m_y+8;
		}
		else if(preface==8)
		{
			b._vy=-m_zdspd;
			b._x=m_x;
			b._y=m_y-8;
		}
		else if(preface==4)
		{
			b._vx=m_mirror?m_zdspd:-m_zdspd;
			b._x=m_mirror?m_x+4:m_x-4;
			b._y=m_y+4;
		}
		m_Bullet.push_back(b);
	}
	if(act==1&&rand()%50==1)
	{			
			if(preface==2)
			{
				m_CurM=50;
			}
			else if(preface==8)
			{
				m_CurM=51;
			}
			else if(preface==4)
			{
				m_CurM=52;
			}
			m_CurF=0;
			m_CurCF=0;
			m_vx=0;
			m_vy=0;
	}
	SinglePic pigpic(GetNo(),m_x,m_y,m_y);
	m_gScene->GetLayer(1)->push_back(pigpic);
	//遍历移动子弹
	for(std::list<Bullet>::iterator k=m_Bullet.begin();k!=m_Bullet.end();)
	{
		//放入子弹和影子的图片
		SinglePic yz(28,k->_x,k->_y);
		m_gScene->GetLayer(1)->push_back(yz);
		SinglePic p(MAKEWORD(HIBYTE(k->code),53) ,k->_x,k->_y+k->_z*0.7,k->_y);
		m_gScene->GetLayer(1)->push_back(p);
		//子弹移位
		k->_x+=k->_vx;
		k->_y+=k->_vy;
		k->_z+=k->_vz;
		if(k->_vz>0.00001||k->_vz<-0.00001)//子弹已经碰到障碍物被弹起
		{
			k->_vz+=0.041;
		}
		if(k->_z>0)//已经落至地面
		{
			k=m_Bullet.erase(k);
			continue;
		}
		if(k->_x<0||k->_y<0)
		{
			k=m_Bullet.erase(k);
			continue;		
		}
		//改变图片帧
		int cf=LOBYTE(k->code);
		int f=HIBYTE(k->code);
		cf++;
		if(cf>=6)
		{
			cf=0;
			f++;
			if(f>=3)
				f=0;
		}
		k->code=MAKEWORD(cf,f);

		//判断子弹和障碍物的碰撞
		if(k->_vz<0.00001&&k->_vz>-0.00001)
		{
			gData * gd=gm->GetSingle(k->_x/16,k->_y/16);
			if(gd&&(gd->id==100))
			{
				k->_vx=k->_vy=0;
				k->_vz=-1;
			}
		}
		//判断子弹和英雄的碰撞
		CHero *hero=m_gScene->GetHero();
		bool hittag=false;
		for(int t=0;t<2;t++)
		{
			float x,y,z;
			hero[t].GetXY(&x,&y,&z);
			POINT p={k->_x,k->_y};
			RECT r={x-5,y-5,x+5,y+5};
			if(PtInRect(&r,p))
			{
				hittag=true;
				float dx=x-k->_x;
				float dy=y-k->_y;
				hero[t].Hit(dx,dy);
				break;
			}
		}
		if(hittag)
		{
			k=m_Bullet.erase(k);
			continue;
		}
		k++;
	}
}