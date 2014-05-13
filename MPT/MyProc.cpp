#include <StdAfx.h>
#include "MyProc.h"
#include <math.h>

//声明外部文件的全局变量
extern int peopleNum;
extern int groupNum;

extern int tourNum;
extern int connNum;

extern int g_frameNum;
extern float g_frameTimeGap;

extern int leftBorder;

extern CRect rect;

// 16个检测框
extern _DetectRect detectRect[16];

// 最终采用的检测框的编号
int finalRectNum;

// 是否已经选定了最终用于比较的线段
bool isRectConfirmed;

CString originText, newText, newText2;
extern int firstFlag;

// 四舍五入
int round(double num)
{
	return (int)floor(num + 0.5);
}

//基于游程的连通区域标记算法
void LabelConnByTour(struct foreImg* fore, struct tour* tours)
{
	int i, j, k, l;

	int maxStart;
	int minRight;
	
	//遍历每一行
	for(i=0; i<HEIGHT; i++)
	{
		//找到每一行的游程
		for(j=0; j<WIDTH; j++)
		{
			if(fore[i*WIDTH+j].isFore)
			{
				tours[tourNum].rowNum = i;
				tours[tourNum].start = j;

				while(true)
				{
					j++;					
					if((j == WIDTH) || (fore[i*WIDTH+j].isFore == false))
					{
						tours[tourNum].end = j - 1;
						break;
					}
				}

				tourNum++;
			}
		}

		//若为连通区域的第一行
		if((connNum == 0) && (tourNum != 0))
		{
			//则针对每一个游程分配一个新的区域标号
			for(j=0; j<tourNum; j++)
			{
				if(tours[j].rowNum == i)
				{	
					tours[j].connNum = connNum;
					connNum++;
				}
			}
		} 
		else	//若不为第一行
		{
			//则将当前行的每一个游程 R_now 与上一行的每一个游程 R_pre 进行匹配
			for(j=0; j<tourNum; j++)
			{
				if(tours[j].rowNum == i)	//当前行
				{
					for(k=0; k<tourNum; k++)
					{
						if(tours[k].rowNum == (i-1))	//上一行
						{
							maxStart = tours[j].start>tours[k].start? tours[j].start : tours[k].start;
							minRight = tours[j].end<tours[k].end? tours[j].end: tours[k].end;
							
							//若 R_now 找到了能与之匹配的 R_pre
							if(maxStart < minRight)
							{
								//若此 R_now 未标记
								if(!tours[j].isMatched)
								{
									//则将 R_pre 的标号赋予 R_now，并标记此 R_now 为已标记
									tours[j].connNum = tours[k].connNum;
									tours[j].isMatched = true;
								} 
								else	//若此 R_now 已标记
								{
									//则将 R_now 的标号赋予 R_pre 及其所有的联通游程
									for(l=0; l<tourNum; l++)
									{
										if(tours[l].connNum == tours[k].connNum)
										{
											tours[l].connNum = tours[j].connNum;
										}
									}
								}
							}//若匹配
						}//上一行
					}
				}//当前行
			}

			//遍历所有的 R_now
			for(j=0; j<tourNum; j++)
			{
				//若某 R_now 未匹配，则为其分配新的区域标号
				if((tours[j].rowNum == i) && (!tours[j].isMatched))
				{
					//AfxMessageBox("s");
					tours[j].connNum = connNum;
					connNum++;
				}
			}
		}//前后两行进行匹配
	}

	//将所有游程的区域标号赋予其对应的像素，并标记此像素为 isNumbered
	for(i=0; i<tourNum; i++)
	{
		for(j=tours[i].start; j<=tours[i].end; j++)
		{
			fore[tours[i].rowNum*WIDTH+j].nConnRgn = tours[i].connNum;
			fore[tours[i].rowNum*WIDTH+j].isNumbered = true;
		}
	}

	//游程号和连通区域标号置零
	//tourNum = 0;
	//connNum = 0;
}


//计算矩形内容
void InitRectCalc(POINT* linePos, int enterType, _DetectRect* newRect)
{
	//赋值计算结果到矩形结构体
	int n0 = (enterType) % 4;
	int n1 = (enterType + 1) % 4;
	int n2 = (enterType + 2) % 4;
	int n3 = (enterType + 3) % 4;

	newRect->enterLine.p0.x = (double)(linePos[n0].x);
	newRect->enterLine.p0.y = (double)(linePos[n0].y);
	newRect->enterLine.p1.x = (double)(linePos[n1].x); 
	newRect->enterLine.p1.y = (double)(linePos[n1].y);

	newRect->leaveLine[0].p0.x = (double)(linePos[n1].x);
	newRect->leaveLine[0].p0.y = (double)(linePos[n1].y);
	newRect->leaveLine[0].p1.x = (double)(linePos[n2].x);
	newRect->leaveLine[0].p1.y = (double)(linePos[n2].y);

	newRect->leaveLine[1].p0.x = (double)(linePos[n2].x);
	newRect->leaveLine[1].p0.y = (double)(linePos[n2].y);
	newRect->leaveLine[1].p1.x = (double)(linePos[n3].x);
	newRect->leaveLine[1].p1.y = (double)(linePos[n3].y);

	newRect->leaveLine[2].p0.x = (double)(linePos[n3].x);
	newRect->leaveLine[2].p0.y = (double)(linePos[n3].y);
	newRect->leaveLine[2].p1.x = (double)(linePos[n0].x);
	newRect->leaveLine[2].p1.y = (double)(linePos[n0].y);

	newRect->corners[0].x = (double)linePos[0].x;
	newRect->corners[0].y = (double)linePos[0].y;
	newRect->corners[1].x = (double)linePos[1].x;
	newRect->corners[1].y = (double)linePos[1].y;
	newRect->corners[2].x = (double)linePos[2].x;
	newRect->corners[2].y = (double)linePos[2].y;
	newRect->corners[3].x = (double)linePos[3].x;
	newRect->corners[3].y = (double)linePos[3].y;

	newRect->isDrew = TRUE;
	newRect->enterType = enterType;
}


//计算旋转公式
void RotatePointCalc(_Point pSource, _Point* pResult, _Point center, long double angle){
	pResult->x = cos(angle) * (pSource.x - center.x) - sin(angle) * (pSource.y - center.y) + center.x;
	pResult->y = sin(angle) * (pSource.x - center.x) + cos(angle) * (pSource.y - center.y) + center.y;
}

//计算旋转后矩形四个角坐标
void RotateRectCalc(_DetectRect* rotateRect, int angleDu){
	long double angle;
	_Point centerPos;
	_DetectRect tmpRect;

	// 度数转为弧度制
	angle = (long double)angleDu * 3.141592653 / 180.0; 
	
	//计算矩形中心坐标（绕中心旋转）
	centerPos.x = (rotateRect->corners[0].x + rotateRect->corners[1].x
				  + rotateRect->corners[2].x + rotateRect->corners[3].x) / 4.0;
	centerPos.y = (rotateRect->corners[0].y + rotateRect->corners[1].y
				  + rotateRect->corners[2].y + rotateRect->corners[3].y) / 4.0;

	//计算旋转后坐标
	RotatePointCalc(rotateRect->enterLine.p0, &tmpRect.enterLine.p0, centerPos, angle);
	RotatePointCalc(rotateRect->enterLine.p1, &tmpRect.enterLine.p1, centerPos, angle);
	RotatePointCalc(rotateRect->leaveLine[0].p0, &tmpRect.leaveLine[0].p0, centerPos, angle);
	RotatePointCalc(rotateRect->leaveLine[0].p1, &tmpRect.leaveLine[0].p1, centerPos, angle);
	RotatePointCalc(rotateRect->leaveLine[1].p0, &tmpRect.leaveLine[1].p0, centerPos, angle);
	RotatePointCalc(rotateRect->leaveLine[1].p1, &tmpRect.leaveLine[1].p1, centerPos, angle);
	RotatePointCalc(rotateRect->leaveLine[2].p0, &tmpRect.leaveLine[2].p0, centerPos, angle);
	RotatePointCalc(rotateRect->leaveLine[2].p1, &tmpRect.leaveLine[2].p1, centerPos, angle);
	RotatePointCalc(rotateRect->corners[0], &tmpRect.corners[0], centerPos, angle);
	RotatePointCalc(rotateRect->corners[1], &tmpRect.corners[1], centerPos, angle);
	RotatePointCalc(rotateRect->corners[2], &tmpRect.corners[2], centerPos, angle);
	RotatePointCalc(rotateRect->corners[3], &tmpRect.corners[3], centerPos, angle);

	rotateRect->enterLine = tmpRect.enterLine;
	rotateRect->leaveLine[0] = tmpRect.leaveLine[0];
	rotateRect->leaveLine[1] = tmpRect.leaveLine[1];
	rotateRect->leaveLine[2] = tmpRect.leaveLine[2];
	rotateRect->corners[0] = tmpRect.corners[0];
	rotateRect->corners[1] = tmpRect.corners[1];
	rotateRect->corners[2] = tmpRect.corners[2];
	rotateRect->corners[3] = tmpRect.corners[3];
}


