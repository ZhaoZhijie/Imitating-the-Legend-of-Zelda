#ifndef _PLAYER_MOVE
#define _PLAYER_MOVE

class CPlayerMove
{
	  unsigned char key[4];//保存方向键顺序
	  int pos;//最后键入的方向，即数组“最后一个”元素的位置
public:
	 CPlayerMove();
	  void Push(unsigned char keycode);//某个方向键按下
	  void Eraser(unsigned char keycode);//某个方向键弹起
	  unsigned short GetDirect();//获取当前的方向
	  int GetPos();
};
#endif