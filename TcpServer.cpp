#include "TcpServer.h"
#include <MSTcpIP.h>
#pragma comment(lib, "Ws2_32.lib")

#define _CS_CONNECTED 0 //����״̬
#define _CS_PREPARE_DISCONNECT 1 //������׼�������Ͽ�״̬
#define _CS_BE_DISCONNECTED 2 //�����������Ͽ�״̬
#define _CS_WAIT_FINISH 3 //�������ȴ�����״̬

int CTcpServer::_AcceptClient()
{
	if (!m_Continue || m_Client.Length() == m_MaxClient)
		return 0;

	//���տͻ�������
	sockaddr_in ci;
	int ci_len = sizeof(ci);
	SOCKET c = accept(m_Listen, (sockaddr*)&ci, &ci_len);

	//���ʹ���
	if (INVALID_SOCKET == c)
	{
		if (WSAEWOULDBLOCK == WSAGetLastError())
			return 1;
		else
			return -1;
	}
	//��������
	else
	{
		//����������
		BOOL b = TRUE;
		setsockopt(c, SOL_SOCKET, SO_KEEPALIVE, (const char*)&b, sizeof(b));
		tcp_keepalive tk = {1, m_StartKAYime, m_IntervalKAYime};
		DWORD w;
		WSAIoctl(c, SIO_KEEPALIVE_VALS, &tk, sizeof(tk), 0, 0, &w, 0, 0);

		//װ��ͻ�ӳ��
		CLIENT client;
		client.state = _CS_CONNECTED;
		client.ip = ci.sin_addr.S_un.S_addr;
		client.rd.allsize = 0;
		client.rd.allsize_bytes = 0;
		client.rd.data = 0;
		client.rd.recvsize = 0;
		m_Client.Insert(c, client);

		//�����¼������
		_TS_NET_EVENT tne = {_TSNE_CONNECT, c, client.ip};
		m_TSNetEvent.Enter(tne);

		return 2;
	}
}

void CTcpServer::_OperateData(const char* buf, int r, SOCKET s, unsigned long ip, _RECV_DATA* rd)
{
	//��ǰ�����±�
	int cur_i = 0;
	while (cur_i < r)
	{
		//��ǰ���ճ��Ȳ���
		if (rd->allsize_bytes < 4)
		{
			//��ǰ�������곤�Ȳ���
			if (4 - rd->allsize_bytes > r - cur_i)
			{
				//���ֽڿ����������ܳ���
				memcpy((char*)&rd->allsize + rd->recvsize, buf + cur_i, r - cur_i);

				//�����ܳ�����Ч�ֽ�������
				rd->allsize_bytes += r - cur_i;

				//��ǰ�����±����
				cur_i += r - cur_i;
			}
			//��ǰ�������곤�Ȳ���
			else
			{
				//���ֽڿ����������ܳ���
				memcpy((char*)&rd->allsize + rd->recvsize, buf + cur_i, 4 - rd->allsize_bytes);

				//��ǰ�����±����
				cur_i += 4 - rd->allsize_bytes;

				//�����ܳ�����Ч�ֽ�������Ϊ4
				rd->allsize_bytes = 4;

				//���ٶ��ڴ�
				rd->data = (char*)m_MP.Allocate(rd->allsize);
			}
		}
		//��ǰ�������ݲ���
		else
		{
			//��ǰ���ܽ���������
			if (rd->allsize - rd->recvsize > r - cur_i)
			{
				//����ʣ�µ������ֽڵ�������
				memcpy(rd->data + rd->recvsize, buf + cur_i, r - cur_i);

				//��ǰ�Ѿ����յ����ݳ��ȵ���
				rd->recvsize += r - cur_i;

				//��ǰ�����±����
				cur_i += r - cur_i;
			}
			else
			{
				//������ǰ��������Ҫ���ֽڵ�������
				memcpy(rd->data + rd->recvsize, buf + cur_i, rd->allsize - rd->recvsize);

				//��ǰ�����±����
				cur_i += rd->allsize - rd->recvsize;

				//�����������¼�
				_TS_NET_EVENT tne = {_TSNE_DATA, s, ip, rd->data, rd->allsize};
				m_TSNetEvent.Enter(tne);

				//��ս���
				rd->allsize = 0;
				rd->allsize_bytes = 0;
				rd->data = 0;
				rd->recvsize = 0;
			}
		}
	}
}