//计算矩形四条边斜率截距
void ABRectCalc(_Line* rectLine)
{
	double xDif;

	if ((rectLine->p0.x - rectLine->p1.x < 1.0) && (rectLine->p0.x - rectLine->p1.x >= 0.0))
	{
		xDif = 1.0;
		rectLine->p0.x += 1.0;
	}
	else if ((rectLine->p0.x - rectLine->p1.x < 0.0) && (rectLine->p0.x - rectLine->p1.x >= -1.0))
	{
		xDif = -1.0;
		rectLine->p0.x += -1.0;
	}
	else
	{
		xDif = rectLine->p0.x - rectLine->p1.x;
	}

	rectLine->a = (rectLine->p0.y - rectLine->p1.y) / xDif;
	rectLine->b = rectLine->p0.y - (rectLine->a * rectLine->p0.x);

	rectLine->maxX = max(rectLine->p0.x, rectLine->p1.x);
	rectLine->minX = min(rectLine->p0.x, rectLine->p1.x);
	rectLine->maxY = max(rectLine->p0.y, rectLine->p1.y);
	rectLine->minY = min(rectLine->p0.y, rectLine->p1.y);
}

//16个矩形修正
void TotalRectMend(_DetectRect* detectRectArray)
{
	int i;

	//计算矩形四条边线线段表达式
	for (i = 0; i <16; i ++)
	{
		if(detectRectArray[i].isDrew == TRUE)
		{
			ABRectCalc(&(detectRectArray[i].enterLine));		
			ABRectCalc(&(detectRectArray[i].leaveLine[0]));
			ABRectCalc(&(detectRectArray[i].leaveLine[1]));
			ABRectCalc(&(detectRectArray[i].leaveLine[2]));
		}

		detectRectArray[i].currentLeaveLine = -1;
		detectRectArray[i].delayCount = 0;
	}
}

// 绘制检测区域
void DrawDetectRect(_DetectRect dRect, int rectID, int drawMode)
{
	CRect picRect;
	CPoint enterP0, enterP1, leaveP0, leaveP1;
	double centerX, centerY;

	AfxGetApp()->m_pMainWnd->GetDlgItem(rectID)->GetWindowRect(&picRect);
	AfxGetApp()->m_pMainWnd->ScreenToClient(&picRect);
	CClientDC pDC((CFinalTestDlg *)AfxGetApp()->m_pMainWnd);

	dRect.enterLine.p0.x = dRect.enterLine.p0.x * (double)(picRect.Width()) / (double)WIDTH;
	dRect.enterLine.p0.y = dRect.enterLine.p0.y * (double)(picRect.Height()) / (double)HEIGHT;
	dRect.enterLine.p1.x = dRect.enterLine.p1.x * (double)(picRect.Width()) / (double)WIDTH;
	dRect.enterLine.p1.y = dRect.enterLine.p1.y * (double)(picRect.Height()) / (double)HEIGHT;
	dRect.leaveLine[1].p0.x = dRect.leaveLine[1].p0.x * (double)(picRect.Width()) / (double)WIDTH;
	dRect.leaveLine[1].p0.y = dRect.leaveLine[1].p0.y * (double)(picRect.Height()) / (double)HEIGHT;
	dRect.leaveLine[1].p1.x = dRect.leaveLine[1].p1.x * (double)(picRect.Width()) / (double)WIDTH;
	dRect.leaveLine[1].p1.y = dRect.leaveLine[1].p1.y * (double)(picRect.Height()) / (double)HEIGHT;
	
	//重新计算旋转后绝对坐标（对话框坐标）
	enterP0.x = (long)(dRect.enterLine.p0.x + picRect.left);
	enterP0.y = (long)(dRect.enterLine.p0.y + picRect.top);
	enterP1.x = (long)(dRect.enterLine.p1.x + picRect.left);
	enterP1.y = (long)(dRect.enterLine.p1.y + picRect.top);
	leaveP0.x = (long)(dRect.leaveLine[1].p0.x + picRect.left);
	leaveP0.y = (long)(dRect.leaveLine[1].p0.y + picRect.top);
	leaveP1.x = (long)(dRect.leaveLine[1].p1.x + picRect.left);
	leaveP1.y = (long)(dRect.leaveLine[1].p1.y + picRect.top);	

	centerX = (enterP0.x + enterP1.x + leaveP0.x + leaveP1.x) / 4.0 - 4.0;
	centerY = (enterP0.y + enterP1.y + leaveP0.y + leaveP1.y) / 4.0 - 6.0;

	//绘制框序号
	CFont*  pOldFont;
	CFont newFont;
	newFont.CreatePointFont(1, "Arial");
	pOldFont = pDC.SelectObject(&newFont);
	pDC.SetTextColor(RGB(0,0,255));
	CString txt;
	txt.Format("%d", dRect.rectNum);
	pDC.TextOut((int)centerX,(int)centerY,txt);
	
	//创建画笔
	CPen penY(PS_SOLID, 0, RGB(255,255,0));
	CPen penB(PS_SOLID, 0, RGB(0,0,255));
	CPen penG(PS_SOLID, 0, RGB(0,255,0));
	CPen penR(PS_DOT, 0, RGB(255,0,0));
	CPen penBX(PS_DOT, 0, RGB(0,0,255));
	CPen penGX(PS_DOT, 0, RGB(0,255,0));

	//无勾选
	if (drawMode == 0)
	{
		CPen* enterPen = pDC.SelectObject(&penB); 
		pDC.MoveTo(enterP0);
		pDC.LineTo(enterP1);
		
		CPen* leavePen = pDC.SelectObject(&penG); 
		pDC.LineTo(leaveP0);
		pDC.LineTo(leaveP1);
		pDC.LineTo(enterP0);
	}
	//有勾选
	else if (drawMode == 1)
	{
		CPen* enterPen = pDC.SelectObject(&penBX); 
		pDC.MoveTo(enterP0);
		pDC.LineTo(enterP1);
		
		CPen* leavePen = pDC.SelectObject(&penGX); 
		pDC.LineTo(leaveP0);
		pDC.LineTo(leaveP1);
		pDC.LineTo(enterP0);
	}
	else if (drawMode == 2)	// 画红框
	{
		CPen* enterPen = pDC.SelectObject(&penR); 
		pDC.MoveTo(enterP0);
		pDC.LineTo(enterP1);
		
		CPen* leavePen = pDC.SelectObject(&penG); 
		pDC.LineTo(leaveP0);
		pDC.LineTo(leaveP1);
		pDC.LineTo(enterP0);
	}
}

