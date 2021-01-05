#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <Winsock2.h>
#include "Map.h"
#include "Queue.h"
#include "MemoryPool.h"

#define _TSNE_CONNECT 0
#define _TSNE_S_DISCONNECT 1
#define _TSNE_C_DISCONNECT 2
#define _TSNE_E_DISCONNECT 3
#define _TSNE_DATA 4

struct _TS_NET_EVENT
{
	//0���пͻ���������
	//1����������Ϊ�������ĶϿ�
	//2���ͻ�����Ϊ�������ĶϿ�
	//3����Ϊ����ĶϿ�
	//4���յ����Կͻ��˵�����
	int type; //�¼�����
	SOCKET s; //�ͻ���ʶ
	unsigned long ip; //�ͻ�ip��ַ
	char* data; //����
	int len; //���ݳ���
};

class CTcpServer
{
	unsigned short m_Port; //�˿ں�
	int m_MaxClient; //���������
	int m_StartKAYime; //����������ʱ��
	int m_IntervalKAYime; //���������ʱ��
	bool m_Continue; //ѭ�����
	SOCKET m_Listen; //�����׽���
	CQueue<_TS_NET_EVENT> m_TSNetEvent; //�����¼�����
	struct _SEND_DATA //�������ݽṹ��
	{
		int allsize; //���ݰ��ܴ�С
		char* data; //����
		int sendsize; //�Ѿ����͵Ĵ�С
	};
	struct _RECV_DATA //�������ݽṹ��
	{
		int allsize; //�����ܳ���
		int allsize_bytes; //�����ܳ��ȵ���Ч�ֽ�
		char* data; //����
		int recvsize; //�Ѿ��յ������ݳ���
	};
	struct CLIENT
	{
		int state; //״̬�����ӡ�׼�������Ͽ��������Ͽ����ȴ�����
		unsigned long ip; //ip��ַ
		CQueue<_SEND_DATA> sd_queue; //�������ݶ���
		_RECV_DATA rd; //��������
	};
	CMap<SOCKET, CLIENT> m_Client; //�ͻ���ӳ��
	CMemoryPool m_MP; //�ڴ��

	int _AcceptClient();
	void _OperateData(const char* buf, int r, SOCKET s, unsigned long ip, _RECV_DATA* rd);
	int _RecvData();
	int _SendData();
	void _Close(int index, SOCKET s, CLIENT* c, int type);

public:

	//��ʼ��
	bool Init(
		unsigned short port, //�˿ں�
		int max_client, //���������
		int start_ka_time, //����������ʱ�䣨�룩
		int interval_ka_time); //���������ʱ�䣨�룩

	//��ֹ
	void Stop();

	//ѭ����־
	bool Continue();

	//����
	void End();

	//����
	//1����������
	//2����������
	//3����������
	void Run();

	//�õ������¼�
	bool GetTSNetEvent(_TS_NET_EVENT* ptne);

	//�ͷ������¼���ʹ�õ��ڴ�
	void ReleaseTSNetEvent(_TS_NET_EVENT* ptne);

	//�Ͽ�����
	int Disconnect(SOCKET s);

	//��������
	int SendData(SOCKET s, const char* data, int len);

	//����ip��ַת�ַ���ip��ַ
	static void GetStrIP(unsigned long ul_ip, char* str_ip);

	//�õ���ǰ�ڴ�ʹ�����
	float GetUseMemoryMB();
};

#endif