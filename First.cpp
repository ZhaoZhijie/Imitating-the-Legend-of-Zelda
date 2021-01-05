#include "First.h"
#include <algorithm>
#include "Tools.h"
#include "Pig.h"
#define _SPEED 1
#define _CLIENT_W 240
#define _CLIENT_H 160
#define _MAP_W 1000
#define _MAP_H 1000

CMoves CFirst::m_Moves;
CGoods CFirst::m_Goods;
//构造函数
CFirst::CFirst(const char * name1, const char * name2)
{
	m_GoodsList.clear();
	m_extGood.clear();
	m_TX.clear();
	m_Fire.clear();
	m_Door.clear();
	m_Monster.clear();
	m_gameState=0;
	memset(m_ActBack,0,sizeof(m_ActBack));
	m_ActBack[0].curM=63;//火焰
	m_ActBack[1].curM=67;//河流
	m_p[0].Init(this,917,906,300);
	m_p[1].Init(this,947,906,300);
	//添加几个怪物做测试
	for(int n=0;n<30;n++)
	{
		m_Monster.push_back(new CPig(this,669+(rand()%240-120),265+rand()%90-45));		
	}
	m_Money=0;
	//加载地图数据
	FILE *fp=fopen("Data\\map.data","rb");
	if(!fp)
	{
		MessageBox(NULL,"打开地图失败","提示",0);
	}
	unsigned long goodsize;
	fread(&goodsize,sizeof(goodsize),1,fp);
	preGood pg;
	for(int i=0;i<goodsize;i++)
	{
		fread(&pg,sizeof(preGood),1,fp);
		if(pg.id==105||pg.id==106)
		{//遇到火焰和门顶
			POINT p={pg.x*16,pg.y*16};
			if(pg.id==105)
				m_Fire.push_back(p);
			else
				m_Door.push_back(p);
			continue;
		}
		m_GoodManager.Add(pg.id,pg.x,pg.y,pg.code1,pg.code2);
		if(pg.id==9||pg.id==10)
		{//遇到大石头和大石块
			m_GoodManager.Add(pg.id,pg.x+1,pg.y,1);
			m_GoodManager.Add(pg.id,pg.x,pg.y+1,2);
			m_GoodManager.Add(pg.id,pg.x+1,pg.y+1,3);
		}
	}
	fclose(fp);
}
void CFirst::Input(int id, int type, int p1, int p2)
{
	if(p1==_GI_K_W&&p2==_KS_DC)//按下向上
		m_p[id].KeyIn(_K_UP, _KISS_DOWN);
	if(p1==_GI_K_W&&p2==_KS_UC)//弹起向下
		m_p[id].KeyIn(_K_UP, _KISS_UP);
	if(p1==_GI_K_S&&p2==_KS_DC)//按下向下
		m_p[id].KeyIn(_K_DOWN, _KISS_DOWN);
	if(p1==_GI_K_S&&p2==_KS_UC)//弹起向下
		m_p[id].KeyIn(_K_DOWN, _KISS_UP);
	if(p1==_GI_K_A&&p2==_KS_DC)//按下向左
		m_p[id].KeyIn(_K_LEFT, _KISS_DOWN);
	if(p1==_GI_K_A&&p2==_KS_UC)//弹起向左
		m_p[id].KeyIn(_K_LEFT, _KISS_UP);
	if(p1==_GI_K_D&&p2==_KS_DC)//按下向右
		m_p[id].KeyIn(_K_RIGHT, _KISS_DOWN);
	if(p1==_GI_K_D&&p2==_KS_UC)//弹起向右
		m_p[id].KeyIn(_K_RIGHT, _KISS_UP);
	if(p1==_GI_K_J&&p2==_KS_DC)//按下A
		m_p[id].KeyIn(_K_A, _KISS_DOWN);
	if(p1==_GI_K_J&&p2==_KS_UC)//弹起A
		m_p[id].KeyIn(_K_A, _KISS_UP);
	if(p1==_GI_K_K&&p2==_KS_DC)//按下B
		m_p[id].KeyIn(_K_B, _KISS_DOWN);
	if(p1==_GI_K_K&&p2==_KS_UC)//弹起B
		m_p[id].KeyIn(_K_B, _KISS_UP);
	if(p1==_GI_K_U&&p2==_KS_DC)//按下L
		m_p[id].KeyIn(_K_L, _KISS_DOWN);
	if(p1==_GI_K_U&&p2==_KS_UC)//弹起L
		m_p[id].KeyIn(_K_L, _KISS_UP);
	if(p1==_GI_K_I&&p2==_KS_DC)//按下R
		m_p[id].KeyIn(_K_R, _KISS_DOWN);
	if(p1==_GI_K_I&&p2==_KS_UC)//弹起R
		m_p[id].KeyIn(_K_R, _KISS_UP);
}
//两个角色y坐标的比较
bool Compare(SinglePic &p1, SinglePic &p2)
{
	return p1.order<p2.order;
}
bool CFirst::Output(int id, char ** data, int *len,int * myvirX,int * myvirY)
{
	//先计算视口坐标
	int virX;
	int virY;
	float fx,fy;
	m_p[id].GetXY(&fx,&fy);
	int x1,y1;
	x1=fx;
	y1=fy;
	if(x1<_CLIENT_W/2)
		virX=0;
	else if(x1>_MAP_W-_CLIENT_W/2)
		virX=_MAP_W-_CLIENT_W;
	else 
		virX=x1-_CLIENT_W/2;
	if(y1<_CLIENT_H/2)
		virY=0;
	else if(y1>_MAP_H-_CLIENT_H/2)
		virY=_MAP_H-_CLIENT_H;
	else 
		virY=y1-_CLIENT_H/2;
	m_virX=virX;
	m_virY=virY;
	*myvirX=virX;
	*myvirY=virY;
	//统计所有显示对象个数 一个显示对象占6个字节 
	int goodsize=0;
	gData ** gdata=m_GoodManager.GetList();
	int starty=virY/16;
	int endy=starty+11;
	for(int cury=starty;cury<endy;cury++)
	{
		//扫描每一行物品
		gData * curp=gdata[cury]->next;
		while(curp)
		{
			if(curp->id>=100)
			{
				curp=curp->next;
				continue;
			}
			if(curp->x+16>=virX&&curp->x<virX+240)
			{
				SinglePic sp(curp->id,0,0,0);
				if(curp->id==9||curp->id==10)
				{
					if(curp->code1==0)
					{
						sp.x=curp->x;
						sp.y=curp->y;
						m_Layer[0].push_back(sp);
					}
					else if(curp->code1==1)
					{
						sp.x=curp->x-16;
						sp.y=curp->y;	
						m_Layer[0].push_back(sp);
					}
					else if(curp->code1==2)
					{
						sp.x=curp->x;
						sp.y=curp->y-16;	
						m_Layer[0].push_back(sp);
					}
					else if(curp->code1==3)
					{
						sp.x=curp->x-16;
						sp.y=curp->y-16;	
						m_Layer[0].push_back(sp);
					}
				}
				else
				{
					sp.x=curp->x;
					sp.y=curp->y;	
					m_Layer[0].push_back(sp);
				}
			}
			if(curp->x>=virX+240)
				break;
			curp=curp->next;
		}
	}
	//遍历宝石
	for(std::list<extGood>::iterator g=m_extGood.begin();g!=m_extGood.end();g++)
	{
		if(g->cf<500||g->cf%3==0)
		{
			SinglePic bs(g->id,g->x,g->y+g->z*0.7,g->y);
			m_Layer[0].push_back(bs);
		}
	}
	//火焰
	for(std::list<POINT>::iterator n=m_Fire.begin();n!=m_Fire.end();n++)
	{
		SinglePic hy(MAKEWORD(m_ActBack[0].curF,m_ActBack[0].curM),n->x,n->y);
		m_Layer[0].push_back(hy);
	}
	//河流
	SinglePic hl(MAKEWORD(m_ActBack[1].curF,m_ActBack[1].curM),0,784);
	m_Layer[0].push_front(hl);
	//门顶
	for(std::list<POINT>::iterator n=m_Door.begin();n!=m_Door.end();n++)
	{
		SinglePic md(29,n->x,n->y);
		m_Layer[2].push_back(md);
	}
	//血量 金钱
	SinglePic pic_gold(16,197+virX,146+virY);
	unsigned char num1,num2,num3;
	num1=m_Money/100;
	num2=m_Money%100/10;
	num3=m_Money%100%10;
	SinglePic pic_gfst(17+num1,208+virX,147+virY);
	SinglePic pic_gsed(17+num2,216+virX,147+virY);
	SinglePic pic_gsrd(17+num3,224+virX,147+virY);
	m_Layer[2].push_back(pic_gold);
	m_Layer[2].push_back(pic_gfst);
	m_Layer[2].push_back(pic_gsed);
	m_Layer[2].push_back(pic_gsrd);
	//血量
	unsigned char myLife=m_p[id].GetLife();
	for(unsigned char p=1;p<=myLife;p++)
	{
		SinglePic pic_life(27,127+virX+(p-1)*8,8+virY);
		m_Layer[2].push_back(pic_life);
	}
	//判断有没有结束
	if(m_gameState==2)
	{
		SinglePic picback(36,0+virX,0+virY);
		m_Layer[2].push_back(picback);
		float ytxt=160-2*m_count;
		if(ytxt<25)
			ytxt=25;
		SinglePic pictxt(37,12+virX,ytxt+virY);
		m_Layer[2].push_back(pictxt);
	}
	goodsize+=GetPicSize()+m_TX.size();

	*len=20+8+goodsize*8;
	*data=new char[*len];
	char unit[8];
	int i=0;
	//显示背景地图
	*(short*)&unit[0]=0;
	*(short*)&unit[2]=-virX;
	*(short*)&unit[4]=-virY;
	memcpy(*data+20+i*8, unit, 8);
	i++;
	SinglePic * fstPic;
	for(int n=0;n<3;n++)
	{
		if(n==1)
			fstPic=(SinglePic*)(*data+20+i*8);
		for(std::list<SinglePic>::iterator k=m_Layer[n].begin();k!=m_Layer[n].end();k++)
		{
			k->x-=virX;
			k->y-=virY;
			memcpy(*data+20+i*8, &*k, 8);
			i++;				
		}
	}
	//对第二层的数据排序(需要排序的都放在了第二层)
	std::sort(fstPic,fstPic+m_Layer[1].size(),Compare);
	//显示特效
	for(std::list<CTeXiao>::iterator t=m_TX.begin();t!=m_TX.end();t++)
	{
		float x,y;
		t->GetXY(&x,&y);
		unsigned no=t->GetNo();
		*(unsigned short*)&unit[0]=t->GetNo();
		*(unsigned short*)&unit[2]=x-virX;
		*(unsigned short*)&unit[4]=y-virY;
		memcpy(*data+20+i*8, unit, 8);
		i++;				
	}
	return true;
}
void CFirst::Run()
{
	for(int n=0;n<2;n++)
	{
		m_ActBack[n].curCF++;
		if(m_ActBack[n].curCF>=CFirst::GetMoves()->GetContinueFrame(m_ActBack[n].curM,m_ActBack[n].curF))
		{
			m_ActBack[n].curCF=0;
			m_ActBack[n].curF++;
			if(m_ActBack[n].curF>=CFirst::GetMoves()->GetFrameLen(m_ActBack[n].curM))
			{
				m_ActBack[n].curF=0;
			}		
		}
	}
	m_count++;
	LayerClear();
	if(m_gameState==2)
		return;
	for(std::list<CTeXiao>::iterator i=m_TX.begin();i!=m_TX.end();)
	{
		if(!i->Run())
			i=m_TX.erase(i);
		else 
			i++;
	}
	for(int i=0;i<2;i++)
	{
		m_p[i].Run();
		float x,y,z,vx,vy,vz;
		m_p[i].GetXY(&x,&y,&z);
		m_p[i].GetVXY(&vx,&vy,&vz);
		int site=m_p[i].GetSite();
		int act=site%100;
		//人与掉落物体的碰撞
		fRect r;
		fRect rRole={x-5,y-5,x+5,y+5};
		for(std::list<extGood>::iterator g=m_extGood.begin();g!=m_extGood.end();)
		{
			//移动掉落物品
			if(g->cf>700&&g->id!=34)//钥匙不能消失
			{
				g=m_extGood.erase(g);
				continue;
			}
			g->x+=g->vx;
			g->y+=g->vy;
			g->z+=g->vz;
			g->vz+=0.1;
			if(g->z>0&&g->vz>0)
			{
				g->z=0;
				if(g->vz<1 )
				{
					g->vx=g->vy=g->vz=0;
				}
				else
				{
					g->vz=-g->vz/2;
				}
			}
			fRect rgood={g->x,g->y,g->x+7,g->y+14};
			if(g->id==35)
			{
				rgood._right=rgood._left+15;
				rgood._bottom=rgood._top+15;
			}
			if(CTools::RectCrash(&r,&rRole,&rgood))
			{
				//获得宝石
				if(g->id!=34)
				{
					m_Money+=5;
					g=m_extGood.erase(g);
				}
				else 
				{
					m_p[i].GetKey();
					g=m_extGood.erase(g);
				}
				continue;
			}
			g->cf++;
			g++;
		}
		bool tag=false;
		//人与箱子 草 石头等的碰撞按人所占据的矩形覆盖的地图格子判断，以锚点为中心的10x10矩形
		int gx1=(x-5)/16;
		int gy1=(y-5)/16;
		int gx2=(x+5)/16;
		int gy2=(y+5)/16;
		int zb[4][2]={{gx1,gy1},{gx2,gy1},{gx1,gy2},{gx2,gy2}};
		//根据人物的速度方向 调整碰撞顺序 以防止在连续障碍处卡住
		int order[4]={0,1,2,3};
		if(vx<0)
		{
			int tmp;
			tmp=order[0];
			order[0]=order[1];
			order[1]=tmp;
			tmp=order[2];
			order[2]=order[3];
			order[3]=tmp;
		}
		if(vy<0)
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
			gData* gd=m_GoodManager.GetSingle(zb[order[k]][0],zb[order[k]][1]);
			if(gd)
			{
				fRect r;
				fRect rRole={x-5,y-5,x+5,y+5};
				fRect rBlock={zb[order[k]][0]*16, zb[order[k]][1]*16,rBlock._left+16,rBlock._top+16};
				if(CTools::RectCrash(&r,&rRole,&rBlock))
				{
					if(gd->id!=101&&gd->id!=4&&gd->id!=102&&gd->id!=103&&gd->id!=104)
					{
						CTools::CrashAdjust(rRole, rBlock, &x, &y, vx, vy);
						m_p[i].SetXY(x,y,z);
					}
					if(gd->id!=102&&gd->id!=103&&gd->id!=104)
						m_p[i].HandleCrash(gd);
				}
			}
			else if(act==11||act==12)
			{//在水中时如果周围是陆地要被挡住
				fRect r;
				fRect rRole={x-5,y-5,x+5,y+5};
				fRect rBlock={zb[order[k]][0]*16, zb[order[k]][1]*16,rBlock._left+16,rBlock._top+16};		
				CTools::CrashAdjust(rRole, rBlock, &x, &y, vx, vy);
				m_p[i].SetXY(x,y,z);
			}
		}
	}
	for(std::list<CRole*>::iterator n=m_Monster.begin();n!=m_Monster.end();n++)
	{
		(*n)->Run();
	}
	fRect c;
	//怪物之间的体积碰撞 和怪物与人的碰撞
	for(std::list<CRole*>::iterator n=m_Monster.begin();n!=m_Monster.end();n++)
	{
		float gx,gy,gz,gvx,gvy,gvz;
		int gsite=(*n)->GetSite();
		int gpreface=gsite/100;
		(*n)->GetXY(&gx,&gy,&gz);
		(*n)->GetVXY(&gvx,&gvy,&gvz);
		//和人之间的碰撞
		for(int b=0;b<2;b++)
		{
			float bx,by,bz,bvx,bvy,bvz;
			m_p[b].GetXY(&bx,&by,&bz);
			m_p[b].GetVXY(&bvx,&bvy,&bvz);		
			fRect r={gx-8,gy-8,gx+8,gy+8};
			fRect rk={bx-5,by-5,bx+5,by+5};
			if(CTools::RectCrash(&c,&r,&rk))
			{
				CTools::CrashAdjust(r, rk, &gx, &gy, gvx, gvy,&bx,&by,bvx,bvy);
				(*n)->SetXY(gx,gy,gz);
				m_p[b].SetXY(bx,by,bz);
				//根据双方连心的方向将人弹开
				float dx=bx-gx;
				float dy=by-gy;
				m_p[b].Hit(dx,dy);
			}
		}
		//与其它怪物的碰撞 
		std::list<CRole*>::iterator k=n;
		k++;
		for(k;k!=m_Monster.end();k++)
		{
			float tx,ty,tz,tvx,tvy,tvz;
			int ksite=(*k)->GetSite();
			int kpreface=ksite/100;
			(*k)->GetXY(&tx,&ty,&tz);	
			(*k)->GetVXY(&tvx,&tvy,&tvz);	
			fRect r={gx-8,gy-8,gx+8,gy+8};
			fRect rk={tx-8,ty-8,tx+8,ty+8};
			if(CTools::RectCrash(&c,&r,&rk))
			{
				CTools::CrashAdjust(r, rk, &gx, &gy, gvx, gvy,&tx,&ty,tvx,tvz);
				(*n)->SetXY(gx,gy,gz);
				(*k)->SetXY(tx,ty,tz);
			}
		}	
	}
}