//平移检测框
void MoveDetectRect(_DetectRect* moveRect, int moveMode){
	if (moveMode == RECT_MOVE_LEFT){
		moveRect->corners[0].x --;
		moveRect->corners[1].x --;
		moveRect->corners[2].x --;
		moveRect->corners[3].x --;
		moveRect->enterLine.p0.x --;
		moveRect->enterLine.p1.x --;
		moveRect->leaveLine[0].p0.x --;
		moveRect->leaveLine[0].p1.x --;
		moveRect->leaveLine[1].p0.x --;
		moveRect->leaveLine[1].p1.x --;
		moveRect->leaveLine[2].p0.x --;
		moveRect->leaveLine[2].p1.x --;
	}
	else if (moveMode == RECT_MOVE_UP){
		moveRect->corners[0].y --;
		moveRect->corners[1].y --;
		moveRect->corners[2].y --;
		moveRect->corners[3].y --;
		moveRect->enterLine.p0.y --;
		moveRect->enterLine.p1.y --;
		moveRect->leaveLine[0].p0.y --;
		moveRect->leaveLine[0].p1.y --;
		moveRect->leaveLine[1].p0.y --;
		moveRect->leaveLine[1].p1.y --;
		moveRect->leaveLine[2].p0.y --;
		moveRect->leaveLine[2].p1.y --;
	}
	else if (moveMode == RECT_MOVE_RIGHT){
		moveRect->corners[0].x ++;
		moveRect->corners[1].x ++;
		moveRect->corners[2].x ++;
		moveRect->corners[3].x ++;
		moveRect->enterLine.p0.x ++;
		moveRect->enterLine.p1.x ++;
		moveRect->leaveLine[0].p0.x ++;
		moveRect->leaveLine[0].p1.x ++;
		moveRect->leaveLine[1].p0.x ++;
		moveRect->leaveLine[1].p1.x ++;
		moveRect->leaveLine[2].p0.x ++;
		moveRect->leaveLine[2].p1.x ++;
	}
	else if (moveMode == RECT_MOVE_DOWN){
		moveRect->corners[0].y ++;
		moveRect->corners[1].y ++;
		moveRect->corners[2].y ++;
		moveRect->corners[3].y ++;
		moveRect->enterLine.p0.y ++;
		moveRect->enterLine.p1.y ++;
		moveRect->leaveLine[0].p0.y ++;
		moveRect->leaveLine[0].p1.y ++;
		moveRect->leaveLine[1].p0.y ++;
		moveRect->leaveLine[1].p1.y ++;
		moveRect->leaveLine[2].p0.y ++;
		moveRect->leaveLine[2].p1.y ++;
	}
}

//显示检测框信息
void DispDetectRectInfo(int rectID){
	unsigned int enterWidth = 0;
	unsigned int leaveLength = 0;
	int xDist;
	int yDist;
	POINT corners[4];
	_DetectRect rect = detectRect[rectID-1];
	CString rectInfo;
	
	if (rect.isDrew)
	{
		xDist = (int)(rect.enterLine.p0.x - rect.enterLine.p1.x);
		yDist = (int)(rect.enterLine.p0.y - rect.enterLine.p1.y);
		enterWidth = (unsigned int)sqrt(xDist*xDist + yDist*yDist);
		xDist = (int)(rect.leaveLine[0].p0.x - rect.leaveLine[0].p1.x);
		yDist = (int)(rect.leaveLine[0].p0.y - rect.leaveLine[0].p1.y);
		leaveLength = (unsigned int)sqrt(xDist*xDist + yDist*yDist);
		for (int i = 0; i < 4; i++)
		{
			corners[i].x = (long)rect.corners[i].x;
			corners[i].y = (long)rect.corners[i].y;
		}
		
		CString line1;
		line1.Format("The enter line length of Rect_%d  is %d \r\n", rectID, enterWidth);
		CString line2;
		line2.Format("The rect detect depth of Rect_%d  is %d \r\n", rectID, leaveLength);
		CString line3;
		line3.Format("The four corners of the  Rect_%d  is (%d, %d) (%d, %d) (%d, %d) (%d, %d) \r\n", 
					 rectID, 
					 corners[0].x, corners[0].y, 
					 corners[1].x, corners[1].y,
					 corners[2].x, corners[2].y,
					 corners[3].x, corners[3].y);
		rectInfo = line1 + line2 + line3;
	}
	else{
		rectInfo.Format("Rect_%d  hasn't been draw!", rectID);
	}

	AfxMessageBox(rectInfo);
}


//创建人体模型的空列表
pplList CreatePeopleList()
{
	pplList L;
	L = (pplList)malloc(sizeof(pplModel));
	L->peopleNum = 0;
	L->nextPeople = NULL;
	return L;
}

//增加人体
void AddPeople(int peopleNum, double* histBins, unsigned long time, pplList L)
{
	int i;
	pplNode P, newPpl;
	
	//P指向表头
	P = L;

	//找到表的末尾位置
	while(P->nextPeople != NULL)
	{
		P = P->nextPeople;
	}

	//在链表的末尾添加新的人体模型
	newPpl = (pplNode)malloc(sizeof(pplModel));
	newPpl->peopleNum = peopleNum;

	for(i=0; i<4096; i++)
	{
		newPpl->histBins[i] = histBins[i];
	}
	newPpl->appearTime = time;
	newPpl->stayTime = 0;

	P->nextPeople = newPpl;
	newPpl->nextPeople = NULL;
}


//更新人体
void UpdatePeople(int peopleNum, double* histBins, unsigned long time, double speed, pplList L)
{
	int i;
	pplNode P;
	CString str;

	//若为空表，则退出
	if(L->nextPeople == NULL)
	{
		return;
	}

	P = L->nextPeople;

	while(P->peopleNum != peopleNum)
	{
		P = P->nextPeople;

		//若到达表尾，则退出
		if(P == NULL)
		{
			return;
		}
	}

	//更新概率直方图
	for(i=0; i<4096; i++)
	{
		P->histBins[i] = HIST_UPDATE_WEIGHT * P->histBins[i] + (1 - HIST_UPDATE_WEIGHT) * histBins[i];
	}

	//计算逗留时间
	P->stayTime = time - P->appearTime;
	if(P->stayTime > ABNORMAL_STAY_TIME)	//判断是否异常
	{
		str.Format("人%d逗留时间过长！", P->peopleNum);
		//AfxMessageBox(str.GetBuffer(0));
	}

	//保存当前速度
	P->speed = speed;

	if(P->speed > ABNORMAL_SPEED)
	{
		str.Format("人%d速度过快！", P->peopleNum);
		//AfxMessageBox(str.GetBuffer(0));
	}
}

//更新人体（更新时间、速度，不更新直方图）
void UpdatePeopleExHist(int peopleNum, unsigned long time, double speed, pplList L)
{
	pplNode P;
	CString str;
	
	//若为空表，则退出
	if(L->nextPeople == NULL)
	{
		return;
	}
	
	P = L->nextPeople;
	
	while(P->peopleNum != peopleNum)
	{
		P = P->nextPeople;
		
		//若到达表尾，则退出
		if(P == NULL)
		{
			return;
		}
	}
	
	//计算逗留时间
	P->stayTime = time - P->appearTime;
	if(P->stayTime > ABNORMAL_STAY_TIME)	//判断是否异常
	{
		str.Format("人%d逗留时间过长！", P->peopleNum);
		//AfxMessageBox(str.GetBuffer(0));
	}

	//保存当前速度
	P->speed = speed;
	if(P->speed > ABNORMAL_SPEED)
	{
		str.Format("人%d速度过快！", P->peopleNum);
		//AfxMessageBox(str.GetBuffer(0));
	}
}

//获取人体的直方图信息
void GetPeopleHist(int peopleNum, double* histBins, pplList L)
{
	int i;
	pplNode P;
	
	//若为空表，则退出
	if(L->nextPeople == NULL)
	{
		return;
	}
	
	P = L->nextPeople;
	
	while(P->peopleNum != peopleNum)
	{
		P = P->nextPeople;
		
		//若到达表尾，则退出
		if(P == NULL)
		{
			return;
		}
	}
	
	//更新概率直方图
	for(i=0; i<4096; i++)
	{
		histBins[i] = P->histBins[i];
	}
}

//删除人体
void DeletePeople(int peopleNum, pplList L)
{
	pplNode P, tmpNode;

	//若此表为空表，则退出函数
	if(L->nextPeople == NULL)
	{
		return;
	}

	//P指向表头
	P = L;

	//找到含有此 peopleNum 的表项的前一项 P
	while((P->nextPeople)->peopleNum != peopleNum)
	{
		//若未找到，则指针移到下一项
		P = P->nextPeople;
		
		//如果已经找到表尾，则退出程序
		if(P->nextPeople == NULL)
		{
			return;
		}
	}

	//删除此表项
	tmpNode = P->nextPeople;
	P->nextPeople = tmpNode->nextPeople;
	free(tmpNode);
}

//打印人体
void PrintPeopleList(pplList L)
{
	CString str, tmp;
	pplNode P;

	str = "P：";

	//若为空表，则退出
	if(L->nextPeople == NULL)
	{	
		return;
	}

	P = L->nextPeople;

	//遍历每一个人体模型，并显示其人体编号
	while(P != NULL)
	{
		tmp.Format("%d", P->peopleNum);
		str += tmp;
		P = P->nextPeople;
	}
		
	//显示所有的人体编号
	AfxMessageBox(str.GetBuffer(0));
}