int CTcpServer::_RecvData()
{
	const int len = 1024 * 64;
	char buf[len];

	for (int i = 0; i < m_Client.Length(); ++i)
	{
		//������Ǳ����Ͽ�״̬��ô���׽��ֽ��н�������
		if (m_Client.At(i)->data.state != _CS_BE_DISCONNECTED)
		{
			//��������
			int r = recv(m_Client.At(i)->key, buf, len, 0);

			//����
			if (r == SOCKET_ERROR)
			{
				//��ĳ�����
				if (WSAEWOULDBLOCK != WSAGetLastError())
				{
					//��Ϊ����ɾ����ǰ�ͻ��˼���һ�������Դ
					_Close(i, m_Client.At(i)->key, &m_Client.At(i)->data, _TSNE_E_DISCONNECT);
					--i;
				}
			}
			//���ŶϿ�
			else if (r == 0)
			{
				//�������ȴ�����״̬
				if (m_Client.At(i)->data.state == _CS_WAIT_FINISH)
				{
					//��Ϊ�����������Ͽ�ɾ����ǰ�ͻ��˼���һ�������Դ
					_Close(i, m_Client.At(i)->key, &m_Client.At(i)->data, _TSNE_S_DISCONNECT);
					--i;
				}
				//����״̬�������׼�������Ͽ�״̬
				else
					m_Client.At(i)->data.state = _CS_BE_DISCONNECTED;
			}
			//�յ�������
			else
				_OperateData(buf, r, m_Client.At(i)->key, m_Client.At(i)->data.ip, &m_Client.At(i)->data.rd);
		}
	}

	return 1;
}

int CTcpServer::_SendData()
{
	for (int i = 0; i < m_Client.Length(); ++i)
	{
		//������ǵȴ�����״̬��ô���׽��ֽ��з�������
		if (m_Client.At(i)->data.state != _CS_WAIT_FINISH)
		{
			//��������ѭ��ʱ��ı��
			//0��ʾ���ͻ����������������������
			//1��ʾ��ʱ���ܷ���
			//2��ʾ����
			int flag1 = 0;

			//���Ͷ�ͷ����
			_SEND_DATA* sd;
			while (m_Client.At(i)->data.sd_queue.Length() > 0)
			{
				//�õ���ͷ����
				m_Client.At(i)->data.sd_queue.GetHead(&sd);

				//��������ѭ��ʱ��ı��
				//0��ʾ�����ݰ������������
				//1��ʾ��ʱ���ܷ���
				//2��ʾ����
				int flag2 = 0;

				//ѭ�����Ͷ�ͷ����
				while (sd->sendsize < sd->allsize)
				{
					//����
					int r = send(
						m_Client.At(i)->key,
						sd->data + sd->sendsize,
						sd->allsize - sd->sendsize,
						0);

					//����
					if (SOCKET_ERROR == r)
					{
						//��ǰ�ݲ��ܷ�
						if (WSAEWOULDBLOCK == WSAGetLastError())
							flag2 = 1;
						//��ǰ����
						else
							flag2 = 2;
						break;
					}
					else
						sd->sendsize += r;
				}

				if (flag2 == 0)
				{
					m_MP.Free(sd->data);
					m_Client.At(i)->data.sd_queue.Quit(sd);
				}
				else
				{
					flag1 = flag2;
					break;
				}
			}

			//����״ֵ̬���д���

			//��ǰ�ͻ�û�д����͵�������
			if (flag1 == 0)
			{
				switch (m_Client.At(i)->data.state)
				{
				case _CS_CONNECTED: break;
				case _CS_PREPARE_DISCONNECT:
					{
						shutdown(m_Client.At(i)->key, SD_SEND);
						m_Client.At(i)->data.state = _CS_WAIT_FINISH;
						break;
					}
				case _CS_BE_DISCONNECTED:
					{
						shutdown(m_Client.At(i)->key, SD_SEND);
						//��Ϊ�ͻ��������Ͽ�ɾ����ǰ�ͻ��˼���һ�������Դ
						_Close(i, m_Client.At(i)->key, &m_Client.At(i)->data, _TSNE_C_DISCONNECT);
						--i;
					}
				}
			}
			//��ʱ���ܷ��Ļ��ͻ���һ����
			else if (flag1 == 1)
				;
			//������ôɾ���ÿͻ�
			else
			{
				//��Ϊ����ɾ����ǰ�ͻ��˼���һ�������Դ
				_Close(i, m_Client.At(i)->key, &m_Client.At(i)->data, _TSNE_E_DISCONNECT);
				--i;
			}
		}
	}

	return 1;
}

