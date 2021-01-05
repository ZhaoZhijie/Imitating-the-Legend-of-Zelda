#include <iostream>
#include "TcpServer.h"
#include "GameProtocol.h"
#include "Queue.h"
#include "Map.h"
#include <list>
#include <string>
#include "Game.h"
#include "First.h"
#include "Role.h"
#define SLEEPTIME 30
#define ROOM_STATE_EMPTY  0   //空房间 ，在讲房间数据发到客户端时空房间不用发
#define ROOM_STATE_WAIT  1		//玩家在房间等待，还未进入游戏
#define ROOM_STATE_RUN  2		//已经进入游戏
#define PLAYER_STATE_HALL 0		//在大厅
#define PLAYER_STATE_UNPRE 1		//在房间 未准备
#define PLAYER_STATE_PRE 2			//在房间准备
#define PLAYER_STATE_ADMIN 3			//在房间房主
#define PLAYER_STATE_LOAD 4		//加载游戏中
#define PLAYER_STATE_WAIT 5		//加载完毕等待游戏开始
#define PLAYER_STATE_GAME 6		//已经进入游戏
#define _MAP_BOOT 1
#define _MAP_BAR 2
#define _MAP_FLY 3
#define _MAP_RAND 4
struct PlayerData
{
	char _name[20];//玩家名字
	int _state;//玩家状态
	int _order;//座号
	int _no;//房间号
};
struct RoomData
{
	int _num;//人数
	int _tag;//开始标志
	int _type;//地图类型
	std::string _name;//房间名称
	SOCKET _psocket[2];
	CGame * _game;
};