//中值滤波
void MiddleFilter(foreImg* img, int filterH, int filterW, int filterMX, int filterMY)
{
	int i, j, k, l;
	char nTrue, nFalse;
	
	//行（除去边缘几行）
	for(i=filterMY; i<HEIGHT-filterH+filterMY+1; i++)
	{
		//列（除去边缘几列）
		for(j=filterMX; j<WIDTH-filterW+filterMX+1; j++)
		{
			nTrue = 0;
			nFalse = 0;
			
			//进行中值滤波
			for(k=0; k<filterH; k++)
			{
				for(l=0; l<filterW; l++)
				{
					//分别统计前景和背景的个数
					if(img[(i-filterMY+k)*WIDTH+j-filterMX+l].isFore)
					{
						nTrue++;
						if(nTrue > 12)
						{
							img[i*WIDTH+j].isFore = true;
							goto next;
						}
					} 
					else
					{
						nFalse++;
						if(nFalse > 12)
						{
							img[i*WIDTH+j].isFore = false;
							goto next;
						}
					}

				}//l
			}//k
			next:;
			//img[i*WIDTH+j].isFore = (nTrue > nFalse)? true: false;
		}//j
	}//i
	
	//将边缘设置为背景
	for(i=0; i<HEIGHT; i++)
	{
		for(j=0; j<WIDTH; j++)
		{
			if(i<filterH || i>=(HEIGHT-filterH) || j<filterW || j>=(WIDTH - filterW))
			{
				img[i*WIDTH+j].isFore = false;
			}//if
		}//j
	}//i
}

//去除阴影
void RemoveShadow(foreImg* fore)
{
	int i, j;
	
	int hei[WIDTH];
	for(i=0; i<WIDTH; i++)
	{
		hei[i] = 0;
	}
	
	//统计各列的前景像素数
	for(j=0; j<WIDTH; j++)
	{
		for(i=0; i<HEIGHT; i++)
		{
			if(fore[i*WIDTH+j].isFore)
			{
				hei[j]++;
			}
		}
	}
	
	//找到最大值
	int maxHeight = 0;
	for(j=0; j<WIDTH; j++)
	{
		if(hei[j] > maxHeight)
		{
			maxHeight = hei[j];
		}
	}
	
	//去除前景像素数小于阈值的列
	for(j=0; j<WIDTH; j++)
	{
		if(hei[j] < 0.25 * maxHeight)
		{
			for(i=0; i<HEIGHT; i++)
			{
				fore[i*WIDTH +j].isFore = false;
			}
		}
	}	
}

// 计算某人体模型与某连通区域的匹配系数
double MatchPeopleToConn(int peopleNum, int nConnRgn, int nPixel, pplList L, foreImg* fore)
{
	int i;

	static double pplHistBins[4096];
	static double connHistBins[4096];

	double minHistSum = 0.0;
	double pplHistSum = 0.0;

	// 获取人体直方图模型
	GetPeopleHist(peopleNum, pplHistBins, L);

	// 获取连通区域的直方图模型
	CreateHistBins(connHistBins, nConnRgn, nPixel, fore);

	for(i=0; i<4096; i++)
	{
		minHistSum += ((pplHistBins[i] < connHistBins[i])? pplHistBins[i]: connHistBins[i]);
		pplHistSum += pplHistBins[i];
	}

	return (minHistSum / pplHistSum);
}

// 背景学习
void BgLearn(IplImage* pFrame, int frameNum, int watershed, double* mean, double* var)
{
	int i, j;
	static long int sum[PIXEL_BYTES_NUM];		// RGB累加和
	static long int squSum[PIXEL_BYTES_NUM];	// RGB平方和

	// 初始化
	if(frameNum == 1)
	{
		for(i=0; i<PIXEL_BYTES_NUM; i++)
		{
			sum[i] = 0;
			squSum[i] = 0;
		}	
	}

	// 累加
	for(j=0; j<PIXEL_BYTES_NUM; j++)
	{
		sum[j] += (*pFrame).imageData[j];
		squSum[j] += (*pFrame).imageData[j] * (*pFrame).imageData[j];
	}
	
	// 计算均值和方差
	if(frameNum == watershed)
	{
		for(i=0; i<PIXEL_BYTES_NUM; i++)
		{
			mean[i] = sum[i] / (double)watershed;
			var[i] = sqrtl(squSum[i] / (double)watershed - mean[i] * mean[i]);
		}
	}

	// 显示
	ShowImg(pFrame, IDC_PICTURE_A);
	drawAllRects(pFrame);
	ShowImg(pFrame, IDC_PICTURE_B);
}

//背景差分与更新，以提取前景
void BgDiff(IplImage* pFrame, double* mean, double* var, foreImg* fore) 
{
	int i;
	
	//差值
	double diffB, diffG, diffR;
	
	//开始背景差分与背景更新
	for(i=0; i<PIXEL_NUM; i++)
	{
		//进行背景差分
		diffB = fabsl((*pFrame).imageData[i*3] - mean[i*3]);
		bool isBFore = (diffB > 3 * var[i*3]);
		if(isBFore)
		{
			//标记为前景，并赋R、G、B值
			fore[i].isFore = true;
			fore[i].bValue = (*pFrame).imageData[i*3];
			fore[i].gValue = (*pFrame).imageData[i*3+1];
			fore[i].rValue = (*pFrame).imageData[i*3+2];
		}
		else
		{
			diffG = fabsl((*pFrame).imageData[i*3+1] - mean[i*3+1]);
			bool isGFore = (diffG > 3 * var[i*3+1]);
			if(isGFore)
			{
				//标记为前景，并赋R、G、B值
				fore[i].isFore = true;
				fore[i].bValue = (*pFrame).imageData[i*3];
				fore[i].gValue = (*pFrame).imageData[i*3+1];
				fore[i].rValue = (*pFrame).imageData[i*3+2];
			}
			else
			{
				diffR = fabsl((*pFrame).imageData[i*3+2] - mean[i*3+2]);
				bool isRFore = (diffR > 3 * var[i*3+2]);
				if(isRFore)
				{
					//标记为前景，并赋R、G、B值
					fore[i].isFore = true;
					fore[i].bValue = (*pFrame).imageData[i*3];
					fore[i].gValue = (*pFrame).imageData[i*3+1];
					fore[i].rValue = (*pFrame).imageData[i*3+2];
				}
				else	//若为背景像素，则进行背景更新
				{
					//更新均值
					mean[i*3] = BG_UPDATE_WEIGHT * mean[i*3] + (1 - BG_UPDATE_WEIGHT) * (*pFrame).imageData[i*3];
					mean[i*3+1] = BG_UPDATE_WEIGHT * mean[i*3+1] + (1 - BG_UPDATE_WEIGHT) * (*pFrame).imageData[i*3+1];
					mean[i*3+2] = BG_UPDATE_WEIGHT * mean[i*3+2] + (1 - BG_UPDATE_WEIGHT) * (*pFrame).imageData[i*3+2];
				}
			}
		}
	}
}

//创建色彩的概率直方图容器
void CreateHistBins(double* histBins, int nConnRgn, int nPixel, foreImg* fore) 
{
	int i;
	
	for(i=0; i<4096; i++)
	{
		histBins[i] = (double)0;
	}
	
	//计算颜色直方图
	for(i=0; i<PIXEL_NUM; i++)
	{
		if((fore[i].nConnRgn == nConnRgn) && (fore[i].isFore))
		{
			int index = (fore[i].rValue / 16) * 256 + (fore[i].gValue / 16) * 16 + (fore[i].bValue / 16) * 1;
			histBins[index]++;
		}
	}
	
	//计算概率直方图
	for(i=0; i<4096; i++)
	{
		histBins[i] /= nPixel;
	}
}

//判断匹配类型
int GetMatchType(int matchTag, pplConn* prePeos, pplConn* currPeos) 
{
	int i;
	
	//分别计算上一帧和当前帧中的匹配区域的个数
	int preMatchNum = 0; 
	int currMatchNum = 0;
	int matchType = 0;
	
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(prePeos[i].matchTag == matchTag)
		{
			preMatchNum++;
		}
		if(currPeos[i].matchTag == matchTag)
		{
			currMatchNum++;
		}
	}
	
	//1对1
	if((preMatchNum == 1) && (currMatchNum == 1))
	{
		matchType = MATCH_TYPE_1T1;
	}
	else if((preMatchNum == 1) && (currMatchNum > 1))	//1对多
	{
		matchType = MATCH_TYPE_1TM;
	} 
	else if((preMatchNum > 1) && (currMatchNum == 1))	//多对1
	{
		matchType = MATCH_TYPE_MT1;
	}
	
	return matchType;
}

