#include "PlayerMove.h"
#include <Windows.h>
CPlayerMove::CPlayerMove()
{
	pos=-1;
}

void CPlayerMove::Push(unsigned char keycode)
{
	key[++pos]=keycode;
}
void CPlayerMove::Eraser(unsigned char keycode)
{
	bool tag=false;
	for(int i=0;i<=pos;i++)
	{
		if(!tag)
		{
			if(key[i]==keycode)
				tag=true;
		}else
		{
			key[i-1]=key[i];
		}
	}
	pos--;
}

unsigned short CPlayerMove::GetDirect()
{// 2 4 6 8     2*8=16    4*6=24     2*4=8  2*6 =12  4*8=32  6*8=48
	//���ֽڱ�ʾ8����֮һ  ���ֽڱ�ʾ4����֮һ
 	if(pos==0)
		return MAKEWORD(key[pos],key[pos]);
	else if(pos>0)
	{
		/*˵��ͬʱ������2��8 ����ͬʱ����4��6 �Ժ󰴵�Ϊ׼
			��������������󰴵�ֻ�ı��ƶ����򲻸ı���������
		*/
		int num=key[pos]*key[pos-1];
		if(num==16||num==24)
			return MAKEWORD(key[pos],key[pos]);
		else if(num==8)
			return MAKEWORD(1,key[pos-1]);
		else if(num==12)
			return MAKEWORD(3,key[pos-1]);
		else if(num==32)
			return MAKEWORD(7,key[pos-1]);
		else if(num==48)
			return MAKEWORD(9,key[pos-1]);
	}
	else
		return 0;
}

int CPlayerMove::GetPos()
{
	return pos;
}