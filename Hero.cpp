#include "Hero.h"
#include <math.h>
#include "Game.h"
#include "First.h"
#include "TeXiao.h"
#include <Windows.h>
#include "Tools.h"
#define _SPEED 3.0
#define _PUSH_SPEED 0.6
void CHero::Init(CFirst * fst, float x, float y, unsigned char life)
{
	m_x=x;
	m_y=y;
	m_z=0;
	m_vx=0;
	m_vy=0;
	m_vz=0;
	m_Life=life;
	m_turn=2;
	m_GetKey=false;
	m_mirror=false;
	m_type=_ROLE_HERO;
	m_state=_STATE_AUTO;
	m_walkspd=1.3;
	m_gScene=fst;
	m_CurM=1;
	m_CurF=0;
	m_CurCF=0;
	m_Lift=_LIFT_NONE;
	m_Weapon=_WEAPON_WING;
	m_Push.id=0;
	m_hurtMove[0]=54;
	m_hurtMove[1]=55;
	m_hurtMove[2]=56;
	m_ShuiWen=new CTeXiao(65,0,0);
}
void CHero::Run()
{
	RunMoves();
	for(int i=0;i<8;i++)
		 m_KStime[i]++;
	m_statelast++;//状态计数增加
	int site=CFirst::GetMoves()->GetSite(m_CurM, m_CurF); 
	int preface=site/100;//还没考虑镜像时的面向
	int act=site%100;//动作
	if(preface!=4)
		m_mirror=false;
	//显示自己
	SinglePic mp(GetNo(),m_x,m_y+m_z*0.7,m_y);
	m_gScene->GetLayer(1)->push_back(mp);
	//如果有钥匙 在头顶显示钥匙
	if(m_GetKey)
	{
		SinglePic pys(35,m_x-7,m_y+m_z*0.7-28);
		m_gScene->GetLayer(2)->push_back(pys);		
	}
	//如果举着东西
	if(act==4||act==5||act==6)
	{
		int dx=CFirst::GetMoves()->GetBigX(m_CurM,m_CurF);
		int dy=CFirst::GetMoves()->GetBigY(m_CurM,m_CurF);
		SinglePic plift(m_Lift,m_x+dx,m_y+dy);
		m_gScene->GetLayer(2)->push_back(plift);	
	}
	for(std::list<Bullet>::iterator b=m_Bullet.begin();b!=m_Bullet.end();b++)
	{
		SinglePic p(8,b->_x+8,b->_y+8);
		m_gScene->GetLayer(1)->push_back(p);
		SinglePic pb(b->_id,b->_x,b->_y+b->_z*0.7);
		m_gScene->GetLayer(1)->push_back(pb);
	}
	//人物子弹控制
	for(std::list<Bullet>::iterator k=m_Bullet.begin();k!=m_Bullet.end();)
	{
		k->_x+=k->_vx;
		k->_y+=k->_vy;
		k->_z+=k->_vz;
		if(k->_z<0)
			k->_vz+=0.22;
		if(k->_id==_GOOD_GRASSBONE||k->_id==_GOOD_YST)
		{
			if(k->_z>=8)
			{
				//检测掉落物品 黑 绿 蓝 红
				if(k->code>=1&&k->code<=6)
				{
					unsigned char id=1;
					float x,y;
					if(k->code==1)
						id=12;
					else if(k->code==2)
						id=13;
					else if(k->code==3)
						id=14;
					else if(k->code==4)
						id=15;
					else if(k->code==6)
						id=34;
					extGood g={id,k->_x+4,k->_y+2,-0.1,0,0,-1.5,0};
					m_gScene->GetExtGood()->push_back(g);
				}
				m_gScene->GetTX()->push_back(CTeXiao(60,k->_x-4,k->_y+12));
				k=m_Bullet.erase(k);
				continue;
			}
			k++;
			continue;
		}
		if(k->_id==_GOOD_XSK)
		{
			int prex=m_Push.x/16;
			int prey=m_Push.y/16;
			int curx=k->_x/16;
			int cury=k->_y/16;
			if(m_Push.turn==2)
			{
				if(m_KeyState[_K_DOWN]==_KISS_DOWN||cury<=prey)
				{//继续向前推
					m_Push.x=k->_x;
					m_Push.y=k->_y;
					k++;
					continue;
				}				
				if(cury>prey&&m_KeyState[_K_DOWN]==_KISS_UP)
				{
					m_CurM=1;
					m_CurF=0;
					m_CurCF=0;
					m_gScene->GetGoodManager()->Add(m_Push.id,prex,prey+1,m_Push.code1,m_Push.code2);
					m_Push.id=0;
					k=m_Bullet.erase(k);
					continue;
				}
			}
			else if(m_Push.turn==8)
			{
				if(m_KeyState[_K_UP]==_KISS_DOWN||cury>=prey)
				{
					m_Push.x=k->_x;
					m_Push.y=k->_y;
					k++;
					continue;				
				}
				if(cury<prey&&m_KeyState[_K_UP]==_KISS_UP)
				{
					m_CurM=3;
					m_CurF=0;
					m_CurCF=0;
					m_gScene->GetGoodManager()->Add(m_Push.id,prex,prey,m_Push.code1,m_Push.code2);
					m_Push.id=0;
					k=m_Bullet.erase(k);
					continue;
				}
			}
			else if(m_Push.turn==4)
			{
				if(m_KeyState[_K_LEFT]==_KISS_DOWN||curx>=prex)
				{
					m_Push.x=k->_x;
					m_Push.y=k->_y;
					k++;
					continue;				
				}
				if(curx<prex&&m_KeyState[_K_LEFT]==_KISS_UP)
				{
					m_CurM=5;
					m_CurF=0;
					m_CurCF=0;
					m_gScene->GetGoodManager()->Add(m_Push.id,prex,prey,m_Push.code1,m_Push.code2);
					m_Push.id=0;
					k=m_Bullet.erase(k);
					continue;
				}			
			}
			else if(m_Push.turn==6)
			{
				if(m_KeyState[_K_RIGHT]==_KISS_DOWN||curx<=prex)
				{
					m_Push.x=k->_x;
					m_Push.y=k->_y;
					k++;
					continue;				
				}
				if(curx>prex&&m_KeyState[_K_RIGHT]==_KISS_UP)
				{
					m_CurM=5;
					m_CurF=0;
					m_CurCF=0;
					m_gScene->GetGoodManager()->Add(m_Push.id,prex+1,prey,m_Push.code1,m_Push.code2);
					m_Push.id=0;
					k=m_Bullet.erase(k);
					continue;
				}				
			}
		}
		else
			k++;
	}
	if(m_state==2)
	{
		if(m_statelast<120)
			return;
		else
		{
			//判断够不够钱复活
			if(m_gScene->SubMoney(5))
			{
				m_CurM=1;
				m_CurF=0;
				m_CurCF=0;
				m_state=0;
				m_statelast=0;
			}
			else
			{
				m_gScene->Complete(false);
			}
		}
	}
	//判断有没有踩到陷阱
	gData * footdata=m_gScene->GetGoodManager()->GetSingle(m_x/16,m_y/16);
	if(footdata&&footdata->id==102&&act!=15)
	{//踩到陷阱
		m_CurM=59;
		m_CurF=0;
		m_CurCF=0;
		m_x=footdata->x+8;
		m_y=footdata->y+8;
		return;
	}
	//如果在坠落
	if(act==15&&m_CurF==5&&m_CurCF==7)
	{
		m_x=174;
		m_y=486;
		Hit(0,0,1);
		return;
	}
	//获取面向的地图格子信息
	POINT facept;
	if(preface==2)
	{
		facept.x=m_x;
		facept.y=m_y+7;
	}
	else if(preface==8)
	{
		facept.x=m_x;
		facept.y=m_y-7;
	}
	else if(preface==4)
	{
		if(!m_mirror)
		{
			facept.x=m_x-7;
			facept.y=m_y;
		}
		else
		{
			facept.x=m_x+7;
			facept.y=m_y;
		}
	}
 	int facex=facept.x/16;
	int facey=facept.y/16;
	//如果是被攻击状态
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
			if(m_Life>0)
			{
				if(preface==2)
					m_CurM=1;
				else if(preface==8)
					m_CurM=3;
				else if(preface==4)
					m_CurM=5;
				m_state=0;
			}
			else
			{
				m_CurM=64;
				m_state=2;
			}
			m_CurF=0;
			m_CurCF=0;
			m_statelast=0;
		}
	}
	gData* facedata= m_gScene->GetGoodManager()->GetSingle(facex,facey);
	if(act==3&&facedata&&(facedata->id==32||facedata->id==33)&&m_GetKey)
	{//如果面向的是门并且获得了钥匙则开门
		if(facedata->id==32)
			m_gScene->GetGoodManager()->Del(facex+3,facey);
		else if(facedata->id==33)
			m_gScene->GetGoodManager()->Del(facex-3,facey);
		m_gScene->GetGoodManager()->Del(facex,facey);
		m_GetKey=false;
		return;
	}
	//按攻击
	if((act==0||act==1||act==3||(act==2&&m_CurF>3))&&m_KeyState[_K_B]==_KISS_DOWN&&m_KStime[_K_B]==1)
	{
		if(preface==2)
			m_CurM=7;
		else if(preface==8)
			m_CurM=8;
		else if(preface==4)
			m_CurM=9;
		m_CurF=0;
		m_CurCF=0;
		m_vx=0;
		m_vy=0;
		return;
	}
	//攻击时面向是否有草
	if(act==2&&m_CurF>3)
	{
		POINT actpt;
		if(preface==2)
		{
			actpt.x=m_x;
			actpt.y=m_y+19;
		}
		else if(preface==8)
		{
			actpt.x=m_x;
			actpt.y=m_y-19;
		}
		else if(preface==4)
		{
			if(!m_mirror)
			{
				actpt.x=m_x-19;
				actpt.y=m_y;
			}
			else
			{
				actpt.x=m_x+19;
				actpt.y=m_y;
			}
		}
		int actx=actpt.x/16;
		int acty=actpt.y/16;
		gData * actdata= m_gScene->GetGoodManager()->GetSingle(actx,acty);
		if(actdata&&actdata->id==_GOOD_GRASS)
		{//通过附加信息判断是否掉落宝石或者下面有机关陷阱之类的东西  并将草打掉 
			//附加信息 1黑宝石 2绿 3蓝 4红 5机关 6钥匙
			if(actdata->code1==1)
			{
				extGood blackbs={12,actx*16+4,acty*16+2,-0.1,0,0,-1.6,0};
				m_gScene->GetExtGood()->push_back(blackbs);
			}
			else if(actdata->code1==2)
			{
				extGood blackbs={13,actx*16+4,acty*16+2,-0.1,0,0,-1.6,0};
				m_gScene->GetExtGood()->push_back(blackbs);
			}
			else if(actdata->code1==3)
			{
				extGood blackbs={14,actx*16+4,acty*16+2,-0.1,0,0,-1.6,0};
				m_gScene->GetExtGood()->push_back(blackbs);
			}
			else if(actdata->code1==4)
			{
				extGood blackbs={15,actx*16+4,acty*16+2,-0.1,0,0,-1.6,0};
				m_gScene->GetExtGood()->push_back(blackbs);
			}
			else if(actdata->code1==5)
			{
				m_gScene->GetGoodManager()->Add(4,actx,acty,0,actdata->code2);
			}
			else if(actdata->code1==6)
			{//钥匙
				extGood ys={34,actx*16+4,acty*16+2,-0.1,0,0,-1.6,0};
				m_gScene->GetExtGood()->push_back(ys);				
			}
			m_gScene->GetTX()->push_back(CTeXiao(60,actx*16-4,acty*16+12));
			m_gScene->GetGoodManager()->Del(actx,acty);
		}
	}
	//判断有没有打到怪物或者物品
	if(act==2)
	{
		std::list<CRole*> *monsters=m_gScene->GetMonster();
		MyRect* mr=CFirst::GetMoves()->GetActRect(m_CurM, m_CurF);
		fRect c;
		for(int n=0;n<CFirst::GetMoves()->GetActLength(m_CurM, m_CurF);n++)
		{
			fRect r={m_x+(m_mirror?-mr[n]._x2:mr[n]._x1),m_y+mr[n]._y1,m_x+(m_mirror?-mr[n]._x1:mr[n]._x2),m_y+mr[n]._y2};
			//每个矩形与每个怪物碰撞
			for(std::list<CRole*>::iterator k=monsters->begin();k!=monsters->end();)
			{
				float x,y;
				(*k)->GetXY(&x,&y);
				fRect cr={x-8,y-8,x+8,y+8};
				if(CTools::RectCrash(&c,&r,&cr))
				{
					k=monsters->erase(k);
					CTeXiao tx(62,x,y);
					m_gScene->GetTX()->push_back(tx);
					continue;
				}
				k++;
			}
		}
	}
	//推动物品
	if(m_Push.id!=0)
	{

	}
	//停止推动
	if(act==3&&m_Push.id==0)
	{
		int x=m_x/16;
		int y=m_y/16;
		if(preface==2&&(m_KeyState[_K_DOWN]==_KISS_UP||m_gScene->GetGoodManager()->GetSingle(x,y+1)==0))
		{
			m_CurM=1;
			m_CurF=0;
			m_CurCF=0;
		}
		else if(preface==8&&(m_KeyState[_K_UP]==_KISS_UP||m_gScene->GetGoodManager()->GetSingle(x,y-1)==0))
		{
			m_CurM=3;
			m_CurF=0;
			m_CurCF=0;
		}
		else if(preface==4)
		{
			if((m_mirror&&(m_KeyState[_K_RIGHT]==_KISS_UP||m_gScene->GetGoodManager()->GetSingle(x+1,y)==0))||(!m_mirror&&(m_KeyState[_K_LEFT]==_KISS_UP||m_gScene->GetGoodManager()->GetSingle(x-1,y)==0)))
			{
				m_CurM=5;
				m_CurF=0;
				m_CurCF=0;
			}
		}
	}
	//按举起 16 18 20 举着走 13 14 15 直接走路1 3 5  (act==0||act==1)&&
	if(m_KeyState[_K_R]==_KISS_DOWN&&m_KStime[_K_R]==1)
	{//举起键  动作码编号 0 走动 1 举起5 举着 6 举着走4 跳跃 9
		if((act==0||act==1||act==3))
		{
			if(preface==2)
				m_CurM=22;
			else if(preface==8)
				m_CurM=23;
			else if(preface==4)
				m_CurM=24;
			m_CurF=0;
			m_CurCF=0;
			m_vx=0;
			m_vy=0;
		}
		if(act==6||act==4)
		{//仍出去
			if(preface==2)
				m_CurM=25;
			else if(preface==8)
				m_CurM=26;
			else if(preface==4)
				m_CurM=27;	
			m_CurF=0;
			m_CurCF=0;
			m_vx=0;
			m_vy=0;
			//发射子弹
			float bx,by,bz,bvx,bvy,bvz;
			bx=m_x-8;
			by=m_y-8;
			bz=-15.7-8/0.7;
			bvx=bvy=bvz=0;
			if(preface==2)
				bvy=2.2;
			else if(preface==8)
				bvy=-2.2;
			else if(preface==4)
				bvx=m_mirror?2.2:-2.2;
			bvz=-2;
			Bullet b={m_Lift, bx, by, bz, bvx, bvy, bvz,m_LiftCode};
			m_Bullet.push_back(b);
		}
		return;
	}
	//举起动作时看其面向的格子是什么
	if(act==7)
	{
		if(facedata&&(facedata->id==1||facedata->id==6))
		{
			//将草举起来
			if(preface==2)
				m_CurM=16;		
			else if(preface==8)
				m_CurM=18;	
			else if(preface==4)
				m_CurM=20;	
			m_CurF=0;
			m_CurCF=0;
			m_vx=0;
			m_vy=0;	
			m_Lift=facedata->id==1?_LIFT_GRASS:_GOOD_YST;
			m_LiftCode=facedata->code1;
			int code2=facedata->code2;
			m_gScene->GetGoodManager()->Del(facex,facey);
			if(m_LiftCode==5)
			{
				//生成机关
				m_LiftCode=1;
				m_gScene->GetGoodManager()->Add(4,facex,facey,0,code2);
			}

		}
		return;
	}
	//按道具键 28 29 30 (act==0||act==1)&&
	if(m_KeyState[_K_A]==_KISS_DOWN&&m_KStime[_K_A]==1)
	{
		if(act==0||act==1)
		{//如果是在走路
			//如果面向宝箱按道具键则打开宝箱
			if(facedata&&preface==8&&facedata->id==_GOOD_GBOX)
			{
				m_CurM=43;
				m_CurF=0;
				m_CurCF=0;
				m_gScene->GetGoodManager()->Del(facex,facey);
				m_gScene->GetGoodManager()->Add(_GOOD_KBOX,facex,facey,0,0);
			}
			else
			{
				//使用道具 跳跃
				if(preface==2)
					m_CurM=28;
				else if(preface==8)
					m_CurM=29;
				else if(preface==4)
					m_CurM=30;
				m_CurF=0;
				m_CurCF=0;
				m_vz=-2;
			}
		}
		else if(act==9)
		{//如果是跳跃状态 站立状体码编号 0 走动 1 举起5 举着 6 举着走4 跳跃 9
			if(preface==2)
				m_CurM=31;
			else if(preface==8)
				m_CurM=33;
			else if(preface==4)
				m_CurM=35;	
			m_vz=-2;
			m_CurF=0;
			m_CurCF=0;
		}
		else if(act==10)
		{
			m_vz=-2;
		}
	}
	//跳跃过程中 z向速度
	if(act==9)
	{//跳跃状态
		if(m_CurF==8&&m_CurCF==6)
			m_z=m_vz=0;
		else
			m_vz+=0.1;
	}
	if(act==10)
	{//飞行状态
		m_vz+=0.08;
		if(m_z>=0)
		{
			m_vz=m_z=0;
			if(preface==2)
				m_CurM=1;
			else if(preface==8)
				m_CurM=3;
			else if(preface==4)
				m_CurM=5;
			m_CurF=0;
			m_CurCF=0;
		}
	}
	unsigned short direct=m_PM.GetDirect();
	unsigned char turn=*(unsigned char*)&direct;//移动方向
	unsigned char face=*((unsigned char*)&direct+1);//面向
	//判断游泳
	if(footdata&&footdata->id==104&&m_z>=0)
	{
		if(act!=11&&act!=12)
		{
			if(preface==2)
				m_CurM=37;
			else if(preface==8)
				m_CurM=39;
			else if(preface==4)
				m_CurM=41;
			m_CurF=0;
			m_CurCF=0;
			if(preface==2||preface==8)
			{
				m_y=(int)m_y/16*16+8;
			}
			else if(preface==4)
			{
				m_x=(int)m_x/16*16+8;
			}		
			m_gScene->GetTX()->push_back(CTeXiao(66,m_x,m_y));
		}
	}
	if(footdata&&footdata->id==103)
	{
		if(act==11||act==12)
		{
			if(preface==2)
				m_CurM=1;
			else if(preface==8)
				m_CurM=3;
			else if(preface==4)
				m_CurM=5;
			m_CurF=0;
			m_CurCF=0;			
		}
		if(act!=11&&act!=12&&act!=10&&act!=9)
		{
			m_ShuiWen->Run();
			SinglePic psw(m_ShuiWen->GetNo(),m_x,m_y);
			m_gScene->GetLayer(2)->push_back(psw);
		}
	}
		//先显示影子
	if((!(footdata&&(footdata->id==103||footdata->id==104))||act==9||act==10)&&m_shade)
	{
		SinglePic pyz(8,m_x,m_y);
		m_gScene->GetLayer(0)->push_back(pyz);	
	}
	//人物移动判定
	//先判断人物动作改变
	//站立状体码编号 0 走动 1 举起5 举着 6 举着走4 跳跃 9 水中11 游泳12
	if(face==0&&(act==1||act==4||act==12))
	{//没有任何按键 并且之前人物在走动
		if(preface==2)
		{
			if(act==1)
				m_CurM=1;
			else if(act==4)
				m_CurM=17;
			else if(act==12)
				m_CurM=37;
		}
		else if(preface==4)
		{
			if(act==1)
				m_CurM=5;
			else if(act==4)
				m_CurM=21;
			else if(act==12)
				m_CurM=41;
		}
		else if(preface==8)
		{
			if(act==1)
				m_CurM=3;
			else if(act==4)
				m_CurM=19;
			else if(act==12)
				m_CurM=39;
		}
		m_CurF=0;
		m_CurCF=0;
	}
	//站立状体码编号 0 走动 1 举起5 举着 6 举着走4 跳跃 9
	//动作码举起 16 18 20 举着走 13 14 15 直接走路1 3 5 举着17 19 21
	//按了键要更换动作的情况  以前没动现在要动  以前在动但动的面向改变了
	else if(face!=0&&(act==0||act==6||act==11||((act==1||act==4||act==12)&&((!m_mirror&&preface!=face)||(m_mirror&&face!=6)))))
	{
		if(face==2)
		{
			if(act==0||act==1)
				m_CurM=2;
			else if(act==4||act==6)
				m_CurM=13;
			else if(act==11||act==12)
				m_CurM=38;
		}
		else if(face==4||face==6)
		{
			if(act==0||act==1)
				m_CurM=6;
			else if(act==4||act==6)
				m_CurM=15;
			else if(act==11||act==12)
				m_CurM=42;
			m_mirror=face==6?true:false;
		}
		else if(face==8)
		{
			if(act==0||act==1)
				m_CurM=4;
			else if(act==4||act==6)
				m_CurM=14;
			else if(act==11||act==12)
				m_CurM=40;		
		}
		m_CurF=0;
		m_CurCF=0;
	}
	////判断人物速度改变
	if(!(m_state==1||act==2||(act==3&&m_Push.id!=0)||act==5))
	{
		if(turn==1)
		{
			m_vx=-m_walkspd*0.71;
			m_vy=-m_vx;
		}
		else if(turn==2)
		{
			m_vx=0;
			m_vy=m_walkspd;
			if(act==3)
				m_vy=_PUSH_SPEED;
		}
		else if(turn==3)
		{
			m_vx=m_walkspd*0.71;
			m_vy=m_vx;
		}
		else if(turn==4)
		{
			m_vx=-m_walkspd;
			m_vy=0;
			if(act==3)
				m_vx=-_PUSH_SPEED;
		}
		else if(turn==6)
		{
			m_vx=m_walkspd;
			m_vy=0;
			if(act==3)
				m_vx=_PUSH_SPEED;
		}
		else if(turn==7)
		{
			m_vx=-m_walkspd*0.71;
			m_vy=m_vx;		
		}
		else if(turn==8)
		{
			m_vx=0;
			m_vy=-m_walkspd;
			if(act==3)
				m_vy=-_PUSH_SPEED;
		}
		else if(turn==9)
		{
			m_vx=m_walkspd*0.71;
			m_vy=-m_vx;			
		}
		else if(turn==0&&act!=3)
		{
			m_vx=0;
			m_vy=0;
		}
	}
	m_x+=m_vx;
	m_y+=m_vy;
	m_z+=m_vz;
}
//接收按键输入
void CHero::KeyIn(unsigned char n,unsigned char ks)
{
	m_KeyState[n]=ks;
	m_KStime[n]=0;
	if(n==_K_UP)
	{
		if(ks==_KISS_DOWN)
			m_PM.Push(8);
		else
			m_PM.Eraser(8);
	}
	else if(n==_K_DOWN)
	{
		if(ks==_KISS_DOWN)
			m_PM.Push(2);
		else
			m_PM.Eraser(2);	
	}
	else if(n==_K_LEFT)
	{
		if(ks==_KISS_DOWN)
			m_PM.Push(4);
		else
			m_PM.Eraser(4);	
	}
	else if(n==_K_RIGHT)
	{
		if(ks==_KISS_DOWN)
			m_PM.Push(6);
		else
			m_PM.Eraser(6);	
	}
}