//人体匹配
void MatchPeople(pplConn* prePeos, pplConn* currPeos, foreImg* fore, pplList pList) 
{
	int i, j, k, l, m;
    int matchType;
	int matchTag = 1;
	static int time1 = 0;
	static int time2 = 0;
	bool isEntered;
	static double histBins[4096];

	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;

	//进行前后图像帧中对应于人体的连通区域的匹配
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(currPeos[i].isPeople)
		{
			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				if(prePeos[j].isPeople)
				{
					//当前帧中第i个人体区域和上一帧中第j个人体区域进行匹配
					
					//计算边界框的重叠面积
					int lapLeft = (currPeos[i].left > prePeos[j].left)? currPeos[i].left: prePeos[j].left;
					int lapRight = (currPeos[i].right < prePeos[j].right)? currPeos[i].right: prePeos[j].right;
					//int lapUp = (currPeos[i].up > prePeos[j].up)? currPeos[i].up: prePeos[j].up;
					//int lapDown = (currPeos[i].down < prePeos[j].down)? currPeos[i].down: prePeos[j].down;
					//int lapArea = (lapRight - lapLeft) * (lapDown - lapUp);
					
					//计算匹配系数
					//double matchFactor = (double)lapArea / ((currPeos[i].area < prePeos[j].area)? currPeos[i].area: prePeos[j].area);
					
					int preLen = currPeos[i].right - currPeos[i].left;
					int currLen = prePeos[j].right - prePeos[j].left;
					double matchFactor = (double)(lapRight - lapLeft) / ((preLen < currLen)? preLen: currLen);

					/*
					double minHistSum = 0.0;
					double pplHistSum = 0.0;
					//double currHistSum = 0.0;
					
					for(k=0; k<4096; k++)
					{
						minHistSum += ((currPeos[i].histBins[k] < prePeos[j].histBins[k])? currPeos[i].histBins[k]: prePeos[j].histBins[k]);
						pplHistSum += currPeos[i].histBins[k];
					}
					
					double matchFactor = (minHistSum / pplHistSum);

					CString str;
					str.Format("当前区域%d和上一区域%d:%f", i, j, matchFactor);
					AfxMessageBox(str.GetBuffer(0));
					*/

					//若匹配
					if(matchFactor > THRESHOLD_MATCH)
					{
						//如果发现当前帧的此区域已匹配，则
						if(currPeos[i].matchTag != 0)
						{
							prePeos[j].matchTag = currPeos[i].matchTag;
						}
						else if(prePeos[j].matchTag != 0)
						{
							currPeos[i].matchTag = prePeos[j].matchTag;
						}
						else if((prePeos[j].matchTag == 0) && (currPeos[i].matchTag == 0))	// 若都没有匹配
						{
							prePeos[j].matchTag = matchTag;
							currPeos[i].matchTag = matchTag;
							matchTag++;
						}

						//判断移动方向
						double moveLength = ((currPeos[i].left + currPeos[i].right) / (double)2) - ((prePeos[j].left + prePeos[j].right) / (double)2);
						if(moveLength <= 0)
						{
							currPeos[i].direction = prePeos[i].direction;
							currPeos[i].direction += 1;
						} 
						else
						{
							currPeos[i].direction = prePeos[i].direction;
							currPeos[i].direction -= 1;
						}

						//计算时间
						currPeos[i].speed = fabsl(moveLength / (timeGetTime() - prePeos[i].time) * 1000);
					}	
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// 处理3大类情况：多对1、1对多、1对1

	for(i=1; i<matchTag; i++)
	{
		matchType = GetMatchType(i, prePeos, currPeos);
		
		//1对1
		switch(matchType)
		{
		case MATCH_TYPE_1T1:
			//找到上一帧中匹配标号为 i 的区域
			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				if(prePeos[j].matchTag == i)
				{
					//找到当前帧中匹配标号为 i 的区域
					for(k=0; k<MAX_TRACKING_PEOPLE; k++)
					{
						if(currPeos[k].matchTag == i)
						{
							//若为群组的1对1匹配
							if(prePeos[j].isGroup)	
							{
								currPeos[k].isGroup = true;
								currPeos[k].groupNum = prePeos[j].groupNum;
								for(l=0; l<MAX_TRACKING_PEOPLE; l++)
								{
									currPeos[k].peopleNum[l] = prePeos[j].peopleNum[l];
								}
								
								//更新人体的逗留时间和速度
								for(l=0; l<MAX_TRACKING_PEOPLE; l++)
								{
									if(currPeos[k].peopleNum[l] != 0)
									{
										UpdatePeopleExHist(currPeos[k].peopleNum[l], currPeos[k].time, currPeos[k].speed, pList);
									}
								}
							}
							else	//若为单个人体的1对1匹配
							{
								currPeos[k].peopleNum[0] = prePeos[j].peopleNum[0];

								//更新人体的概率直方图模型
								CreateHistBins(histBins, currPeos[k].nConnRgn, currPeos[k].nPixel, fore);
								UpdatePeople(currPeos[k].peopleNum[0], histBins, currPeos[k].time, currPeos[k].speed, pList);
							}
						}
					}
				}
			}	
			break;

		case MATCH_TYPE_MT1:	//多对1

			//AfxMessageBox("形成群组");

			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				//找到当前帧中形成的群组区域，新建群组模型
				if(currPeos[j].matchTag == i)
				{
					//标记为群组，并赋予新的群组编号
					currPeos[j].isGroup = true;
					currPeos[j].groupNum = groupNum;

					m = 0;

					//将上一帧子群组中的人体编号
					//全部汇聚到新形成的群组中
					for(k=0; k<MAX_TRACKING_PEOPLE; k++)
					{
						if(prePeos[k].matchTag == i)
						{
							for(l=0; l<MAX_TRACKING_PEOPLE; l++)
							{
								if(prePeos[k].peopleNum[l] != 0)
								{
									currPeos[j].peopleNum[m] = prePeos[k].peopleNum[l];
									m++;
								}
							}
						}
					}

					groupNum++;
				}
			}
			break;

		case MATCH_TYPE_1TM:	//1对多
			
			//找到上一帧中分裂的群组区域
			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				if(prePeos[j].matchTag == i)
				{	
					//使用 直方图交叉匹配法 将人体模型对应到这些子群组中
					for(k=0; k<MAX_TRACKING_PEOPLE; k++)
					{
						if(prePeos[j].peopleNum[k] != 0)
						{
							double matchValue = 0.0;
							double max = 0.0;
							int maxConnIndex;	//匹配值最大的区域的数组索引

							for(l=0; l<MAX_TRACKING_PEOPLE; l++)
							{
								if(currPeos[l].matchTag == i)
								{									
									//计算编号为 prePeos[j].peopleNum[k] 的人体模型
									//与编号为 currPeos[l].nConnRgn 的连通区域的匹配值
									matchValue = MatchPeopleToConn(prePeos[j].peopleNum[k], currPeos[l].nConnRgn, currPeos[l].nPixel, pList, fore);
									
									if(matchValue > max)
									{
										max = matchValue;
										maxConnIndex = l;
									}
								}
							}

							//将此人体对应到索引为 l 的区域

							//若此区域还没有人体与之对应
							if(currPeos[maxConnIndex].peopleNum[0] == 0)
							{
								currPeos[maxConnIndex].peopleNum[0] = prePeos[j].peopleNum[k];
							}
							else //若此区域已有人体与之对应
							{
								currPeos[maxConnIndex].isGroup = true;
								for(m=0; m<MAX_TRACKING_PEOPLE; m++)
								{
									if(currPeos[maxConnIndex].peopleNum[m] == 0)
									{
										currPeos[maxConnIndex].peopleNum[m] = prePeos[j].peopleNum[k];
										break;
									}
								}
							}
						}
					}//k，上一帧某群组中的每一个人体编号


					//若子群组中有群组，则初始化新的群组模型
					for(k=0; k<MAX_TRACKING_PEOPLE; k++)
					{
						if(currPeos[k].matchTag == i)
						{
							if(currPeos[k].isGroup)
							{
								//初始化群组模型
								currPeos[k].groupNum = groupNum;
								groupNum++;
							}
						}
					}
					
				}
			}//j，上一帧中的群组
			break;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// 处理特殊的2类情况：1对0、0对1

	//找到上一帧中未匹配的区域
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(prePeos[i].isPeople)
		{
			//若出现未匹配的现象，则触发事件：有人从场景中消失
			if(prePeos[i].matchTag == 0)
			{
				if(prePeos[i].isGroup)
				{
					//删除群组中包含的人体
					for(j=0; j<MAX_TRACKING_PEOPLE; j++)
					{
						if(prePeos[i].peopleNum[j] != 0)
						{
							DeletePeople(prePeos[i].peopleNum[j], pList);
						}
					}
				} 
				else
				{
					//删除此人
					DeletePeople(prePeos[i].peopleNum[0], pList);
				}
			}
		}
	}

	//找到当前帧中未匹配的区域
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(currPeos[i].isPeople)
		{
			if(currPeos[i].matchTag == 0)
			{				
				//为此区域赋予新的人体编号
				currPeos[i].peopleNum[0] = peopleNum;

				//增加人体模型
				CreateHistBins(histBins, currPeos[i].nConnRgn, currPeos[i].nPixel, fore);
				AddPeople(currPeos[i].peopleNum[0], histBins, currPeos[i].time, pList);
				peopleNum++;
			}
		}
	}

	// 区域检测
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(currPeos[i].isPeople)
		{
			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				if(prePeos[j].isPeople
					&& prePeos[j].matchTag == currPeos[i].matchTag)
					
				{
					// 上一帧此人未进入区域，
					if(prePeos[j].areaState == PEOPLE_NOTIN)
					{
						if (isRectConfirmed)
						{
							isEntered = checkEnterRect(detectRect[finalRectNum], &currPeos[i]);
						}
						else
						{
							isEntered = checkEnterMultiRect(&currPeos[i]);
						}

						// 这一帧此人进入区域
						if (isEntered)
						{
							currPeos[i].areaState = PEOPLE_IN;
							
							// 显示跟踪结果
							CEdit* pEdit = (CEdit*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_EDIT_RESULT + finalRectNum);
							pEdit->GetWindowText(originText);
							time1 = (int)(g_frameNum * g_frameTimeGap);
							newText.Format("P%d enters at 00:%02d:%02d:%02d\r\n", currPeos[i].peopleNum[0], time1 / 3600, time1 / 60, time1 % 60);
							//if (firstFlag != 0)
							{
								//newText2.Format("Gap time 00:%02d:%02d:%02d\r\n\r\n", (time1 - time2) / 3600, (time1 - time2) / 60, (time1 - time2) % 60);
								//pEdit->SetWindowText(originText +  newText2 + newText);
							}
							//else
							{
								pEdit->SetWindowText(originText + newText);
							}
						}
						// 这一帧此人仍未进入区域
						else	
						{
							currPeos[i].areaState = PEOPLE_NOTIN;	// 保持未进入状态
						}
					}
					// 上一帧此人已进入区域，
					else if(prePeos[j].areaState == PEOPLE_IN)
					{
						// 这一帧此人离开区域
						if(checkLeaveRect(detectRect[finalRectNum], &currPeos[i]))
						{
							//firstFlag = 1;
							currPeos[i].areaState = PEOPLE_OUT;
							
							// 显示跟踪结果
							CEdit* pEdit = (CEdit*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_EDIT_RESULT + finalRectNum);
							pEdit->GetWindowText(originText);
							time2 = (int)(g_frameNum * g_frameTimeGap);
							//newText.Format("P%d leaves at 00:%02d:%02d:%02d\r\n\r\n", currPeos[i].peopleNum[0], time2 / 3600, time2 / 60, time2 % 60);
							newText.Format("P%d leaves at 00:%02d:%02d:%02d\r\n", currPeos[i].peopleNum[0], time2 / 3600, time2 / 60, time2 % 60);
							pEdit->SetWindowText(originText + newText);
						}
						// 这一帧此人仍在区域中
						else
						{
							currPeos[i].areaState = PEOPLE_IN;	// 保持进入状态
						}
					}
					// 若上一帧此人已经离开区域，则保持离开状态
					else if(prePeos[j].areaState == PEOPLE_OUT)
					{
						currPeos[i].areaState = PEOPLE_OUT;
					}
				}
			}
		}
	}

	//当前帧的数据覆盖上一帧的数据
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		prePeos[i].up = currPeos[i].up;
		prePeos[i].down = currPeos[i].down;
		prePeos[i].left = currPeos[i].left;
		prePeos[i].right = currPeos[i].right;
		prePeos[i].area = currPeos[i].area;
		prePeos[i].nPixel = currPeos[i].nPixel;
		
		prePeos[i].isPeople = currPeos[i].isPeople;
		prePeos[i].isGroup = currPeos[i].isGroup;
		
		for(j=0; j<MAX_TRACKING_PEOPLE; j++)
		{
			prePeos[i].peopleNum[j] = currPeos[i].peopleNum[j];
		}
		prePeos[i].groupNum = currPeos[i].groupNum;
		
		for(j=0; j<4096; j++)
		{
			prePeos[i].histBins[j] = currPeos[i].histBins[j];
		}
		
		prePeos[i].matchTag = 0;
		
		prePeos[i].direction = currPeos[i].direction;
		
		prePeos[i].time = currPeos[i].time;
		
		prePeos[i].speed = currPeos[i].speed;

		prePeos[i].areaState = currPeos[i].areaState;
	}
}

