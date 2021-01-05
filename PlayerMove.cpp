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
	//低字节表示8方向之一  高字节表示4面向之一
 	if(pos==0)
		return MAKEWORD(key[pos],key[pos]);
	else if(pos>0)
	{
		/*说明同时按下了2和8 或者同时按下4和6 以后按的为准
			按下其他方向键后按的只改变移动方向不改变人物面向
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