#ifndef _PLAYER_MOVE
#define _PLAYER_MOVE

class CPlayerMove
{
	  unsigned char key[4];//���淽���˳��
	  int pos;//������ķ��򣬼����顰���һ����Ԫ�ص�λ��
public:
	 CPlayerMove();
	  void Push(unsigned char keycode);//ĳ�����������
	  void Eraser(unsigned char keycode);//ĳ�����������
	  unsigned short GetDirect();//��ȡ��ǰ�ķ���
	  int GetPos();
};
#endif