// 判断人体框是否第一次进入某检测框
bool checkEnterRect(_DetectRect rect, pplConn* people)
{
	float i;

	if(!rect.isDrew){
		return false;
	}

	if((people->direction > 0 && rect.enterType == 3)
		|| (people->direction < 0 && rect.enterType == 1))
	{
		return false;
	}

	// 线段和人体左边界相交
	for (i=0.0f; i<8; i+=0.01f)
	{
		if (people->direction > 0)
		{
			if(((people->left + i) >= rect.enterLine.minX)
				&& ((people->left + i) <= rect.enterLine.maxX)
				&& ((rect.enterLine.a * (people->left + i) + rect.enterLine.b) >= people->up)
				&& ((rect.enterLine.a * (people->left + i) + rect.enterLine.b) <= people->down))
			{
				return true;
			}
		}
		else
		{
			if(((people->right - i) >= rect.enterLine.minX)
				&& ((people->right - i) <= rect.enterLine.maxX)
				&& ((rect.enterLine.a * (people->right - i) + rect.enterLine.b) >= people->up)
				&& ((rect.enterLine.a * (people->right - i) + rect.enterLine.b) <= people->down))
			{
				return true;
			}
		}
	}

	return false;
}

// 判断人体是否进入所有的检测框中的某个
// 若检测到进入，则将此检测框的编号存入全局变量中
bool checkEnterMultiRect(pplConn* people)
{
	int i;
	
	// 遍历所有节点
	for(i=0; i<16; i++)
	{
		if(checkEnterRect(detectRect[i], people))
		{
			finalRectNum = i;
			isRectConfirmed = true;
			return true;
		}
	}

	return false;
}

// 判断人是否离开某线段
bool checkLeaveLine(_Line line, pplConn* people)
{
	// 和人体的右边界不相交，而且右边界在线段的左边
	if (people->direction > 0)
	{
		return (((people->right <= line.minX)
			|| (people->right >= line.maxX)
			|| ((line.a * people->right + line.b) <= people->up)
			|| ((line.a * people->right + line.b) >= people->down)) && (people->right < line.minX));
	} 
	else
	{
		return (((people->left <= line.minX)
			|| (people->left >= line.maxX)
			|| ((line.a * people->left + line.b) <= people->up)
			|| ((line.a * people->left + line.b) >= people->down)) && (people->left > line.maxX));
	}
}

// 计算人体右边界和线段的最小距离
double calDistance(_Line line, pplConn* people)
{
	double distanceA = 0.0;
	double distanceB = 0.0;
	
	distanceA = fabs(line.a * (people->right - 0.5) - people->up + line.b) / sqrt(line.a * line.a + 1);
	distanceB = fabs(line.a * (people->right - 0.5) - people->down + line.b) / sqrt(line.a * line.a + 1);

	return min(distanceA, distanceB);
}