CMemoryPool g_Pool;
CTcpServer g_Server;
std::list<SOCKET> g_LoginClient;//刚连接到服务器的客户端
CMap<SOCKET,PlayerData> g_HallPlayer;//进入大厅的玩家，包括在房间里的玩家
RoomData g_Room[20];//最多20个房间,数组下标即房间编号,当玩家新建房间时，在空房间中选一个
void main()
{
	CFirst::InitData();
	g_Server.Init(8888, 30, 5, 2);
	while(1)
	{
		int t=GetTickCount();
		g_Server.Run();
		_TS_NET_EVENT evt;
		while(g_Server.GetTSNetEvent(&evt))
		{
			switch(evt.type)
			{
			case _TSNE_CONNECT:
				{
					//将客户端添加到登陆队列
					g_LoginClient.push_back(evt.s);
					std::cout<<evt.s<<"有客户端连接\n";
					break;
				}
			case _TSNE_S_DISCONNECT:
			case _TSNE_C_DISCONNECT:
			case _TSNE_E_DISCONNECT:
				{
					//将客户端剔除
					for(std::list<SOCKET>::iterator i=g_LoginClient.begin();i!=g_LoginClient.end();i++)
					{
						if(*i==evt.s)
						{
							g_LoginClient.erase(i);
							break;
						}
					}	
					int len=g_HallPlayer.Length();
					for(int i=0;i<len;i++)
					{
						if(g_HallPlayer.At(i)->key==evt.s)
							g_HallPlayer.Erase(i);
							break;
					}
					std::cout<<evt.s<<"与客户端断开连接\n";
					break;
				}
			case _TSNE_DATA:
				{
					std::cout<<"收到数据\n";
					_GAME_PROTOCOL *gp=(_GAME_PROTOCOL *)evt.data;
					switch(gp->id)
					{
					case _TRY_PASS:
						{
							_C_S_GP2 *gp2=(_C_S_GP2 *)evt.data;
							FILE * fp=fopen("pass.txt","r");
							_S_C_GP3 gp3;
							gp3.result=_TRY_NO;
							if(!fp)
								gp3.result=_TRY_ERROR;
							else
							{
								char name[20];
								char pass[20];
								while(!feof(fp))
								{
									fgets(name,20,fp);
									fgets(pass,20,fp);
									name[strlen(name)-1]='\0';
									pass[strlen(pass)-1]='\0';
									if(strcmp(name,gp2->uname)==0&&strcmp(pass,gp2->upass)==0)
									{
										//判断是否已经登录 g_LoginClient
										//CMap<SOCKET,PlayerData> g_HallPlayer
										int len=g_HallPlayer.Length();
										gp3.result=_TRY_YES;
										for(int i=0;i<len;i++)
										{
											if(strcmp(g_HallPlayer.At(i)->data._name,gp2->uname)==0)
											{
												gp3.result=_TRY_ALREADY;
												break;
											}
										}	
										break;
									}
								}
								fclose(fp);
							}
							g_Server.SendData(evt.s,(const char*)&gp3,sizeof(gp3));
							if(gp3.result==_TRY_YES)
							{//将客户端放入玩家链表,并从客户端登陆链表删除
								PlayerData pdata;
								strcpy(pdata._name,gp2->uname);
								pdata._state=PLAYER_STATE_HALL;
								g_HallPlayer.Insert(evt.s, pdata);
								for(std::list<SOCKET>::iterator i=g_LoginClient.begin();i!=g_LoginClient.end();i++)
								{
									if(*i==evt.s)
									{
										g_LoginClient.erase(i);
										break;
									}
								}
							}
							break;
						}
					case _ENTER_HALL:
						{
							//将大厅的房间数据传输过去
							for(int i=0;i<20;i++)
							{
								if(g_Room[i]._tag!=ROOM_STATE_EMPTY)
								{
									_S_C_GP4 gp4;
									strcpy(gp4.name,g_Room[i]._name.c_str());
									gp4.no=i;
									gp4.num=g_Room[i]._num;
									gp4.tag=g_Room[i]._tag;
									gp4.type=g_Room[i]._type;
									g_Server.SendData(evt.s,(const char*)&gp4,sizeof(gp4));
								}
							}
							//如果是从登陆界面进来将玩家资料传过去
							if(g_HallPlayer.Find(evt.s)->data._state==PLAYER_STATE_HALL)
							{
								_S_C_GP11 gp11;
								strcpy(gp11._name,g_HallPlayer.Find(evt.s)->data._name);
								g_Server.SendData(evt.s,(const char*)&gp11,sizeof(gp11));
							}
							break;
						}
					case _CREATE_ROOM:
						{
							_C_S_GP6 *gp6=(_C_S_GP6 *)evt.data;
							int i;
							bool flag=false;
							for(i=0;i<20;i++)
							{
								if(g_Room[i]._tag==ROOM_STATE_EMPTY)
								{
									 g_Room[i]._tag=ROOM_STATE_WAIT;
									 g_Room[i]._name=gp6->name;
									 g_Room[i]._num=1;
									 g_Room[i]._type=rand()%4+1;
									 g_Room[i]._psocket[0]=evt.s;
									 flag=true;
									break;
								}
							}
							_S_C_GP7 gp7;
							gp7.result=false;
							if(flag)
							{
								//修改该玩家的状态 为房主
								g_HallPlayer.Find(evt.s)->data._state=PLAYER_STATE_ADMIN;
								g_HallPlayer.Find(evt.s)->data._order=0;
								g_HallPlayer.Find(evt.s)->data._no=i;
								//将创建的房间信息发给该玩家
								_S_C_GP12 gp12;
								strcpy(gp12._name,g_Room[i]._name.c_str());
								gp12._no=i;
								gp12._type=g_Room[i]._type;
								g_Server.SendData(evt.s,(const char*)&gp12,sizeof(gp12));
								//给大厅的每个玩家发送房间更新信息
								_S_C_GP4 gp4;
								strcpy(gp4.name,g_Room[i]._name.c_str());
								gp4.no=i;
								gp4.num=g_Room[i]._num;
								gp4.tag=g_Room[i]._tag;
								gp4.type=g_Room[i]._type;
								int len=g_HallPlayer.Length();
								for(int j=0;j<len;j++)
								{
									if(g_HallPlayer.At(j)->data._state==PLAYER_STATE_HALL)
										g_Server.SendData(g_HallPlayer.At(j)->key,(const char*)&gp4,sizeof(gp4));
								}
							}
							g_Server.SendData(evt.s,(const char*)&gp7,sizeof(gp7));
							break;
						}
					case _ENTER_ROOM:
						{
							_S_C_GP10 gp10;//告知该玩家是否成功进入房间
							_C_S_GP8 *gp8=(_C_S_GP8 *)evt.data;
							int no=gp8->no;
							gp10.result=g_Room[no]._num<2?true:false;
							if(g_Room[no]._num<2)
							{
								g_Room[no]._num++;//房间人数加1
								g_HallPlayer.Find(evt.s)->data._state=PLAYER_STATE_UNPRE;
								g_HallPlayer.Find(evt.s)->data._no=no;
								//给刚加入玩家发送房主信息
								_S_C_GP9 gp9;
								gp9._order=g_Room[no]._psocket[0]?0:1;
								SOCKET s=g_Room[no]._psocket[gp9._order];
								strcpy(gp9._name,g_HallPlayer.Find(s)->data._name);
								gp9._state=PLAYER_STATE_ADMIN;
								g_Server.SendData(evt.s,(const char*)&gp9,sizeof(gp9));
								if(g_Room[no]._psocket[0])
								{
									g_HallPlayer.Find(evt.s)->data._order=1;
									g_Room[no]._psocket[1]=evt.s;
								}
								else
								{
									g_HallPlayer.Find(evt.s)->data._order=0;
									g_Room[no]._psocket[0]=evt.s;
								}
								//给刚加入玩家发送房间信息
								_S_C_GP12 gp12;
								strcpy(gp12._name,g_Room[no]._name.c_str());
								gp12._no=no;
								gp12._type=g_Room[no]._type;
								g_Server.SendData(evt.s,(const char*)&gp12,sizeof(gp12));
								//给房主发送刚加入玩家信息
								gp9._order=gp9._order?0:1;
								strcpy(gp9._name,g_HallPlayer.Find(evt.s)->data._name);
								gp9._state=PLAYER_STATE_UNPRE;
								SOCKET adminsocket=g_Room[no]._psocket[(gp9._order+1)%2];
								g_Server.SendData(adminsocket,(const char*)&gp9,sizeof(gp9));
							 }
							break;
						}
					case _PRE_CLICK:
						{//玩家点击了准备
							_C_S_GP13 *gp13=(_C_S_GP13 *)evt.data;
							_S_C_GP14 gp14;
							strcpy(gp14._name,g_HallPlayer.Find(evt.s)->data._name);
							gp14._state=g_HallPlayer.Find(evt.s)->data._state==PLAYER_STATE_UNPRE?PLAYER_STATE_PRE:PLAYER_STATE_UNPRE;
							g_HallPlayer.Find(evt.s)->data._state=gp14._state;
							//给该房间的两个玩家发送状态变更信息
							for(int i=0;i<2;i++)
							{
								SOCKET s=g_Room[gp13->_no]._psocket[i];
								if(s!=0)
									g_Server.SendData(s,(const char*)&gp14,sizeof(gp14));	
							}
							break;
						}
					case _I_QUIT:
						{
							_C_S_GP15 *gp15=(_C_S_GP15 *)evt.data;
							g_Room[gp15->_no]._num--;
							g_Room[gp15->_no]._tag=g_Room[gp15->_no]._num==0?ROOM_STATE_EMPTY:ROOM_STATE_WAIT;
							int order=g_HallPlayer.Find(evt.s)->data._order;
							g_Room[gp15->_no]._psocket[order]=0;
							g_HallPlayer.Find(evt.s)->data._state=PLAYER_STATE_HALL;
							g_HallPlayer.Find(evt.s)->data._no=-1;
							//如果还有一个玩家且不是房主则升为房主
							if(g_Room[gp15->_no]._num==1)
							{
								_S_C_GP14 gp14;
								SOCKET s=g_Room[gp15->_no]._psocket[(order+1)%2];
								g_HallPlayer.Find(s)->data._state=PLAYER_STATE_ADMIN;
								gp14._state=PLAYER_STATE_ADMIN;
								strcpy(gp14._name,g_HallPlayer.Find(s)->data._name);
								g_Server.SendData(s,(const char*)&gp14,sizeof(gp14));
								_S_C_GP16 gp16;
								g_Server.SendData(s,(const char*)&gp16,sizeof(gp16));
							}
							else if(g_Room[gp15->_no]._num==0)
							{
								//通知大厅玩家某房间删除
								_S_C_GP17 gp17;
								gp17.no=gp15->_no;
								int len=g_HallPlayer.Length();
								for(int j=0;j<len;j++)
								{
									if(g_HallPlayer.At(j)->data._state==PLAYER_STATE_HALL)
										g_Server.SendData(g_HallPlayer.At(j)->key,(const char*)&gp17,sizeof(gp17));
								}
							}
							break;
						}
					case _CLICK_START:
						{
							_C_S_GP20 *gp20=(_C_S_GP20 *)evt.data;
							int order=g_HallPlayer.Find(evt.s)->data._order;
							SOCKET s[2]={g_Room[gp20->_no]._psocket[0], g_Room[gp20->_no]._psocket[1]};
							if(g_Room[gp20->_no]._num==2&&g_HallPlayer.Find(s[(order+1)%2])->data._state==PLAYER_STATE_PRE)
							{
								g_HallPlayer.Find(s[0])->data._state=PLAYER_STATE_LOAD;
								g_HallPlayer.Find(s[1])->data._state=PLAYER_STATE_LOAD;
								g_Room[gp20->_no]._game=new CFirst(g_HallPlayer.Find(s[0])->data._name,g_HallPlayer.Find(s[1])->data._name);
								_S_C_GP21 gp21;
								g_Server.SendData(s[0],(const char*)&gp21,sizeof(gp21));
								g_Server.SendData(s[1],(const char*)&gp21,sizeof(gp21));
							}
							break;
						}
					case _GAME_WAIT:
						{
							int order=g_HallPlayer.Find(evt.s)->data._order;
							int no=g_HallPlayer.Find(evt.s)->data._no;
							SOCKET s[2]={g_Room[no]._psocket[0], g_Room[no]._psocket[1]};
							g_HallPlayer.Find(s[(order+1)%2])->data._state=PLAYER_STATE_WAIT;
							if(g_HallPlayer.Find(s[0])->data._state=PLAYER_STATE_WAIT)
							{
								g_HallPlayer.Find(s[0])->data._state=PLAYER_STATE_GAME;
								g_HallPlayer.Find(s[1])->data._state=PLAYER_STATE_GAME;
								_S_C_GP23 gp23;
								g_Server.SendData(s[0],(const char*)&gp23,sizeof(gp23));
								g_Server.SendData(s[1],(const char*)&gp23,sizeof(gp23));
								g_Room[no]._tag=ROOM_STATE_RUN;
							}
							break;
						}
					case _PLAYER_INPUT:
						{
							_C_S_GP18* gp18=(_C_S_GP18*)evt.data;
							int order=g_HallPlayer.Find(evt.s)->data._order;
							int no=g_HallPlayer.Find(evt.s)->data._no;
							g_Room[no]._game->Input(order, gp18->type, gp18->p1, gp18->p2);
							break;
						}
					}
					g_Server.ReleaseTSNetEvent(&evt);
					break;
				}
			}
		}
		//向房间已进入游戏的玩家发送画面数据
		for(int i=0;i<20;i++)
		{
			if(g_Room[i]._tag==ROOM_STATE_RUN)
			{
				for(int j=0;j<2;j++)
				{
					char * imgdata;
					int len,virx,viry;
					g_Room[i]._game->Output(j,&imgdata,&len,&virx,&viry);
					g_Room[i]._game->Run();
					_S_C_GP19 gp19;
					gp19._len=len;
					gp19._virx=virx;
					gp19._viry=viry;
					memcpy(imgdata,&gp19,sizeof(gp19));
					g_Server.SendData(g_Room[i]._psocket[j], imgdata, len);
					delete []imgdata;
				}
			}
		}

		t=GetTickCount()-t;
		if(t<SLEEPTIME)
			Sleep(SLEEPTIME-t);
		else
			Sleep(1);
	}
}
