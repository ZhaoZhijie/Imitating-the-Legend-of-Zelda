#ifndef _GAME_PROTOCOL_H_
#define _GAME_PROTOCOL_H_

#define _TRY_NICK_NAME 1		//(�ͻ���->������)����ǳ�
#define _TRY_PASS 2//������֤����
#define _TRY_RESULT 3//������֤���
#define _ROOM_DATA 4//�����ķ�������
#define _ENTER_HALL 5 //������� �����������
#define _CREATE_ROOM 6//��������
#define  _CREATE_RESULT 7 //���䴴�����
#define  _ENTER_ROOM 8//���뷿������
#define  _PLAYER_DATA 9//�������
#define  _EROOM_RESULT 10//���뷿����
#define  _OWNER_DATA 11//��ǰ�������
#define  _CUR_ROOM 12 //��ǰ��������
#define  _PRE_CLICK 13//��ҵ��׼����ť
#define  _PRE_CLICK_RESULT 14//׼�����������
#define  _I_QUIT 15//�˳�����
#define  _H_QUIT 16//��һ����˳�����
#define  _ROOM_DEL 17 //����ɾ��
#define  _PLAYER_INPUT 18 //�����������
#define _PLAYER_OUTPUT 19//����������
#define _CLICK_START 20 //�����ʼ
#define  _ENTER_GAME 21//��֪��ҿ��Խ�����Ϸ����
#define  _GAME_WAIT 22//��֪�������ѽ��볡���ȴ����������
#define  _GAME_START 23//��֪���������Ҷ��Ѿ������Կ�ʼ������Ϸ����ʹ��ݿ�����Ϣ
struct _GAME_PROTOCOL
{
	//Э��ID
	int id;
};

//����ǳ��Ƿ��Ѿ�����
struct _C_S_GP1 : public _GAME_PROTOCOL
{
	_C_S_GP1()
	{
		id = _TRY_NICK_NAME;
	}
	char nick_name[32];
};
//������֤����
struct _C_S_GP2 : public _GAME_PROTOCOL
{
	_C_S_GP2()
	{
		id = _TRY_PASS;
	}
	char uname[20];
	char upass[20];
};
//������֤���
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
//������������
struct _S_C_GP4 : public _GAME_PROTOCOL
{
	_S_C_GP4()
	{
		id = _ROOM_DATA;
	}
	int no;//������
	char name[20];//��������
	int num;//��������
	int tag;//����״̬
	int type;//��ͼ����
};
//�����������˵��
struct _C_S_GP5 : public _GAME_PROTOCOL
{
	_C_S_GP5()
	{
		id = _ENTER_HALL;
	}
};
//��������
struct _C_S_GP6 : public _GAME_PROTOCOL
{
	_C_S_GP6()
	{
		id = _CREATE_ROOM;
	}
	char name[20];//������
};
//���䴴�����
struct _S_C_GP7 : public _GAME_PROTOCOL
{
	_S_C_GP7()
	{
		id = _CREATE_RESULT;
	}
	bool result;
};
//���뷿������
struct _C_S_GP8 : public _GAME_PROTOCOL
{
	_C_S_GP8()
	{
		id = _ENTER_ROOM;
	}
	int no;//������
};
//�������
struct _S_C_GP9 : public _GAME_PROTOCOL
{
	_S_C_GP9()
	{
		id = _PLAYER_DATA;
	}
	char _name[20];//�������
	int _state;//���״̬
	int _order;//��߻����ұ� 0����� 1���ұ�
};
struct _S_C_GP10 : public _GAME_PROTOCOL
{
	_S_C_GP10()
	{
		id = _EROOM_RESULT;
	}
	bool result;
};
//��ǰ�������
struct _S_C_GP11 : public _GAME_PROTOCOL
{
	_S_C_GP11()
	{
		id = _OWNER_DATA;
	}
	char _name[20];//�������
};
//��ǰ��������
struct _S_C_GP12 : public _GAME_PROTOCOL
{
	_S_C_GP12()
	{
		id = _CUR_ROOM;
	}
	int _no;//������
	char _name[20];//��������
	int _type;
};
//��ҵ��׼����ť
struct _C_S_GP13 : public _GAME_PROTOCOL
{
	_C_S_GP13()
	{
		id = _PRE_CLICK;
	}
	int _no;//������
};
//���׼�����������
struct _S_C_GP14 : public _GAME_PROTOCOL
{
	_S_C_GP14()
	{
		id = _PRE_CLICK_RESULT;
	}
	char _name[20];//�������
	int _state;//�����״̬
};
//��֪�������˳�����
struct _C_S_GP15 : public _GAME_PROTOCOL
{
	_C_S_GP15()
	{
		id = _I_QUIT;
	}
	int _no;
};
//��֪��Ҹ÷�����һ������˳�����
struct _S_C_GP16 : public _GAME_PROTOCOL
{
	_S_C_GP16()
	{
		id = _H_QUIT;
	}
};
//����ɾ��
struct _S_C_GP17 : public _GAME_PROTOCOL
{
	_S_C_GP17()
	{
		id = _ROOM_DEL;
	}
	int no;
};
//���������Ϣ
#define _INTYPE_KEY 0
#define _INTYPE_MOUSE_CLICK 1
struct _C_S_GP18 : public _GAME_PROTOCOL
{
	_C_S_GP18()
	{
		id = _PLAYER_INPUT;
	}
	//������ ���id �������� p1,p1������Ϣ
	int type; int p1; int p2;
};
//��һ�������
struct _S_C_GP19 : public _GAME_PROTOCOL
{
	_S_C_GP19()
	{
		id = _PLAYER_OUTPUT;
	}
	//id �������� p1,p1������Ϣ
	int _len;
	unsigned short _virx;
	unsigned short _viry;
};
//���������ʼ
struct _C_S_GP20 : public _GAME_PROTOCOL
{
	_C_S_GP20()
	{
		id = _CLICK_START;
	}
	int _no;
};
//��֪��ҿ��Խ�����Ϸ����
struct _S_C_GP21 : public _GAME_PROTOCOL
{
	_S_C_GP21()
	{
		id = _ENTER_GAME;
	}
};
//��֪�������ѽ�����Ϸ���� �ȴ��������
struct _C_S_GP22 : public _GAME_PROTOCOL
{
	_C_S_GP22()
	{
		id = _GAME_WAIT;
	}
};
//��֪���������Ҷ��Ѿ������Կ�ʼ������Ϸ����ʹ��ݿ�����Ϣ
struct _S_C_GP23 : public _GAME_PROTOCOL
{
	_S_C_GP23()
	{
		id = _GAME_START;
	}
};
#endif