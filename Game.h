#ifndef _GAME_H
#define _GAME_H

//游戏处理框架
//1.接收玩家输入信息  键盘鼠标输入 键盘：键码 状态  鼠标  x y坐标
//2.提供玩家画面数据 存放画面数据的缓冲区前10个字节空出来 交给外部处理 也就是如果用这10个字节
//存放某种协议以便从网络发送则该协议结构体不能超过10个字节

class CGame
{
public:
	//构造函数
	CGame();
	virtual ~CGame();
	//根据id获取某个玩家的画面数据 (*data)接收返回的画面数据 图片标识 x y都用short
	virtual bool Output(int id, char ** data, int *len,int * virx, int * viry)=0;
	//接收输入  p1 p2对应具体信息 如果是键盘 p1表示key p2表示state 鼠标的话就是坐标
	virtual void Input(int id, int type, int p1, int p2)=0;
	//运行游戏逻辑
	virtual void Run()=0;
};
#define _GI_M_L 0x00 //鼠标左键
#define _GI_M_M 0x01 //鼠标中键
#define _GI_M_R 0x02 //鼠标右键
#define _GI_K_A 0x03 //键盘字母键A
#define _GI_K_B 0x04 //键盘字母键B
#define _GI_K_C 0x05 //键盘字母键C
#define _GI_K_D 0x06 //键盘字母键D
#define _GI_K_E 0x07 //键盘字母键E
#define _GI_K_F 0x08 //键盘字母键F
#define _GI_K_G 0x09 //键盘字母键G
#define _GI_K_H 0x0A //键盘字母键H
#define _GI_K_I 0x0B //键盘字母键I
#define _GI_K_J 0x0C //键盘字母键J
#define _GI_K_K 0x0D //键盘字母键K
#define _GI_K_L 0x0E //键盘字母键L
#define _GI_K_M 0x0F //键盘字母键M
#define _GI_K_N 0x10 //键盘字母键N
#define _GI_K_O 0x11 //键盘字母键O
#define _GI_K_P 0x12 //键盘字母键P
#define _GI_K_Q 0x13 //键盘字母键Q
#define _GI_K_R 0x14 //键盘字母键R
#define _GI_K_S 0x15 //键盘字母键S
#define _GI_K_T 0x16 //键盘字母键T
#define _GI_K_U 0x17 //键盘字母键U
#define _GI_K_V 0x18 //键盘字母键V
#define _GI_K_W 0x19 //键盘字母键W
#define _GI_K_X 0x1A //键盘字母键X
#define _GI_K_Y 0x1B //键盘字母键Y
#define _GI_K_Z 0x1C //键盘字母键Z
#define _GI_K_0 0x1D //键盘数字键0
#define _GI_K_1 0x1E //键盘数字键1
#define _GI_K_2 0x1F //键盘数字键2
#define _GI_K_3 0x20 //键盘数字键3
#define _GI_K_4 0x21 //键盘数字键4
#define _GI_K_5 0x22 //键盘数字键5
#define _GI_K_6 0x23 //键盘数字键6
#define _GI_K_7 0x24 //键盘数字键7
#define _GI_K_8 0x25 //键盘数字键8
#define _GI_K_9 0x26 //键盘数字键9
#define _GI_K_F01 0x27 //键盘键F1
#define _GI_K_F02 0x28 //键盘键F2
#define _GI_K_F03 0x29 //键盘键F3
#define _GI_K_F04 0x2A //键盘键F4
#define _GI_K_F05 0x2B //键盘键F5
#define _GI_K_F06 0x2C //键盘键F6
#define _GI_K_F07 0x2D //键盘键F7
#define _GI_K_F08 0x2E //键盘键F8
#define _GI_K_F09 0x2F //键盘键F9
#define _GI_K_F10 0x30 //键盘键F10
#define _GI_K_F11 0x31 //键盘键F11
#define _GI_K_F12 0x32 //键盘键F12
#define _GI_K_ESC 0x33 //键盘键Esc
#define _GI_K_OEM_3 0x34 //键盘键~
#define _GI_K_OEM_MINUS 0x35 //键盘键_
#define _GI_K_OEM_PLUS 0x36 //键盘键+
#define _GI_K_OEM_5 0x37 //键盘键|
#define _GI_K_BACK 0x38 //键盘键Back Space
#define _GI_K_TAB 0x39 //键盘键Tab
#define _GI_K_OEM_4 0x3A //键盘键{
#define _GI_K_OEM_6 0x3B //键盘键}
#define _GI_K_RETURN 0x3C //键盘键Enter
#define _GI_K_CAPITAL_LOCK 0x3D //键盘键Caps Lock
#define _GI_K_OEM_1 0x3E //键盘键:
#define _GI_K_OEM_7 0x3F //键盘键"
#define _GI_K_L_SHIFT 0x40 //键盘键左Shift
#define _GI_K_OEM_COMMA 0x41 //键盘键<
#define _GI_K_OEM_PERIOD 0x42 //键盘键>
#define _GI_K_OEM_2 0x43 //键盘键?
#define _GI_K_R_SHIFT 0x44 //键盘键右Shift
#define _GI_K_L_CTRL 0x45 //键盘键左Ctrl
#define _GI_K_L_ALT 0x46 //键盘键左Alt
#define _GI_K_SPACE 0x47 //键盘键Spacebar
#define _GI_K_R_ALT 0x48 //键盘键右Alt
#define _GI_K_R_CTRL 0x49 //键盘键右Ctrl
#define _GI_K_PRINT_SCREEN 0x4A //键盘键Print Screen
#define _GI_K_SCROLL_LOCK 0x4B //键盘键Scroll Lock
#define _GI_K_INSERT 0x4C //键盘键Insert
#define _GI_K_HOME 0x4D //键盘键Home
#define _GI_K_PAGE_UP 0x4E //键盘键Page Up
#define _GI_K_DELETE 0x4F //键盘键Delete
#define _GI_K_END 0x50 //键盘键End
#define _GI_K_PAGE_DOWN 0x51 //键盘键Page Down
#define _GI_K_UP 0x52 //键盘键↑
#define _GI_K_DOWN 0x53 //键盘键↓
#define _GI_K_LEFT 0x54 //键盘键←
#define _GI_K_RIGHT 0x55 //键盘键→
#define _GI_NK_NUM_LOCK 0x56 //小键盘键Num Lock
#define _GI_NK_DIVIDE 0x57 //小键盘键除
#define _GI_NK_MULTIPLY 0x58 //小键盘键乘
#define _GI_NK_ADD 0x59 //小键盘键加
#define _GI_NK_SEPARATOR 0x5A //小键盘键减
#define _GI_NK_RETURN _GI_K_RETURN //小键盘键Enter
#define _GI_NK_DECIMAL 0x5B //小键盘键小数点
#define _GI_NK_0 0x5C //小键盘键数字键0
#define _GI_NK_1 0x5D //小键盘键数字键1
#define _GI_NK_2 0x5E //小键盘键数字键2
#define _GI_NK_3 0x5F //小键盘键数字键3
#define _GI_NK_4 0x60 //小键盘键数字键4
#define _GI_NK_5 0x61 //小键盘键数字键5
#define _GI_NK_6 0x62 //小键盘键数字键6
#define _GI_NK_7 0x63 //小键盘键数字键7
#define _GI_NK_8 0x64 //小键盘键数字键8
#define _GI_NK_9 0x65 //小键盘键数字键9

#define _GI_KEY_NUM (0x66) //键盘键的总数

#define _KS_IK 0 //键状态:无效
#define _KS_UH 1 //键状态:持续放开
#define _KS_UC 2 //键状态:当前放开
#define _KS_DH 3 //键状态:持续按下
#define _KS_DC 4 //键状态:当前按下
#endif