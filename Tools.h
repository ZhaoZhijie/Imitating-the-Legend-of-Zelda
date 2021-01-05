#ifndef _TOOLS_H
#define _TOOLS_H
struct fRect
{
	float _left;
	float _top;
	float _right;
	float _bottom;
};
class CTools
{
public:
	//r是相交时的重叠矩形
	static bool RectCrash(fRect *r, fRect *r1, fRect *r2);
	//碰撞后的坐标校正
	static void CrashAdjust(fRect rRole, fRect rBlock, float *x1, float *y1,float vx1,float vy1,float *x2=0,float *y2=0,float vx2=0,float vy2=0);
};
#endif