void CTcpServer::_Close(int index, SOCKET s, CLIENT* c, int type)
{
	//ɾ���ÿͻ��ķ�������
	while (c->sd_queue.Length() > 0)
	{
		_SEND_DATA sd;
		c->sd_queue.Quit(&sd);
		m_MP.Free(sd.data);
	}

	//ɾ���ÿͻ��Ľ�������
	if (c->rd.data)
		m_MP.Free(c->rd.data);

	//�ر��׽���
	closesocket(s);

	//Ͷ�������¼�
	_TS_NET_EVENT tne = {type, s, c->ip};
	m_TSNetEvent.Enter(tne);

	//���ͻ��ӿͻ�ӳ����ɾ��
	m_Client.Erase(index);
}

bool CTcpServer::Init(unsigned short port,
					  int max_client,
					  int start_ka_time,
					  int interval_ka_time)
{
	//�������
	if (port <= 1024 || max_client < 1 || start_ka_time < 1 || start_ka_time < 1)
		return false;

	//�õ�����Ĳ���
	m_Port = port;
	m_MaxClient = max_client;
	m_StartKAYime = start_ka_time * 1000;
	m_IntervalKAYime = interval_ka_time * 1000;

	//����ѭ����־Ϊ��
	m_Continue = true;

	//�������绷��
	WSADATA wd;
	WSAStartup(MAKEWORD(1, 1), &wd);

	//���������׽��ֲ�����Ϊ������ģʽ
	m_Listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	unsigned long nonblocking = 1;
	ioctlsocket(m_Listen, FIONBIO, &nonblocking);

	//����ַ��Ϣ�ṹ��
	sockaddr_in si;
	si.sin_family = AF_INET;
	si.sin_port = htons(m_Port);
	si.sin_addr.S_un.S_addr = ADDR_ANY;

	//�󶨼����׽������ַ��Ϣ�ṹ��
	bind(m_Listen, (sockaddr*)&si, sizeof(si));

	//���ü���ģʽ
	listen(m_Listen, 8);

	return true;
}

void CTcpServer::Stop()
{
	for (int i = 0; i < m_Client.Length(); ++i)
	{
		if (m_Client.At(i)->data.state == _CS_CONNECTED)
			m_Client.At(i)->data.state = _CS_PREPARE_DISCONNECT;
	}
	m_Continue = false;
}

bool CTcpServer::Continue()
{
	return m_Continue || m_Client.Length() > 0;
}

void CTcpServer::End()
{
	closesocket(m_Listen);
	WSACleanup();
}

void CTcpServer::Run()
{
	_AcceptClient();
	_RecvData();
	_SendData();
}

bool CTcpServer::GetTSNetEvent(_TS_NET_EVENT* ptne)
{
	return m_TSNetEvent.Quit(ptne);
}

void CTcpServer::ReleaseTSNetEvent(_TS_NET_EVENT* ptne)
{
	if (_TSNE_DATA == ptne->type)
		m_MP.Free(ptne->data);
}

int CTcpServer::Disconnect(SOCKET s)
{
	CMap<SOCKET, CLIENT>::PAIR* pair = m_Client.Find(s);
	if (0 == pair)
		return 0;
	else
	{
		if (pair->data.state != _CS_CONNECTED)
			return -1;
		else
		{
			pair->data.state = _CS_PREPARE_DISCONNECT;
			return 1;
		}
	}
}

int CTcpServer::SendData(SOCKET s, const char* data, int len)
{
	CMap<SOCKET, CLIENT>::PAIR* pair = m_Client.Find(s);
	if (0 == pair)
		return 0;
	else
	{
		if (pair->data.state != _CS_CONNECTED)
			return -1;
		else
		{
			//׼����������
			_SEND_DATA sd;
			sd.allsize = sizeof(int) + len;
			sd.data = (char*)m_MP.Allocate(sd.allsize);
			*((int*)sd.data) = len;
			memcpy(sd.data + sizeof(int), data, len);
			sd.sendsize = 0;

			//�������������
			pair->data.sd_queue.Enter(sd);

			return 1;
		}
	}
}

void CTcpServer::GetStrIP(unsigned long ul_ip, char* str_ip)
{
	in_addr ia;
	ia.S_un.S_addr = ul_ip;
	char* str = inet_ntoa(ia);
	memcpy(str_ip, str, strlen(str) + 1);
}

float CTcpServer::GetUseMemoryMB()
{
	return m_MP.GetUseMemory();
}