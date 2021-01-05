#include "Tools.h"

bool CTools::RectCrash(fRect *r, fRect *r1, fRect *r2)
{
	if(r1->_right<=r2->_left || r1->_left>=r2->_right || r1->_bottom<=r2->_top || r1->_top>=r2->_bottom)
		return false;
	if(r1->_left<r2->_left)
		r->_left=r2->_left;
	else
		r->_left=r1->_left;
	if(r1->_bottom<r2->_bottom)
		r->_bottom=r1->_bottom;
	else 
		r->_bottom=r2->_bottom;
	if(r1->_top<r2->_top)
		r->_top=r2->_top;
	else
		r->_top=r1->_top;
	if(r1->_right<r2->_right)
		r->_right=r1->_right;
	else
		r->_left=r2->_right;
	return true;
}

void CTools::CrashAdjust(fRect rRole, fRect rBlock,float *x1, float *y1,float vx1,float vy1,float *x2,float *y2,float vx2,float vy2)
{
	fRect c;
	if(!RectCrash(&c, &rRole, &rBlock))
		return;
	if(vx1-vx2>=0)//��������
	{
		float lenx=rRole._right-rBlock._left;
		if(vy1-vy2<0)//����
		{
			float leny=rBlock._bottom-rRole._top;
			if(lenx>=rRole._right-rRole._left)//����ײ
			{
				float t=leny/(vy2-vy1);
				*y1-=vy1*t;	
				if(y2)
					*y2-=vy2*t;
			}
			else if(leny>=rRole._bottom-rRole._top)//����ײ
			{
				if(vx1-vx2!=0)
				{
					float t=lenx/(vx1-vx2);
					*x1-=vx1*t;
					if(x2)
						*x2-=vx2*t;
				}
				else
					*x1-=lenx;
			}
			else//���һ���ж�
			{
				if(-lenx*(vy1-vy2)>leny*(vx1-vx2))//����ײ
				{
					float t=leny/(vy2-vy1);
					*y1-=vy1*t;	
					if(y2)
						*y2-=vy2*t;
				}
				else//����ײ
				{
					if(vx1-vx2!=0)
					{
						float t=lenx/(vx1-vx2);
						*x1-=vx1*t;
						if(x2)
							*x2=-vx2*t;	
					}
					else 
						*x1-=lenx;
				}
			}
		}
		else //����
		{
			float leny=rRole._bottom-rBlock._top;
			if(lenx>=rRole._right-rRole._left)//����ײ
			{
				if(vy1-vy2!=0)
				{
					float t=leny/(vy1-vy2);
					*y1-=vy1*t;	
					if(y2)
						*y2-=vy2*t;
				}
				else 
					*y1-=leny;
			}
			else if(leny>=rRole._bottom-rRole._top)//����ײ
			{
				if(vx1-vx2!=0)
				{
					float t=lenx/(vx1-vx2);
					*x1-=vx1*t;
					if(x2)
						*x2-=vx2*t;	
				}
				else 
					*x1-=lenx;
			}
			else//���һ���ж�
			{
				if(lenx*(vy1-vy2)>leny*(vx1-vx2))//����ײ
				{
					if(vy1-vy2!=0)
					{
						float t=leny/(vy1-vy2);
						*y1-=vy1*t;	
						if(y2)
							*y2-=vy2*t;
					}
					else
						*y1-=leny;
				}
				else//����ײ
				{
					if(vx1-vx2!=0)
					{
						float t=lenx/(vx1-vx2);
						*x1-=vx1*t;
						if(x2)
							*x2-=vx2*t;		
					}
					else
						*x1-=lenx;
				}
			}
		}
	}
	else//��������
	{
		float lenx=rBlock._right-rRole._left;
		if(vy1-vy2<0)//����
		{
			float leny=rBlock._bottom-rRole._top;
			if(lenx>=rRole._right-rRole._left)//����ײ
			{
				float t=leny/(vy2-vy1);
				*y1-=vy1*t;
				if(y2)
					*y2-=vy2*t;
			}
			else if(leny>=rRole._bottom-rRole._top)//����ײ
			{
				float t=lenx/(vx2-vx1);
				*x1-=vx1*t;
				if(x2)
					*x2-=vx2*t;		
			}
			else//���һ���ж�
			{
				if(-lenx*(vy1-vy2)>-leny*(vx1-vx2))//����ײ
				{
					float t=leny/(vy2-vy1);
					*y1-=vy1*t;	
					if(y2)
						*y2-=vy2*t;
				}
				else//����ײ
				{
					float t=lenx/(vx2-vx1);
					*x1-=vx1*t;
					if(x2)
						*x2-=vx2*t;		
				}
			}
		}
		else//����
		{
			float leny=rRole._bottom-rBlock._top;
			if(lenx>=rRole._right-rRole._left)//����ײ
			{
				if(vy1-vy2!=0)
				{
					float t=leny/(vy1-vy2);
					*y1-=vy1*t;	
					if(y2)
						*y2-=vy2*t;
				}
				else
					*y1-=leny;
			}
			else if(leny>=rRole._bottom-rRole._top)//����ײ
			{
					float t=lenx/(vx2-vx1);
					*x1-=vx1*t;
					if(x2)
						*x2-=vx2*t;		
			}
			else//���һ���ж�
			{
				if(lenx*(vy1-vy2)>-leny*(vx1-vx2))//����ײ
				{
					if(vy1-vy2!=0)
					{
						float t=leny/(vy1-vy2);
						*y1-=vy1*t;	
						if(y2)
							*y2-=vy2*t;
					}
					else
						*y1-=leny;
				}
				else//����ײ
				{
					float t=lenx/(vx2-vx1);
					*x1-=vx1*t;
					if(x2)
						*x2-=vx2*t;	
				}
			}
		}
	}
}