std::list<Good> * CFirst::GetGoodlist()
{
	return &m_GoodsList;
}
std::list<CTeXiao>* CFirst::GetTX()
{
	return &m_TX;
}
std::list<SinglePic>* CFirst::GetLayer(int n)
{
	if(n<0||n>2)
		return 0;
	return &m_Layer[n];
}
int CFirst::GetPicSize()
{
	return m_Layer[0].size()+m_Layer[1].size()+m_Layer[2].size();
}
void CFirst::LayerClear()
{
	m_Layer[0].clear();
	m_Layer[1].clear();
	m_Layer[2].clear();
}
std::list<Bullet> * CFirst::GetBullet()
{
	return &m_Bullet;
}
std::list<extGood>* CFirst::GetExtGood()
{
	return &m_extGood;
}
void CFirst::GetVirXY(int *x, int *y)
{
	*x=m_virX;
	*y=m_virY;
}
void CFirst::GetXY(float *x, float *y)
{
	m_p[0].GetXY(x,y);
}
	
//初始化所有角色的动作
void CFirst::InitData()
{
	//初始化动作数据
	//站立向下
	m_Moves.AddFrame(1,0,  5,0,18,24,20,10,18,200);
	m_Moves.AddFrame(1,0,35,0,18,24,20,10,18,200);
	m_Moves.AddFrame(1,0,65,0,18,24,20,10,18,200);
	//行走向下
	m_Moves.AddFrame(2,0,  95,0,18,24,3,10,18,201);
	m_Moves.AddFrame(2,0,125,0,18,24,3,10,20,201);
	m_Moves.AddFrame(2,0,155,0,18,24,3,10,20,201);
	m_Moves.AddFrame(2,0,185,0,18,24,3,10,20,201);
	m_Moves.AddFrame(2,0,215,0,18,24,3,10,19,201);
	m_Moves.AddFrame(2,0,245,0,18,24,3,10,19,201);
	m_Moves.AddFrame(2,0,275,0,18,24,3,10,19,201);
	m_Moves.AddFrame(2,0,305,0,18,24,3,10,20,201);
	m_Moves.AddFrame(2,0,335,0,18,24,3,10,20,201);
	m_Moves.AddFrame(2,0,365,0,18,24,3,10,20,201);
	//站立向上
	m_Moves.AddFrame(3,0,5,779,18,25,20,10,19,800);
	//行走向上
	m_Moves.AddFrame(4,0,  95,779,18,25,3,10,19,801);
	m_Moves.AddFrame(4,0,125,779,18,25,3,10,19,801);
	m_Moves.AddFrame(4,0,155,779,18,25,3,10,20,801);
	m_Moves.AddFrame(4,0,185,779,18,25,3,10,19,801);
	m_Moves.AddFrame(4,0,215,779,18,25,3,10,19,801);
	m_Moves.AddFrame(4,0,245,779,18,25,3,10,19,801);
	m_Moves.AddFrame(4,0,275,779,18,25,3,10,19,801);
	m_Moves.AddFrame(4,0,305,779,18,25,3,10,20,801);
	m_Moves.AddFrame(4,0,335,779,18,25,3,10,19,801);
	m_Moves.AddFrame(4,0,365,779,18,25,3,10,19,801);
	//站立向左
	m_Moves.AddFrame(5,0,  5,1531,17,22,20,  8,17,400);
	m_Moves.AddFrame(5,0,35,1531,17,22,20,  8,17,400);
	m_Moves.AddFrame(5,0,65,1531,17,22,20,  8,17,400);
	//行走向左
	m_Moves.AddFrame(6,0,  92,1531,23,22,3,  8,17,401);
	m_Moves.AddFrame(6,0,124,1531,20,22,3,  8,17,401);
	m_Moves.AddFrame(6,0,154,1531,20,22,3,10,19,401);
	m_Moves.AddFrame(6,0,184,1531,20,23,3,  9,19,401);
	m_Moves.AddFrame(6,0,214,1531,20,23,3,  8,18,401);
	m_Moves.AddFrame(6,0,242,1531,23,22,3,  8,17,401);
	m_Moves.AddFrame(6,0,274,1531,19,22,3,  7,17,401);
	m_Moves.AddFrame(6,0,304,1531,19,22,3,  9,19,401);
	m_Moves.AddFrame(6,0,334,1531,19,22,3,  8,19,401);
	m_Moves.AddFrame(6,0,364,1531,19,22,3,  8,18,401);
	//向下攻击
	m_Moves.AddFrame(7,0,    1,1,80,80,2,40,40,202);
	m_Moves.AddActRect(7,0,-19,-1,-5,5);
	m_Moves.AddFrame(7,0,  81,1,80,80,1,40,40,202);
	m_Moves.AddActRect(7,1,-16,2,-2,10);
	m_Moves.AddFrame(7,0,161,1,80,80,2,40,40,202);
	m_Moves.AddActRect(7,2,-16,4,-2,14);
	m_Moves.AddFrame(7,0,241,1,80,80,2,40,40,202);
	m_Moves.AddActRect(7,3,-6,7,0,21);
	m_Moves.AddFrame(7,0,321,1,80,80,3,40,40,202);
	m_Moves.AddActRect(7,4,0,9,5,25);
	m_Moves.AddFrame(7,0,401,1,80,80,2,40,40,202);
	m_Moves.AddActRect(7,5,5,6,12,20);
	m_Moves.AddFrame(7,0,481,1,80,80,2,40,40,202);
	m_Moves.AddActRect(7,6,6,2,19,15);
	m_Moves.AddFrame(7,0,561,1,80,80,1,40,40,202);
	m_Moves.AddActRect(7,7,7,-2,21,6);
	m_Moves.AddFrame(7,0,641,1,80,80,1,40,40,202);
	m_Moves.AddActRect(7,8,8,-4,23,1);
	//向上攻击
	m_Moves.AddFrame(8,0,    1,1,80,80,1,40,40,802);
	m_Moves.AddActRect(8,0,6,-6,19,12);
	m_Moves.AddFrame(8,0,  81,1,80,80,2,40,40,802);
	m_Moves.AddActRect(8,1,0,-10,17,-4);
	m_Moves.AddFrame(8,0,161,1,80,80,2,40,40,802);
	m_Moves.AddActRect(8,2,-2,-16,15,-6);
	m_Moves.AddFrame(8,0,241,1,80,80,1,40 ,40,802);
	m_Moves.AddActRect(8,3,-1,-25,6,-7);
	m_Moves.AddFrame(8,0,321,1,80,80,4,40,40,802);
	m_Moves.AddActRect(8,4,-6,-30,-1,-13);
	m_Moves.AddFrame(8,0,401,1,80,80,2,40,40,802);
	m_Moves.AddActRect(8,5,-4,-12,4,3);
	m_Moves.AddFrame(8,0,481,1,80,80,2,40,40,802);
	m_Moves.AddActRect(8,6,-20,-16,-6,-7);
	m_Moves.AddFrame(8,0,561,1,80,80,1,40,40,802);
	m_Moves.AddActRect(8,7,-22,-8,-8,-1);
	m_Moves.AddFrame(8,0,641,1,80,80,1,40,40,802);
	m_Moves.AddActRect(8,8,-22,-4,-7,0);
	//向左攻击
	m_Moves.AddFrame(9,0,    1,1,80,80,3,40,40,402);
	m_Moves.AddActRect(9,0,0,-24,5,-8);
	m_Moves.AddFrame(9,0,  81,1,80,80,2,40,40,402);
	m_Moves.AddActRect(9,1,-13,-18,-4,-5);
	m_Moves.AddFrame(9,0,161,1,80,80,3,40,40,402);
	m_Moves.AddActRect(9,2,-23,-7,-14,-4);
	m_Moves.AddFrame(9,0,241,1,80,80,2,40,40,402);
	m_Moves.AddActRect(9,3,-26,-3,-10,3);
	m_Moves.AddFrame(9,0,321,1,80,80,2,40,40,402);
	m_Moves.AddActRect(9,4,-21,2,-6,9);
	m_Moves.AddFrame(9,0,401,1,80,80,2,40,40,402);
	m_Moves.AddActRect(9,5,-9,1,0,15);
	m_Moves.AddFrame(9,0,481,1,80,80,1,40,40,402);
	m_Moves.AddActRect(9,6,-6,3,0,17);
	m_Moves.AddFrame(9,0,561,1,80,80,1,40,40,402);
	m_Moves.AddActRect(9,7,-2,3,3,18);
	//向下推
	m_Moves.AddFrame(10,0,    1,1,80,80,  3,40,40,203);
	m_Moves.AddFrame(10,0,  81,1,80,80,  8,40,40,203);
	m_Moves.AddFrame(10,0,161,1,80,80,  8,40,40,203);
	m_Moves.AddFrame(10,0,241,1,80,80,16,40,40,203);
	m_Moves.AddFrame(10,0,321,1,80,80,  8,40,40,203);
	m_Moves.AddFrame(10,0,401,1,80,80,  8,40,40,203);
	m_Moves.AddFrame(10,0,481,1,80,80,16,40,40,203);
	//向上推
	m_Moves.AddFrame(11,0,    1,1,80,80,  3,40,40,803);
	m_Moves.AddFrame(11,0,  81,1,80,80,  8,40,40,803);
	m_Moves.AddFrame(11,0,161,1,80,80,  8,40,40,803);
	m_Moves.AddFrame(11,0,241,1,80,80,16,40,40,803);
	m_Moves.AddFrame(11,0,321,1,80,80,  8,40,40,803);
	m_Moves.AddFrame(11,0,401,1,80,80,  8,40,40,803);
	m_Moves.AddFrame(11,0,481,1,80,80,16,40,40,803);
	//向左推
	m_Moves.AddFrame(12,0,    1,1,80,80,  3,40,40,403);
	m_Moves.AddFrame(12,0,  81,1,80,80,  8,40,40,403);
	m_Moves.AddFrame(12,0,161,1,80,80,  8,40,40,403);
	m_Moves.AddFrame(12,0,241,1,80,80,16,40,40,403);
	m_Moves.AddFrame(12,0,321,1,80,80,  8,40,40,403);
	m_Moves.AddFrame(12,0,401,1,80,80,  8,40,40,403);
	//向下举着走13
	m_Moves.AddFrame(13,0,  95,30,18,23,3,10,18,204,-8,-26);
	m_Moves.AddFrame(13,0,125,31,18,22,3,10,19,204,-8,-26);
	m_Moves.AddFrame(13,0,155,30,18,24,3,  9,20,204,-8,-28);
	m_Moves.AddFrame(13,0,185,30,18,24,3,10,20,204,-8,-28);
	m_Moves.AddFrame(13,0,215,30,18,24,3,10,19,204,-8,-27);
	m_Moves.AddFrame(13,0,245,30,18,23,3,10,18,204,-8,-26);
	m_Moves.AddFrame(13,0,275,31,18,22,3,10,19,204,-8,-26);
	m_Moves.AddFrame(13,0,305,30,18,24,3,10,20,204,-8,-28);
	m_Moves.AddFrame(13,0,335,30,18,24,3,10,20,204,-8,-28);
	m_Moves.AddFrame(13,0,365,30,18,24,3,10,19,204,-8,-27);
	//向上举着走14
	m_Moves.AddFrame(14,0,  95,811,18,22,3,10,17,804,-8,-26);
	m_Moves.AddFrame(14,0,125,811,18,22,3,10,17,804,-8,-26);
	m_Moves.AddFrame(14,0,155,810,18,24,3,10,20,804,-8,-28);
	m_Moves.AddFrame(14,0,185,809,18,25,3,10,19,804,-8,-28);
	m_Moves.AddFrame(14,0,215,810,18,24,3,10,21,804,-8,-27);
	m_Moves.AddFrame(14,0,245,811,18,22,3,10,17,804,-8,-26);
	m_Moves.AddFrame(14,0,275,811,18,22,3,  9,17,804,-8,-26);
	m_Moves.AddFrame(14,0,305,810,18,24,3,10,19,804,-8,-28);
	//向左举着走15
	m_Moves.AddFrame(15,0,  92,1561,23,22,3,  8,17,404,-8,-26);
	m_Moves.AddFrame(15,0,124,1561,20,21,3,  8,17,404,-8,-26);
	m_Moves.AddFrame(15,0,154,1561,20,21,3,10,19,404,-8,-28);
	m_Moves.AddFrame(15,0,184,1561,20,22,3,  9,19,404,-8,-28);
	m_Moves.AddFrame(15,0,214,1561,20,22,3,  8,18,404,-8,-27);
	m_Moves.AddFrame(15,0,242,1561,23,22,3,  8,17,404,-8,-26);
	m_Moves.AddFrame(15,0,274,1561,19,22,3,  7,17,404,-8,-26);
	m_Moves.AddFrame(15,0,304,1561,19,22,3,  9,19,404,-8,-28);
	m_Moves.AddFrame(15,0,334,1561,19,22,3,  8,19,404,-8,-28);
	m_Moves.AddFrame(15,0,364,1561,20,22,3,  8,17,404,-8,-27);
	//向下举起
	m_Moves.AddFrame(16,0,  95,62,18,19,6,10,10,205,-8,1);
	m_Moves.AddFrame(16,0,125,62,18,20,6,10,14,205,-8,1);
	m_Moves.AddFrame(16,0,155,62,18,20,6,10,15,205,-8,-15);
	m_Moves.AddFrame(16,0,185,61,18,22,6,10,17,205,-8,-26);
	//向下举着
	m_Moves.AddFrame(17,0,215,60,18,23,3,10,18,206,-8,-27);
	//向上举起
	m_Moves.AddFrame(18,0,  96,842,16,19,6,  9,16,805,-8,-15);
	m_Moves.AddFrame(18,0,126,842,16,20,6,  9,18,805,-8,-15);
	m_Moves.AddFrame(18,0,155,842,18,20,6,10,17,805,-8,-21);
	m_Moves.AddFrame(18,0,185,841,18,21,6,10,17,805,-8,-26);
	//向上举着
	m_Moves.AddFrame(19,0,215,841,18,21,3,10,17,806,-8,-27);
	//向左举起
	m_Moves.AddFrame(20,0,  94,1592,20,19,6,13,13,405,-20,-9);
	m_Moves.AddFrame(20,0,125,1592,17,19,6,11,14,405,-20,-9);
	m_Moves.AddFrame(20,0,156,1592,16,20,6,10,15,405,-18,-15);
	m_Moves.AddFrame(20,0,186,1592,16,21,6,  7,16,405,-8,-26);
	//向左举着
	m_Moves.AddFrame(21,0,215,1591,17,22,3,8,17,406,-8,-27);
	//向下举
	m_Moves.AddFrame(22,0,335,61,18,22,4,10,18,207);
	m_Moves.AddFrame(22,0,305,61,18,21,18,10,17,207);
	//向上举
	m_Moves.AddFrame(23,0,312,901,20,22,4,11,17,807);
	m_Moves.AddFrame(23,0,342,901,18,22,18,10,17,807);
	//向左举
	m_Moves.AddFrame(24,0,    1,  1,80,80,  5, 40, 40,407);
	m_Moves.AddFrame(24,0,  81,  1,80,80,  3, 40, 40,407);
	m_Moves.AddFrame(24,0,161,  1,80,80,13 ,40, 40,407);
	//向下仍
	m_Moves.AddFrame(25,0,245,61,18,22,3,10,17,208);
	m_Moves.AddFrame(25,0,273,61,22,22,4,12,17,208);
	m_Moves.AddFrame(25,0,305,61,18,21,3,10,16,208);
	m_Moves.AddFrame(25,0,335,61,18,22,5,10,17,208);
	//向上仍
	m_Moves.AddFrame(26,0,244,841,20,22,3,11,18,808);
	m_Moves.AddFrame(26,0,274,841,20,21,3,11,17,808);
	m_Moves.AddFrame(26,0,305,841,18,21,3,10,17,808);
	m_Moves.AddFrame(26,0,334,841,20,21,4,11,17,808);
	//向左仍
	m_Moves.AddFrame(27,0,245,1591,18,22,2,  7,17,408);
	m_Moves.AddFrame(27,0,274,1591,19,21,4,  9,16,408);
	m_Moves.AddFrame(27,0,304,1591,20,21,3,10,16,408);
	m_Moves.AddFrame(27,0,335,1591,18,22,4,  8,17,408);
	//向下跳
	m_Moves.AddFrame(28,0,  89,90,30,24,  5,16,19,209);
	m_Moves.AddFrame(28,0,119,90,30,24,  3,16,19,209);
	m_Moves.AddFrame(28,0,149,90,30,24,  3,16,19,209);
	m_Moves.AddFrame(28,0,179,90,30,24,  3,16,19,209);
	m_Moves.AddFrame(28,0,209,90,30,24,  3,16,19,209);
	m_Moves.AddFrame(28,0,239,90,30,24,  3,16,19,209);
	m_Moves.AddFrame(28,0,269,90,30,24,  3,16,19,209);
	m_Moves.AddFrame(28,0,299,90,30,24,10,16,19,209);
	m_Moves.AddFrame(28,0,329,90,30,24,  7,16,19,209);
	//向上跳
	m_Moves.AddFrame(29,0,  89,870,30,24,  5,16,19,809);
	m_Moves.AddFrame(29,0,119,870,30,24,  3,16,19,809);
	m_Moves.AddFrame(29,0,149,870,30,24,  3,16,19,809);
	m_Moves.AddFrame(29,0,179,870,30,24,  3,16,19,809);
	m_Moves.AddFrame(29,0,209,870,30,24,  3,16,19,809);
	m_Moves.AddFrame(29,0,239,870,30,24,  3,16,19,809);
	m_Moves.AddFrame(29,0,269,870,30,24,  3,16,19,809);
	m_Moves.AddFrame(29,0,299,870,30,24,10,16,19,809);
	m_Moves.AddFrame(29,0,329,870,30,24,  7,16,19,809);
	//向左跳
	m_Moves.AddFrame(30,0,  89,1620,30,24,  5,13,19,409);
	m_Moves.AddFrame(30,0,119,1620,30,24,  3,13,19,409);
	m_Moves.AddFrame(30,0,149,1620,30,24,  3,13,19,409);
	m_Moves.AddFrame(30,0,179,1620,30,24,  3,13,19,409);
	m_Moves.AddFrame(30,0,209,1620,30,24,  3,13,19,409);
	m_Moves.AddFrame(30,0,239,1620,30,24,  3,13,19,409);
	m_Moves.AddFrame(30,0,269,1620,30,24,  3,13,19,409);
	m_Moves.AddFrame(30,0,299,1620,30,24,10,13,19,409);
	m_Moves.AddFrame(30,0,329,1620,30,24,  7,13,19,409);
	//向下飞	展翅	在跳跃第8帧的4帧以后才能飞
	m_Moves.AddFrame(31,0,  90,211,18,21,  4,10,17,210);
	m_Moves.AddFrame(31,0,115,211,28,21,  4,14,17,210);
	//向下飞 飞行
	m_Moves.AddFrame(32,0,144,211,32,21,  5,16,19,210);
	m_Moves.AddFrame(32,0,178,211,32,21,  5,16,19,210);
	m_Moves.AddFrame(32,0,211,211,28,21,  5,14,19,210);
	//向上飞 展翅
	m_Moves.AddFrame(33,0,  95,990,18,24,  4,  9,19,810);
	m_Moves.AddFrame(33,0,122,990,24,24,  4,12,19,810);
	//向上飞 飞行
	m_Moves.AddFrame(34,0,148,990,32,24,  5,16,19,810);
	m_Moves.AddFrame(34,0,181,990,30,24,  5,15,19,810);
	m_Moves.AddFrame(34,0,212,990,28,24,  5,14,19,810);
	//向左飞 展翅
	m_Moves.AddFrame(35,0,  95,1742,18,20,  4,10,15,410);
	m_Moves.AddFrame(35,0,123,1741,22,22,  4,10,15,410);
	//向左飞 飞行
	m_Moves.AddFrame(36,0,152,1740,24,20,  5,11,15,410);
	m_Moves.AddFrame(36,0,183,1741,22,18,  5,10,15,410);
	m_Moves.AddFrame(36,0,213,1743,23,16,  5,10,13,410);
	//水中 向下
	m_Moves.AddFrame(37,0,  95,244,18,18,  8,10,12,211);
	m_Moves.AddFrame(37,0,124,244,20,19,  8,11,12,211);
	m_Moves.AddFrame(37,0,153,244,22,19,  8,12,11,211);
	m_Moves.AddFrame(37,0,185,244,18,17,  8,10,12,211);
	//向下游泳
	m_Moves.AddFrame(38,0,211,241,26,19,  8,14,12,212);
	m_Moves.AddFrame(38,0,240,239,28,21,  8,15,14,212);
	m_Moves.AddFrame(38,0,272,243,24,17,  8,13,10,212);
	m_Moves.AddFrame(38,0,302,244,24,17,  8,13,11,212);
	//水中向上
	m_Moves.AddFrame(39,0,  95,1021,18,19,  8,10,12,811);
	m_Moves.AddFrame(39,0,124,1022,20,18,  8,11,11,811);
	m_Moves.AddFrame(39,0,153,1021,22,21,  8,12,12,811);
	m_Moves.AddFrame(39,0,185,1021,18,19,  8,10,12,811);
	//向上游泳
	m_Moves.AddFrame(40,0,213,1021,22,21,8,12,14,812);
	m_Moves.AddFrame(40,0,242,1022,24,21,8,13,13,812);
	m_Moves.AddFrame(40,0,272,1021,24,22,8,13,12,812);
	m_Moves.AddFrame(40,0,300,1021,28,24,8,15,11,812);
	//水中向左
	m_Moves.AddFrame(41,0,  93,1805,21,17,8,10,11,411);
	m_Moves.AddFrame(41,0,120,1805,23,17,8,13,10,411);
	m_Moves.AddFrame(41,0,151,1805,23,20,8,12,11,411);
	m_Moves.AddFrame(41,0,185,1805,18,17,8,  9,11,411);
	//向左游泳
	m_Moves.AddFrame(42,0,214,1805,28,19,8,12,11,412);
	m_Moves.AddFrame(42,0,244,1805,31,18,8,12,10,412);
	m_Moves.AddFrame(42,0,284,1805,34,21,8,11,12,412);
	m_Moves.AddFrame(42,0,325,1805,36,22,8,10,12,412);
	//举起宝石
	m_Moves.AddFrame(43,0,1,1,80,80,36,40,40,213);
	m_Moves.AddFrame(43,0,81,1,80,80,1,40,40,213);
	m_Moves.AddFrame(43,0,161,1,80,80,2,40,40,213);
	m_Moves.AddFrame(43,0,241,1,80,80,2,40,40,213);
	m_Moves.AddFrame(43,0,321,1,80,80,3,40,40,213);
	m_Moves.AddFrame(43,0,401,1,80,80,2,40,40,213);
	m_Moves.AddFrame(43,0,481,1,80,80,2,40,40,213);
	m_Moves.AddFrame(43,0,561,1,80,80,2,40,40,213);
	m_Moves.AddFrame(43,0,641,1,80,80,1,40,40,213);
	m_Moves.AddFrame(43,0,721,1,80,80,3,40,40,213);
	m_Moves.AddFrame(43,0,801,1,80,80,2,40,40,213);
	m_Moves.AddFrame(43,0,881,1,80,80,1,40,40,213);
	m_Moves.AddFrame(43,0,961,1,80,80,3,40,40,213);
	m_Moves.AddFrame(43,0,1041,1,80,80,2,40,40,213);
	m_Moves.AddFrame(43,0,1121,1,80,80,1,40,40,213);
	m_Moves.AddFrame(43,0,1201,1,80,80,3,40,40,213);
	m_Moves.AddFrame(43,0,1281,1,80,80,2,40,40,213);
	m_Moves.AddFrame(43,0,1361,1,80,80,1,40,40,213);
	m_Moves.AddFrame(43,0,1441,1,80,80,1,40,40,213);
	m_Moves.AddFrame(43,0,1521,1,80,80,58,40,40,213);
	m_Moves.AddFrame(43,0,1601,1,80,80,1,40,40,213);
	//猪爬行 向下
	m_Moves.AddFrame(44,0,11,7,16,15,16,8,14,200);
	m_Moves.AddFrame(44,0,53,6,16,15,16,8,14,200);
	//猪爬行 向上
	m_Moves.AddFrame(45,0,6,7,16,14,16,8,14,800);
	m_Moves.AddFrame(45,0,31,7,16,14,16,8,14,800);
	//猪爬行 向左
	m_Moves.AddFrame(46,0,3,3,14,15,16,9,13,400);
	m_Moves.AddFrame(46,0,27,3,16,15,16,10,12,400);
	m_Moves.AddFrame(46,0,49,3,16,14,16,11,13,400);
	//猪向下 不动
	m_Moves.AddFrame(47,0,62,26,16,15,5,8,15,201);
	//猪向上 不动
	m_Moves.AddFrame(48,0,52,6,16,15,5,8,14,801);
	//猪向左不动
	m_Moves.AddFrame(49,0,27,3,16,15,5,10,12,401);
	//吐子弹向下
	m_Moves.AddFrame(50,0,10,5,16,19,16,8,14,202);
	m_Moves.AddFrame(50,0,34,5,16,19,4,8, 14,202);
	//吐子弹向上
	m_Moves.AddFrame(51,0,4,9,16,17,16,8,17,802);
	m_Moves.AddFrame(51,0,31,9,16,17,4,8,17,802);
	//吐子弹向左
	m_Moves.AddFrame(52,0,8,2,16,15,16,8,15,402);
	m_Moves.AddFrame(52,0,33,2,19,15,4,13,15,402);
	//猪子弹
	m_Moves.AddFrame(53,0,0,98,8,7,6,4,4);
	m_Moves.AddFrame(53,0,8,98,8,8,6,4,4);
	m_Moves.AddFrame(53,0,16,98,8,7,6,4,4);
	//受伤向下
	m_Moves.AddFrame(54,0,0,222,18,24,4,10,18,214);
	m_Moves.AddFrame(54,0,5,0,18,24,4,10,18,214);
	//受伤向上
	m_Moves.AddFrame(55,0,5,779,18,25,4,10,19,814);
	m_Moves.AddFrame(55,0,20,222,18,25,4,10,19,814);
	//受伤向左
	m_Moves.AddFrame(56,0,  5,1531,17,22,4,  8,17,414);
	m_Moves.AddFrame(56,0,41,  223,17,22,4,  8,17,414);
	//门左打开
	m_Moves.AddFrame(57,0,66,109,16,48,13,  0,16);
	//门右打开
	m_Moves.AddFrame(58,0,82,109,16,48,13,  0,16);
	//坠落
	m_Moves.AddFrame(59,0,1,1,80,80,8,  40,40,215);
	m_Moves.AddFrame(59,0,81,1,80,80,8,  40,40,215);
	m_Moves.AddFrame(59,0,161,1,80,80,8,  40,40,215);
	m_Moves.AddFrame(59,0,241,1,80,80,8,  40,40,215);
	m_Moves.AddFrame(59,0,321,1,80,80,8,  40,40,215);
	m_Moves.AddFrame(59,0,401,1,80,80,8,  40,40,215);
	//草碎掉
	m_Moves.AddFrame(60,0,0,118,26,21,7,0,21);
	m_Moves.AddFrame(60,0,0,139,26,21,6,0,21);
	m_Moves.AddFrame(60,0,0,160,26,21,4,0,21);
	m_Moves.AddFrame(60,0,0,181,26,21,3,0,21);
	//草地动态效果
	m_Moves.AddFrame(61,0,  1,1,18,16,1,9,8);
	m_Moves.AddFrame(61,0,19,1,18,16,1,9,8);
	m_Moves.AddFrame(61,0,37,1,18,16,1,9,8);
	m_Moves.AddFrame(61,0,55,1,18,16,1,9,8);
	//怪物死亡特效
	m_Moves.AddFrame(62,0,1,1,36,36,3,18,18);
	m_Moves.AddFrame(62,0,37,1,36,36,3,18,18);
	m_Moves.AddFrame(62,0,73,1,36,36,3,18,18);
	m_Moves.AddFrame(62,0,109,1,36,36,3,18,18);
	m_Moves.AddFrame(62,0,145,1,36,36,3,18,18);
	m_Moves.AddFrame(62,0,181,1,36,36,3,18,18);
	m_Moves.AddFrame(62,0,217,1,36,36,3,18,18);
	//火焰
	m_Moves.AddFrame(63,0,1,1,16,16,10,0,0);
	m_Moves.AddFrame(63,0,17,1,16,16,10,0,0);
	m_Moves.AddFrame(63,0,33,1,16,16,10,0,0);
	m_Moves.AddFrame(63,0,49,1,16,16,10,0,0);
	//倒地
	m_Moves.AddFrame(64,0,1,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,81,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,161,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,241,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,321,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,401,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,481,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,561,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,641,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,721,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,801,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,881,1,80,80,4,40,40,216);
	m_Moves.AddFrame(64,0,961,1,80,80,900,40,40,216);
	//水纹
	m_Moves.AddFrame(65,0,1,1,18,10,8,-9,0);
	m_Moves.AddFrame(65,0,19,1,18,10,8,-9,0);
	m_Moves.AddFrame(65,0,37,1,18,10,8,-9,0);
	m_Moves.AddFrame(65,0,55,1,18,10,8,-9,0);
	m_Moves.AddFrame(65,0,73,1,18,10,8,-9,0);
	//水花
	m_Moves.AddFrame(66,0,1,1,42,25,6,25,10);
	m_Moves.AddFrame(66,0,42,1,42,25,6,25,10);
	m_Moves.AddFrame(66,0,85,1,42,25,6,25,10);
	m_Moves.AddFrame(66,0,127,1,42,25,6,25,10);
	m_Moves.AddFrame(66,0,169,1,42,25,6,25,10);
	//河流
	m_Moves.AddFrame(67,0,0,0,1000,148,12,0,0);
	m_Moves.AddFrame(67,0,0,0,1000,148,12,0,0);
	m_Moves.AddFrame(67,0,0,0,1000,148,12,0,0);
	m_Moves.AddFrame(67,0,0,0,1000,148,12,0,0);
	//设置动作连接
	m_Moves.SetNextMove(1,1);
	m_Moves.SetNextMove(2,2);
	m_Moves.SetNextMove(3,3);
	m_Moves.SetNextMove(4,4);
	m_Moves.SetNextMove(5,5);
	m_Moves.SetNextMove(6,6);	
	m_Moves.SetNextMove(7,1);	
	m_Moves.SetNextMove(8,3);	
	m_Moves.SetNextMove(9,5);	
	m_Moves.SetNextMove(10,-1);	
	m_Moves.SetNextMove(11,-1);	
	m_Moves.SetNextMove(12,12);	
	m_Moves.SetNextMove(13,13);
	m_Moves.SetNextMove(14,14);
	m_Moves.SetNextMove(15,15);
	m_Moves.SetNextMove(17,17);
	m_Moves.SetNextMove(19,19);
	m_Moves.SetNextMove(21,21);
	m_Moves.SetNextMove(16,17);
	m_Moves.SetNextMove(18,19);
	m_Moves.SetNextMove(20,21);
	m_Moves.SetNextMove(22,1);
	m_Moves.SetNextMove(23,3);
	m_Moves.SetNextMove(24,5);
	m_Moves.SetNextMove(25,1);
	m_Moves.SetNextMove(26,3);
	m_Moves.SetNextMove(27,5);
	m_Moves.SetNextMove(28,1);
	m_Moves.SetNextMove(29,3);
	m_Moves.SetNextMove(30,5);
	m_Moves.SetNextMove(31,32);
	m_Moves.SetNextMove(32,32);
	m_Moves.SetNextMove(33,34);
	m_Moves.SetNextMove(34,34);
	m_Moves.SetNextMove(35,36);
	m_Moves.SetNextMove(36,36);
	m_Moves.SetNextMove(37,37);
	m_Moves.SetNextMove(38,38);
	m_Moves.SetNextMove(39,39);
	m_Moves.SetNextMove(40,40);
	m_Moves.SetNextMove(41,41);
	m_Moves.SetNextMove(42,42);
	m_Moves.SetNextMove(43,3);
	m_Moves.SetNextMove(44,44);
	m_Moves.SetNextMove(45,45);
	m_Moves.SetNextMove(46,46);
	m_Moves.SetNextMove(47,47);
	m_Moves.SetNextMove(48,48);
	m_Moves.SetNextMove(49,49);
	m_Moves.SetNextMove(50,47);
	m_Moves.SetNextMove(51,48);
	m_Moves.SetNextMove(52,49);
	m_Moves.SetNextMove(54,54);
	m_Moves.SetNextMove(55,55);
	m_Moves.SetNextMove(56,56);
	m_Moves.SetNextMove(60,60);
	m_Moves.SetNextMove(64,1);
	m_Moves.SetNextMove(65,65);
	m_Moves.SetNextMove(66,66);
	m_Moves.SetNextMove(67,67);
}

CMoves* CFirst::GetMoves()
{
	return &m_Moves;
}
CGoods* CFirst::GetGoods()
{
	return &m_Goods;
}
void CFirst::Complete(bool wintag)
{
		m_gameState=wintag?1:2;
		m_count=0;
}
CGoodManager * CFirst::GetGoodManager()
{
	return &m_GoodManager;
}
bool CFirst::SubMoney(int n)
{
	if(m_Money>=n)
	{
		m_Money--;
		return true;
	}
	return false;
}
std::list<CRole*>* CFirst::GetMonster()
{
	return &m_Monster;
}
CHero * CFirst::GetHero()
{
	return m_p;
}