// 判断人是否离开某检测框
bool checkLeaveRect(_DetectRect rect, pplConn* people)
{
	int i;
	int leaveCount = 0;
	double distance = 0.0;
	double minDistance = 10000;
	int minIndex = 0;

	for(i=0; i<3; i++)
	{
		if(checkLeaveLine(rect.leaveLine[i], people))
		{
			leaveCount++;
		}
	}

	if (leaveCount == 3)	// 离开3条线段，表示完全离开检测框，计时开始
	{
		for(i=0; i<3; i++)
		{
			distance = calDistance(detectRect[finalRectNum].leaveLine[i], people);
			if(distance <= minDistance)
			{
				minIndex = i;
				minDistance = distance;
			}
		}
		
		detectRect[finalRectNum].currentLeaveLine = minIndex;
		detectRect[finalRectNum].delayCount = 2;
		return true;
	}
	else
	{
		return false;
	}
}
  
//提取连通区域的信息
void GetConnsInfo(foreImg* fore, connRgn* conns)
{
	int i, j;

	//初始化当前帧的连通区域的信息
	for(i=0; i<MAX_CONN_RGN; i++)
	{
		conns[i].nPixel = 0;
		conns[i].left = WIDTH - 1;
		conns[i].right = 0;
		conns[i].up = HEIGHT - 1;
		conns[i].down = 0;
	}
	
	//为结构体数组填充信息
	for(i=0; i<HEIGHT; i++)
	{
		for(j=0; j<WIDTH; j++)
		{
			//若此像素被标记
			if(fore[i*WIDTH+j].isNumbered)
			{
				//获取连通区域的编号
				char index = fore[i*WIDTH+j].nConnRgn;
				
				//累加像素数
				conns[index].nPixel++;
				
				//赋予连通区域的编号
				conns[index].nConnRgn = index;
				
				//更新边界值
				if(i < conns[index].up)		//上边界
				{
					conns[index].up = i;
				}
				if(i > conns[index].down)	//下边界
				{
					conns[index].down = i;
				}
				if(j < conns[index].left)	//左边界
				{
					conns[index].left = j;
				}
				if(j > conns[index].right)	//右边界
				{
					conns[index].right = j;
				}
			}
		}//列
	}//行
}

//提取连通区域的信息
void GetConnsInfoByTour(struct tour* tours, struct connRgn* conns)
{
	int i;
	
	//初始化当前帧的连通区域的信息
	for(i=0; i<MAX_CONN_RGN; i++)
	{
		conns[i].nPixel = 0;
		conns[i].left = WIDTH - 1;
		conns[i].right = 0;
		conns[i].up = HEIGHT - 1;
		conns[i].down = 0;
	}
	
	for(i=0; i<tourNum; i++)
	{
		int index = tours[i].connNum;
		
		conns[index].nPixel += (tours[i].end - tours[i].start + 1);
		
		conns[index].nConnRgn = index;
		
		//更新边界值
		if(tours[i].rowNum < conns[index].up)	//上边界
		{
			conns[index].up = tours[i].rowNum;
		}
		if(tours[i].rowNum > conns[index].down)	//下边界
		{
			conns[index].down = tours[i].rowNum;
		}
		if(tours[i].start < conns[index].left)	//左边界
		{
			conns[index].left = tours[i].start;
		}
		if(tours[i].end > conns[index].right)	//右边界
		{
			conns[index].right = tours[i].end;
		}
	}
}

//提取人体区域的信息
void GetPeopleConns(connRgn* conns, pplConn* currPeos, foreImg* fore) 
{
	int i, j, k;
	
	//标记为人体区域，计算面积
	for(i=0,j=0; i<MAX_CONN_RGN; i++)
	{
		//若像素数目大于阈值，则对应为人体
		if(conns[i].nPixel > THRESHOLD_HUMAN_PIXEL_NUM)
		{
			currPeos[j].isPeople = true;
			currPeos[j].up = conns[i].up;
			currPeos[j].down = conns[i].down;
			currPeos[j].left = conns[i].left;
			currPeos[j].right = conns[i].right;
			currPeos[j].nConnRgn = conns[i].nConnRgn;
			currPeos[j].nPixel = conns[i].nPixel;
			currPeos[j].time = timeGetTime();
			
			//计算边界框面积
			currPeos[j].area = (conns[i].down - conns[i].up) * (conns[i].right - conns[i].left);
			
			/* author:hustlzp date:2012/7/3 */
			//计算每一个区域的色彩直方图概率模型
			for(k=0; k<4096; k++)
			{
				currPeos[j].histBins[k] = (double)0;
			}
			
			//计算颜色直方图
			for(k=0; k<PIXEL_NUM; k++)
			{
				if((fore[k].nConnRgn == currPeos[j].nConnRgn) && (fore[k].isFore))
				{
					int index = (fore[k].rValue / 16) * 256 + (fore[k].gValue / 16) * 16 + (fore[k].bValue / 16) * 1;
					currPeos[j].histBins[index]++;
				}
			}
			
			//计算概率直方图
			for(k=0; k<4096; k++)
			{
				currPeos[j].histBins[k] /= currPeos[j].nPixel;
			}
			/*end*/

			j++;
		}
	}
}

//获取时间差值（单位：us）
int GetTimeDiff(LARGE_INTEGER t2, LARGE_INTEGER t1, LARGE_INTEGER tc) 
{
	return (int)((double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart * 1000000);
}

//获取静止背景与动态背景的分水岭帧数
int GetFrameWaterShed(int videoType)
{
	int waterShed;
	
	switch(videoType)
	{
		case VIDEO_TYPE_1P:
			waterShed = 300;
			break;
		case VIDEO_TYPE_2P:
			waterShed = 200;
			break;
		case VIDEO_TYPE_3P:
			waterShed = 339;
			break;
	}

	return waterShed;
}

//获取视频文件地址
CString GetFilePath(int videoType)
{
	//CString rootPath("");
	CString filePath;

	char pBuf[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pBuf);
	CString rootPath(pBuf);
	rootPath += "\\";
	
	switch(videoType)
	{
	case VIDEO_TYPE_1P:
		{
			filePath = rootPath + "1P\\";
			break;
		}
	case VIDEO_TYPE_2P:
		{
			filePath = rootPath + "2P\\";
			break;
		}
	case VIDEO_TYPE_3P:
		{
			filePath = rootPath + "3P\\";
			break;
		}
	}
	
	return filePath;
}

//从某点开始标记此连通区域
void label(foreImg* fore, int i, int j, int nConnNum)
{
	int k, l;
	for(k=0; k<3; k++)
	{
		for(l=0; l<3; l++)
		{
			//若超出边界，则返回
			if((i-1+k) < 0 || (i-1+k)>= HEIGHT || (j-1+l) < 0 || (j-1+l) >= WIDTH)
				break; 
			//标记中心点
			if((k == 1) && (l == 1))
			{
				fore[(i-1+k)*WIDTH+j-1+l].nConnRgn = nConnNum;
				fore[(i-1+k)*WIDTH+j-1+l].isNumbered = true;
			}
			else if(fore[(i-1+k)*WIDTH+j-1+l].isFore && (fore[(i-1+k)*WIDTH+j-1+l].isNumbered == false))
			{
				//开始下一轮标记
				label(fore, i-1+k, j-1+l, nConnNum);
			}
		}
	}
}

//标记算法
void LabelConnRgn(foreImg* fore)
{
	int i, j;
	int nConnNum = 0;

	for(i=0; i<HEIGHT; i++)
	{
		for(j=0; j<WIDTH; j++)
		{
			//若此点为前景且未被标记，则开始此次标记
			if(fore[i*WIDTH+j].isFore && (fore[i*WIDTH+j].isNumbered == false))
			{
				label(fore, i, j, nConnNum);
				nConnNum++;
			}
		}
	}
}

//绘制前景区域到 IplImage 结构中
void DrawForeImg(foreImg * fore, IplImage* pForeImg)
{
	int i;
	
	//绘制连通区域
	for(i=0; i<PIXEL_BYTES_NUM; i+=3)
	{
		//if(fore[i/3].isFore && fore[i/3].isNumbered)
		if(fore[i/3].isFore)
		{
			(*pForeImg).imageData[i] = (char)255;
			(*pForeImg).imageData[i+1] = (char)255;
			(*pForeImg).imageData[i+2] = (char)255;
		}
	}
}

//绘制某个人体的前景连通区域
void DrawPeopleForeImg(foreImg * fore, IplImage* pForeImg, char nConnRgn)
{
	int i;
	
	//绘制连通区域
	for(i=0; i<PIXEL_BYTES_NUM; i+=3)
	{
		if(fore[i/3].nConnRgn == nConnRgn && fore[i/3].isNumbered)
		{
			(*pForeImg).imageData[i] = (char)255;
			(*pForeImg).imageData[i+1] = (char)255;
			(*pForeImg).imageData[i+2] = (char)255;
		}
	}
}

//绘制人体编号
void DrawPeopleNum(IplImage* pForeImg, int peopleNum, int left, int bottom) 
{
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1);
	CString str;
	str.Format("P%d", peopleNum);
	cvPutText(pForeImg, str.GetBuffer(0), cvPoint(left, bottom), &font, CV_RGB(255,255,255));
}