//void CTools::CrashAdjust(fRect rRole, fRect rBlock,float *x1, float *y1,float vx1,float vy1)
//{
//	fRect c;
//	if(!RectCrash(&c, &rRole, &rBlock))
//		return;
//	if(vx1>=0)//��������
//	{
//		float lenx=rRole._right-rBlock._left;
//		if(vy1<0)//����
//		{
//			float leny=rBlock._bottom-rRole._top;
//			if(lenx>=rRole._right-rRole._left)//����ײ
//				*y1+=leny;					
//			else if(leny>=rRole._bottom-rRole._top)//����ײ
//				*x1-=lenx;
//			else//���һ���ж�
//			{
//				float tx=lenx/vx1;
//				float ty=leny/vy1;
//				if(-lenx*vy1>leny*vx1)//����ײ
//					*y1+=leny;
//				else//����ײ
//					*x1-=lenx;
//			}
//		}
//		else //����
//		{
//			float leny=rRole._bottom-rBlock._top;
//			if(lenx>=rRole._right-rRole._left)//����ײ
//				*y1-=leny;
//			else if(leny>=rRole._bottom-rRole._top)//����ײ
//				*x1-=lenx;
//			else//���һ���ж�
//			{
//				float tx=lenx/vx1;
//				float ty=leny/vy1;
//				if(lenx*vy1>leny*vx1)//����ײ
//					*y1-=leny;	
//				else//����ײ
//					*x1-=lenx;					
//			}
//		}
//	}
//	else//��������
//	{
//		float lenx=rBlock._right-rRole._left;
//		if(vy1<0)//����
//		{
//			float leny=rBlock._bottom-rRole._top;
//			if(lenx>=rRole._right-rRole._left)//����ײ
//				*y1+=leny;
//			else if(leny>=rRole._bottom-rRole._top)//����ײ
//				*x1+=lenx;
//			else//���һ���ж�
//			{
//				float tx=lenx/vx1;
//				float ty=leny/vy1;
//				if(-lenx*vy1>-leny*vx1)//����ײ
//					*y1+=leny;
//				else//����ײ
//					*x1+=lenx;
//			}
//		}
//		else//����
//		{
//			float leny=rRole._bottom-rBlock._top;
//			if(lenx>=rRole._right-rRole._left)//����ײ
//				*y1-=leny;
//			else if(leny>=rRole._bottom-rRole._top)//����ײ
//				*x1+=lenx;
//			else//���һ���ж�
//			{
//				float tx=lenx/vx1;
//				float ty=leny/vy1;
//				if(lenx*vy1>-leny*vx1)//����ײ
//					*y1-=leny;
//				else//����ײ
//					*x1+=lenx;
//			}
//		}
//	}
//}