unsigned char CHero::GetLift()
{
	return m_Lift;
}
unsigned char CHero::GetLife()
{
	return m_Life;
}
//获得钥匙
void CHero::GetKey()
{
	m_GetKey=true;
}
//处理被动碰撞事件，也就是走路时碰到一些东西
void CHero::HandleCrash(gData* gd)
{
	int x=m_x/16;
	int y=m_y/16;
	int site=CFirst::GetMoves()->GetSite(m_CurM, m_CurF); 
	int preface=site/100;//还没考虑镜像时的面向
	int act=site%100;//动作
	if(gd->id==101)
	{
		SinglePic sp(MAKEWORD(act==0?0:m_CurF%4,61),m_x,m_y);
		m_gScene->GetLayer(2)->push_back(sp);
		return;
	}	
	if(gd->id==4)
	{//踩到机关
		SinglePic sp(5,gd->x,gd->y);
		m_gScene->GetLayer(1)->push_front(sp);
		unsigned long code=gd->code2;
		int x=LOWORD(code);
		int y=HIWORD(code);
		gData * door=m_gScene->GetGoodManager()->GetSingle(x,y);
		if(door)
		{
			m_gScene->GetGoodManager()->Del(x,y);
			m_gScene->GetGoodManager()->Del(x+3,y);
		}
		return;
	}
	if(act==1)
	{
		bool tag=false;//表示是否可以从边界滑出
		if((preface==2&&gd->y>y*16)||(preface==8&&gd->y<y*16))
		{
			int cd=m_x-gd->x;
			tag=true;
			if(cd>11)
			{
				//检测是否可以从右侧滑出
				if(m_gScene->GetGoodManager()->GetSingle(gd->x/16+1,gd->y/16))
					tag=false;
				else
					m_x+=0.8;
			}
			else if(cd<5)
			{
				//检测是否可以从左侧滑出
				if(m_gScene->GetGoodManager()->GetSingle(gd->x/16-1,gd->y/16))
					tag=false;
				else
					m_x-=0.8;			
			}
			else
				tag=false;
			if(!tag)
			{
				if(m_KeyState[_K_DOWN]==_KISS_DOWN&&preface==2)
				{
					m_CurM=10;
					m_CurF=0;
					m_CurCF=0;
				}  
				else if(m_KeyState[_K_UP]==_KISS_DOWN&&preface==8)
				{
					m_CurM=11;
					m_CurF=0;
					m_CurCF=0;
				}
			}
		}
		else if(preface==4)
		{
			int cd=m_y-gd->y;
			tag=true;//表示是否可以从边界滑出
			if(cd>11)
			{
				//检测是否可以从下侧滑出
				if(m_gScene->GetGoodManager()->GetSingle(gd->x/16,gd->y/16+1))
					tag=false;
				else
					m_y+=0.8;
			}
			else if(cd<5)
			{
				//检测是否可以从上侧滑出
				if(m_gScene->GetGoodManager()->GetSingle(gd->x/16,gd->y/16-1))
					tag=false;
				else
					m_y-=0.8;			
			}
			else
				tag=false;
			if(!tag&&((m_mirror&&m_KeyState[_K_RIGHT]==_KISS_DOWN)||(!m_mirror&&m_KeyState[_K_LEFT]==_KISS_DOWN)))
			{
				m_CurM=12;
				m_CurF=0;
				m_CurCF=0;
			}
		}
		if(!tag)
		{
			//根据碰到的不同东西执行不同操作
			if(gd->id==_GOOD_XSK)
			{
				//把石块当成子弹向某个方向发射
				float bx,by,bz,bvx,bvy,bvz;
				bz=0;
				bvx=bvy=bvz=0;
				m_Push.id=gd->id;
				m_Push.x=gd->x;
				m_Push.y=gd->y;
				m_Push.turn=preface;
				if(preface==4&&m_mirror)
					m_Push.turn=6;
				m_Push.code1=gd->code1;
				m_Push.code2=gd->code2;
				if(preface==2)
				{
					bvy=m_vy=_PUSH_SPEED;
					m_Push.y++;
					//m_vy=1;
				}
				else if(preface==8)
				{
					bvy=m_vy=-_PUSH_SPEED;
					m_Push.y--;
					//m_vy=-1;
				}
				else if(preface==4)
				{
					if(m_mirror)
					{
						m_Push.x++;
						bvx=m_vx=_PUSH_SPEED;
					}
					else
					{
						m_Push.x--;
						bvx=m_vx=-_PUSH_SPEED;
					}
					//m_vx=bvx;
				}
				bx=m_Push.x;
				by=m_Push.y;
				Bullet b={_GOOD_XSK, bx, by, bz, bvx, bvy, bvz};
				m_Bullet.push_back(b);
				m_gScene->GetGoodManager()->Del(gd->x/16,gd->y/16);
			}
		}
	}
}