//绘制群组编号
void DrawGroupNum(IplImage* pForeImg, int groupNum, int* peopleNum, int left, int bottom)
{
	int i;
	CString str, tmp;
	CvFont font;

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1);

	str.Format("G%d:", groupNum);
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(peopleNum[i] != 0)
		{
			tmp.Format("P%d", peopleNum[i]);
			str += tmp;
		}
	}

	cvPutText(pForeImg, str.GetBuffer(0), cvPoint(left, bottom), &font, CV_RGB(255,255,255));
}

//绘制人体的运动方向
void DrawDirection(IplImage* pForeImg, int direction, int left, int bottom) 
{
	CvFont font;
	CString str;
	
	//初始化字体
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1);
	
	//不同的移动方向，显示不同的记号
	if(direction > 0)
	{
		str = "<-";
	} else {
		str = "->";
	}
	
	cvPutText(pForeImg, str.GetBuffer(0), cvPoint(left, bottom), &font, CV_RGB(0,0,0));
}

//绘制人体速度
void DrawSpeed(IplImage* pForeImg, double speed, int left, int bottom)
{
	CvFont font;
	CString str;
	
	//初始化字体
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1);

	str.Format("%.1f", speed);
	str += "p/s";
	cvPutText(pForeImg, str.GetBuffer(0), cvPoint(left, bottom), &font, CV_RGB(0,255,255));
}

//绘制边界框
void DrawRect(IplImage* img, int up, int down, int left, int right) 
{
	int	i, j;
	
	//绘制左右边界
	for(i=up; i<down; i++)
	{
		(*img).imageData[i*WIDTH*3 + left*3] = (char)255;
		(*img).imageData[i*WIDTH*3 + left*3+1] = (char)255;
		(*img).imageData[i*WIDTH*3 + left*3+2] = (char)255;
		
		(*img).imageData[i*WIDTH*3 + right*3] = (char)255;
		(*img).imageData[i*WIDTH*3 + right*3+1] = (char)255;
		(*img).imageData[i*WIDTH*3 + right*3+2] = (char)255;
	}
	
	//绘制上下边界
	for(j=left; j<right; j++)
	{
		(*img).imageData[up*WIDTH*3 + j*3] = (char)255;
		(*img).imageData[up*WIDTH*3 + j*3+1] = (char)255;
		(*img).imageData[up*WIDTH*3 + j*3+2] = (char)255;
		
		(*img).imageData[down*WIDTH*3 + j*3] = (char)255;
		(*img).imageData[down*WIDTH*3 + j*3+1] = (char)255;
		(*img).imageData[down*WIDTH*3 + j*3+2] = (char)255;
	}
}

//绘制中心点
void DrawCentroid(IplImage* img, int up, int down, int left, int right)
{
	//计算中点坐标
	int midX = (left + right) / 2;
	int midY = (up + down) / 2;
	
	//绘制中点
	(*img).imageData[midY*WIDTH*3 + midX*3] = (char)0;
	(*img).imageData[midY*WIDTH*3 + midX*3+1] = (char)0;   
	(*img).imageData[midY*WIDTH*3 + midX*3+2] = (char)255;
}

#define COLOR_BLUE   0
#define COLOR_GREEN  1
#define COLOR_RED    2

// 绘制线段
void drawLine(_Line line, IplImage* pFrame, int color)
{
	int i, j;

	for(i=0; i<HEIGHT; i++)
	{
		for(j=0; j<WIDTH; j++)
		{
			if ((line.a * j - i + line.b) * (line.a * j - i + line.b) <= (1 + line.a * line.a))
			{
				if((j >= line.minX)
					&& (j <= line.maxX)
					&& (i >= line.minY)
					&& (i <= line.maxY))
				{
					if(color == COLOR_BLUE)
					{
						(*pFrame).imageData[i*WIDTH*3 + j*3] = (char)255;
						(*pFrame).imageData[i*WIDTH*3 + j*3+1] = (char)0;
						(*pFrame).imageData[i*WIDTH*3 + j*3+2] = (char)0;
					}
					else if(color == COLOR_RED)
					{
						(*pFrame).imageData[i*WIDTH*3 + j*3] = (char)0;
						(*pFrame).imageData[i*WIDTH*3 + j*3+1] = (char)0;
						(*pFrame).imageData[i*WIDTH*3 + j*3+2] = (char)255;
					}
					else
					{
						(*pFrame).imageData[i*WIDTH*3 + j*3] = (char)0;
						(*pFrame).imageData[i*WIDTH*3 + j*3+1] = (char)255;
						(*pFrame).imageData[i*WIDTH*3 + j*3+2] = (char)0;
					}
				}
			}
		}
	}
}

// 绘制单个检测框
void drawRect(_DetectRect* rect, IplImage* pFrame, int drawMode)
{
	int i;

	if (drawMode == 0)
	{
		drawLine(rect->enterLine, pFrame, COLOR_BLUE);
	}
	else
	{
		drawLine(rect->enterLine, pFrame, COLOR_RED);
	}

	for(i=0; i<3; i++)
	{
		if(rect->delayCount != 0 && rect->currentLeaveLine == i)
		{
			drawLine(rect->leaveLine[i], pFrame, COLOR_RED);
			rect->delayCount -= 1;
		}
		else
		{
			drawLine(rect->leaveLine[i], pFrame, COLOR_GREEN);
		}
	}
}

// 绘制所有检测框
void drawAllRects(IplImage* pFrame)
{
	int i;
	CString str;

	for (i = 0; i < 16; i++)
	{
		if(detectRect[i].isDrew == 1)
		{
			// 若已确定最终的检测框，则绘制为红色
			if(isRectConfirmed && i == finalRectNum)
			{
				drawRect(&detectRect[i], pFrame, 2);
			}
			else
			{
				drawRect(&detectRect[i], pFrame, 0);
			}
		}
	}
}

//绘制人体信息
void DrawPeople(pplConn* ppl, foreImg* fore, IplImage* pFrame) 
{
	int i;

	//绘制人体信息：边界框、中心点、编号、运动方向
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(ppl[i].isPeople)
		{
			//绘制人体的前景像素
			//DrawPeopleForeImg(fore, pForeImg, ppl[i].nConnRgn);
			//绘制边界框
			DrawRect(pFrame, ppl[i].up, ppl[i].down, ppl[i].left, ppl[i].right);
			//绘制中心点
			//DrawCentroid(pForeImg, ppl[i].up, ppl[i].down, ppl[i].left, ppl[i].right);
			//绘制运动方向
			DrawDirection(pFrame, ppl[i].direction, ppl[i].left, ppl[i].up-15);
			//绘制运动速度
			//DrawSpeed(pFrame, ppl[i].speed, ppl[i].left, ppl[i].up-27);

			//绘制人体编号
			if(ppl[i].isGroup)
			{
				DrawGroupNum(pFrame, ppl[i].groupNum, ppl[i].peopleNum, ppl[i].left, ppl[i].up-3);
			}
			else
			{
				DrawPeopleNum(pFrame, ppl[i].peopleNum[0], ppl[i].left, ppl[i].up-3);
			}
		}
	}

	drawAllRects(pFrame);
}

//显示图片
void ShowImg(IplImage* pForeImage, int repID) 
{
	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	CWnd *pic = AfxGetApp()->m_pMainWnd->GetDlgItem(repID);
	mWin->sgImage.CopyOf(pForeImage, 3);
	HDC camPicHDC = pic->GetDC()->GetSafeHdc();
	pic->GetClientRect(&rect);
	mWin->sgImage.DrawToHDC(camPicHDC, &rect);
}