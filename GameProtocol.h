#ifndef _GAME_PROTOCOL_H_
#define _GAME_PROTOCOL_H_

#define _TRY_NICK_NAME 1		//(客户端->服务器)检查昵称
#define _TRY_PASS 2//密码验证请求
#define _TRY_RESULT 3//密码验证结果
#define _ROOM_DATA 4//大厅的房间数据
#define _ENTER_HALL 5 //进入大厅 请求大厅数据
#define _CREATE_ROOM 6//创建房间
#define  _CREATE_RESULT 7 //房间创建结果
#define  _ENTER_ROOM 8//进入房间申请
#define  _PLAYER_DATA 9//玩家数据
#define  _EROOM_RESULT 10//加入房间结果
#define  _OWNER_DATA 11//当前玩家数据
#define  _CUR_ROOM 12 //当前房间数据
#define  _PRE_CLICK 13//玩家点击准备按钮
#define  _PRE_CLICK_RESULT 14//准备点击处理结果
#define  _I_QUIT 15//退出房间
#define  _H_QUIT 16//另一玩家退出房间
#define  _ROOM_DEL 17 //房间删除
#define  _PLAYER_INPUT 18 //玩家输入数据
#define _PLAYER_OUTPUT 19//玩家输出数据
#define _CLICK_START 20 //点击开始
#define  _ENTER_GAME 21//告知玩家可以进入游戏场景
#define  _GAME_WAIT 22//告知服务器已进入场景等待其他玩家中
#define  _GAME_START 23//告知玩家其他玩家都已就绪可以开始接收游戏画面和传递控制信息
struct _GAME_PROTOCOL
{
	//协议ID
	int id;
};

//检查昵称是否已经存在
struct _C_S_GP1 : public _GAME_PROTOCOL
{
	_C_S_GP1()
	{
		id = _TRY_NICK_NAME;
	}
	char nick_name[32];
};
//密码验证请求
struct _C_S_GP2 : public _GAME_PROTOCOL
{
	_C_S_GP2()
	{
		id = _TRY_PASS;
	}
	char uname[20];
	char upass[20];
};
//密码验证结果
#define _TRY_YES 0
#define _TRY_NO 1
#define _TRY_ERROR 2
#define _TRY_ALREADY 3
struct _S_C_GP3 : public _GAME_PROTOCOL
{
	_S_C_GP3()
	{
		id = _TRY_RESULT;
	}
	int result;
};
//大厅房间数据
struct _S_C_GP4 : public _GAME_PROTOCOL
{
	_S_C_GP4()
	{
		id = _ROOM_DATA;
	}
	int no;//房间编号
	char name[20];//房间名称
	int num;//房间人数
	int tag;//房间状态
	int type;//地图类型
};
//进入大厅场景说明
struct _C_S_GP5 : public _GAME_PROTOCOL
{
	_C_S_GP5()
	{
		id = _ENTER_HALL;
	}
};
//创建房间
struct _C_S_GP6 : public _GAME_PROTOCOL
{
	_C_S_GP6()
	{
		id = _CREATE_ROOM;
	}
	char name[20];//房间名
};
//房间创建结果
struct _S_C_GP7 : public _GAME_PROTOCOL
{
	_S_C_GP7()
	{
		id = _CREATE_RESULT;
	}
	bool result;
};
//加入房间申请
struct _C_S_GP8 : public _GAME_PROTOCOL
{
	_C_S_GP8()
	{
		id = _ENTER_ROOM;
	}
	int no;//房间编号
};
//玩家数据
struct _S_C_GP9 : public _GAME_PROTOCOL
{
	_S_C_GP9()
	{
		id = _PLAYER_DATA;
	}
	char _name[20];//玩家名字
	int _state;//玩家状态
	int _order;//左边还是右边 0是左边 1是右边
};
struct _S_C_GP10 : public _GAME_PROTOCOL
{
	_S_C_GP10()
	{
		id = _EROOM_RESULT;
	}
	bool result;
};
//当前玩家数据
struct _S_C_GP11 : public _GAME_PROTOCOL
{
	_S_C_GP11()
	{
		id = _OWNER_DATA;
	}
	char _name[20];//玩家名字
};
//当前房间数据
struct _S_C_GP12 : public _GAME_PROTOCOL
{
	_S_C_GP12()
	{
		id = _CUR_ROOM;
	}
	int _no;//房间编号
	char _name[20];//房间名字
	int _type;
};
//玩家点击准备按钮
struct _C_S_GP13 : public _GAME_PROTOCOL
{
	_C_S_GP13()
	{
		id = _PRE_CLICK;
	}
	int _no;//房间编号
};
//玩家准备点击处理结果
struct _S_C_GP14 : public _GAME_PROTOCOL
{
	_S_C_GP14()
	{
		id = _PRE_CLICK_RESULT;
	}
	char _name[20];//玩家名字
	int _state;//处理后状态
};
//告知服务器退出房间
struct _C_S_GP15 : public _GAME_PROTOCOL
{
	_C_S_GP15()
	{
		id = _I_QUIT;
	}
	int _no;
};
//告知玩家该房间另一个玩家退出房间
struct _S_C_GP16 : public _GAME_PROTOCOL
{
	_S_C_GP16()
	{
		id = _H_QUIT;
	}
};
//房间删除
struct _S_C_GP17 : public _GAME_PROTOCOL
{
	_S_C_GP17()
	{
		id = _ROOM_DEL;
	}
	int no;
};
//玩家输入信息
#define _INTYPE_KEY 0
#define _INTYPE_MOUSE_CLICK 1
struct _C_S_GP18 : public _GAME_PROTOCOL
{
	_C_S_GP18()
	{
		id = _PLAYER_INPUT;
	}
	//房间编号 玩家id 输入类型 p1,p1具体信息
	int type; int p1; int p2;
};
//玩家画面数据
struct _S_C_GP19 : public _GAME_PROTOCOL
{
	_S_C_GP19()
	{
		id = _PLAYER_OUTPUT;
	}
	//id 输入类型 p1,p1具体信息
	int _len;
	unsigned short _virx;
	unsigned short _viry;
};
//房主点击开始
struct _C_S_GP20 : public _GAME_PROTOCOL
{
	_C_S_GP20()
	{
		id = _CLICK_START;
	}
	int _no;
};
//告知玩家可以进入游戏场景
struct _S_C_GP21 : public _GAME_PROTOCOL
{
	_S_C_GP21()
	{
		id = _ENTER_GAME;
	}
};
//告知服务器已进入游戏场景 等待其他玩家
struct _C_S_GP22 : public _GAME_PROTOCOL
{
	_C_S_GP22()
	{
		id = _GAME_WAIT;
	}
};
//告知玩家其他玩家都已就绪可以开始接收游戏画面和传递控制信息
struct _S_C_GP23 : public _GAME_PROTOCOL
{
	_S_C_GP23()
	{
		id = _GAME_START;
	}
};
#endif