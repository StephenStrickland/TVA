#include <StdAfx.h>
#include "MyProc.h"
#include <math.h>

//�����ⲿ�ļ���ȫ�ֱ���
extern int peopleNum;
extern int groupNum;

extern int tourNum;
extern int connNum;

extern int g_frameNum;
extern float g_frameTimeGap;

extern int leftBorder;

extern CRect rect;

// 16������
extern _DetectRect detectRect[16];

// ���ղ��õļ���ı��
int finalRectNum;

// �Ƿ��Ѿ�ѡ�����������ڱȽϵ��߶�
bool isRectConfirmed;

CString originText, newText, newText2;
extern int firstFlag;

// ��������
int round(double num)
{
	return (int)floor(num + 0.5);
}

//�����γ̵���ͨ�������㷨
void LabelConnByTour(struct foreImg* fore, struct tour* tours)
{
	int i, j, k, l;

	int maxStart;
	int minRight;
	
	//����ÿһ��
	for(i=0; i<HEIGHT; i++)
	{
		//�ҵ�ÿһ�е��γ�
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

		//��Ϊ��ͨ����ĵ�һ��
		if((connNum == 0) && (tourNum != 0))
		{
			//�����ÿһ���γ̷���һ���µ�������
			for(j=0; j<tourNum; j++)
			{
				if(tours[j].rowNum == i)
				{	
					tours[j].connNum = connNum;
					connNum++;
				}
			}
		} 
		else	//����Ϊ��һ��
		{
			//�򽫵�ǰ�е�ÿһ���γ� R_now ����һ�е�ÿһ���γ� R_pre ����ƥ��
			for(j=0; j<tourNum; j++)
			{
				if(tours[j].rowNum == i)	//��ǰ��
				{
					for(k=0; k<tourNum; k++)
					{
						if(tours[k].rowNum == (i-1))	//��һ��
						{
							maxStart = tours[j].start>tours[k].start? tours[j].start : tours[k].start;
							minRight = tours[j].end<tours[k].end? tours[j].end: tours[k].end;
							
							//�� R_now �ҵ�������֮ƥ��� R_pre
							if(maxStart < minRight)
							{
								//���� R_now δ���
								if(!tours[j].isMatched)
								{
									//�� R_pre �ı�Ÿ��� R_now������Ǵ� R_now Ϊ�ѱ��
									tours[j].connNum = tours[k].connNum;
									tours[j].isMatched = true;
								} 
								else	//���� R_now �ѱ��
								{
									//�� R_now �ı�Ÿ��� R_pre �������е���ͨ�γ�
									for(l=0; l<tourNum; l++)
									{
										if(tours[l].connNum == tours[k].connNum)
										{
											tours[l].connNum = tours[j].connNum;
										}
									}
								}
							}//��ƥ��
						}//��һ��
					}
				}//��ǰ��
			}

			//�������е� R_now
			for(j=0; j<tourNum; j++)
			{
				//��ĳ R_now δƥ�䣬��Ϊ������µ�������
				if((tours[j].rowNum == i) && (!tours[j].isMatched))
				{
					//AfxMessageBox("s");
					tours[j].connNum = connNum;
					connNum++;
				}
			}
		}//ǰ�����н���ƥ��
	}

	//�������γ̵������Ÿ������Ӧ�����أ�����Ǵ�����Ϊ isNumbered
	for(i=0; i<tourNum; i++)
	{
		for(j=tours[i].start; j<=tours[i].end; j++)
		{
			fore[tours[i].rowNum*WIDTH+j].nConnRgn = tours[i].connNum;
			fore[tours[i].rowNum*WIDTH+j].isNumbered = true;
		}
	}

	//�γ̺ź���ͨ����������
	//tourNum = 0;
	//connNum = 0;
}


//�����������
void InitRectCalc(POINT* linePos, int enterType, _DetectRect* newRect)
{
	//��ֵ�����������νṹ��
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


//������ת��ʽ
void RotatePointCalc(_Point pSource, _Point* pResult, _Point center, long double angle){
	pResult->x = cos(angle) * (pSource.x - center.x) - sin(angle) * (pSource.y - center.y) + center.x;
	pResult->y = sin(angle) * (pSource.x - center.x) + cos(angle) * (pSource.y - center.y) + center.y;
}

//������ת������ĸ�������
void RotateRectCalc(_DetectRect* rotateRect, int angleDu){
	long double angle;
	_Point centerPos;
	_DetectRect tmpRect;

	// ����תΪ������
	angle = (long double)angleDu * 3.141592653 / 180.0; 
	
	//��������������꣨��������ת��
	centerPos.x = (rotateRect->corners[0].x + rotateRect->corners[1].x
				  + rotateRect->corners[2].x + rotateRect->corners[3].x) / 4.0;
	centerPos.y = (rotateRect->corners[0].y + rotateRect->corners[1].y
				  + rotateRect->corners[2].y + rotateRect->corners[3].y) / 4.0;

	//������ת������
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


//�������������б�ʽؾ�
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

//16����������
void TotalRectMend(_DetectRect* detectRectArray)
{
	int i;

	//����������������߶α��ʽ
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

// ���Ƽ������
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
	
	//���¼�����ת��������꣨�Ի������꣩
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

	//���ƿ����
	CFont*  pOldFont;
	CFont newFont;
	newFont.CreatePointFont(1, "Arial");
	pOldFont = pDC.SelectObject(&newFont);
	pDC.SetTextColor(RGB(0,0,255));
	CString txt;
	txt.Format("%d", dRect.rectNum);
	pDC.TextOut((int)centerX,(int)centerY,txt);
	
	//��������
	CPen penY(PS_SOLID, 0, RGB(255,255,0));
	CPen penB(PS_SOLID, 0, RGB(0,0,255));
	CPen penG(PS_SOLID, 0, RGB(0,255,0));
	CPen penR(PS_DOT, 0, RGB(255,0,0));
	CPen penBX(PS_DOT, 0, RGB(0,0,255));
	CPen penGX(PS_DOT, 0, RGB(0,255,0));

	//�޹�ѡ
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
	//�й�ѡ
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
	else if (drawMode == 2)	// �����
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

//ƽ�Ƽ���
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

//��ʾ������Ϣ
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


//��������ģ�͵Ŀ��б�
pplList CreatePeopleList()
{
	pplList L;
	L = (pplList)malloc(sizeof(pplModel));
	L->peopleNum = 0;
	L->nextPeople = NULL;
	return L;
}

//��������
void AddPeople(int peopleNum, double* histBins, unsigned long time, pplList L)
{
	int i;
	pplNode P, newPpl;
	
	//Pָ���ͷ
	P = L;

	//�ҵ����ĩβλ��
	while(P->nextPeople != NULL)
	{
		P = P->nextPeople;
	}

	//�������ĩβ����µ�����ģ��
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


//��������
void UpdatePeople(int peopleNum, double* histBins, unsigned long time, double speed, pplList L)
{
	int i;
	pplNode P;
	CString str;

	//��Ϊ�ձ����˳�
	if(L->nextPeople == NULL)
	{
		return;
	}

	P = L->nextPeople;

	while(P->peopleNum != peopleNum)
	{
		P = P->nextPeople;

		//�������β�����˳�
		if(P == NULL)
		{
			return;
		}
	}

	//���¸���ֱ��ͼ
	for(i=0; i<4096; i++)
	{
		P->histBins[i] = HIST_UPDATE_WEIGHT * P->histBins[i] + (1 - HIST_UPDATE_WEIGHT) * histBins[i];
	}

	//���㶺��ʱ��
	P->stayTime = time - P->appearTime;
	if(P->stayTime > ABNORMAL_STAY_TIME)	//�ж��Ƿ��쳣
	{
		str.Format("��%d����ʱ�������", P->peopleNum);
		//AfxMessageBox(str.GetBuffer(0));
	}

	//���浱ǰ�ٶ�
	P->speed = speed;

	if(P->speed > ABNORMAL_SPEED)
	{
		str.Format("��%d�ٶȹ��죡", P->peopleNum);
		//AfxMessageBox(str.GetBuffer(0));
	}
}

//�������壨����ʱ�䡢�ٶȣ�������ֱ��ͼ��
void UpdatePeopleExHist(int peopleNum, unsigned long time, double speed, pplList L)
{
	pplNode P;
	CString str;
	
	//��Ϊ�ձ����˳�
	if(L->nextPeople == NULL)
	{
		return;
	}
	
	P = L->nextPeople;
	
	while(P->peopleNum != peopleNum)
	{
		P = P->nextPeople;
		
		//�������β�����˳�
		if(P == NULL)
		{
			return;
		}
	}
	
	//���㶺��ʱ��
	P->stayTime = time - P->appearTime;
	if(P->stayTime > ABNORMAL_STAY_TIME)	//�ж��Ƿ��쳣
	{
		str.Format("��%d����ʱ�������", P->peopleNum);
		//AfxMessageBox(str.GetBuffer(0));
	}

	//���浱ǰ�ٶ�
	P->speed = speed;
	if(P->speed > ABNORMAL_SPEED)
	{
		str.Format("��%d�ٶȹ��죡", P->peopleNum);
		//AfxMessageBox(str.GetBuffer(0));
	}
}

//��ȡ�����ֱ��ͼ��Ϣ
void GetPeopleHist(int peopleNum, double* histBins, pplList L)
{
	int i;
	pplNode P;
	
	//��Ϊ�ձ����˳�
	if(L->nextPeople == NULL)
	{
		return;
	}
	
	P = L->nextPeople;
	
	while(P->peopleNum != peopleNum)
	{
		P = P->nextPeople;
		
		//�������β�����˳�
		if(P == NULL)
		{
			return;
		}
	}
	
	//���¸���ֱ��ͼ
	for(i=0; i<4096; i++)
	{
		histBins[i] = P->histBins[i];
	}
}

//ɾ������
void DeletePeople(int peopleNum, pplList L)
{
	pplNode P, tmpNode;

	//���˱�Ϊ�ձ����˳�����
	if(L->nextPeople == NULL)
	{
		return;
	}

	//Pָ���ͷ
	P = L;

	//�ҵ����д� peopleNum �ı����ǰһ�� P
	while((P->nextPeople)->peopleNum != peopleNum)
	{
		//��δ�ҵ�����ָ���Ƶ���һ��
		P = P->nextPeople;
		
		//����Ѿ��ҵ���β�����˳�����
		if(P->nextPeople == NULL)
		{
			return;
		}
	}

	//ɾ���˱���
	tmpNode = P->nextPeople;
	P->nextPeople = tmpNode->nextPeople;
	free(tmpNode);
}

//��ӡ����
void PrintPeopleList(pplList L)
{
	CString str, tmp;
	pplNode P;

	str = "P��";

	//��Ϊ�ձ����˳�
	if(L->nextPeople == NULL)
	{	
		return;
	}

	P = L->nextPeople;

	//����ÿһ������ģ�ͣ�����ʾ��������
	while(P != NULL)
	{
		tmp.Format("%d", P->peopleNum);
		str += tmp;
		P = P->nextPeople;
	}
		
	//��ʾ���е�������
	AfxMessageBox(str.GetBuffer(0));
}

//��ֵ�˲�
void MiddleFilter(foreImg* img, int filterH, int filterW, int filterMX, int filterMY)
{
	int i, j, k, l;
	char nTrue, nFalse;
	
	//�У���ȥ��Ե���У�
	for(i=filterMY; i<HEIGHT-filterH+filterMY+1; i++)
	{
		//�У���ȥ��Ե���У�
		for(j=filterMX; j<WIDTH-filterW+filterMX+1; j++)
		{
			nTrue = 0;
			nFalse = 0;
			
			//������ֵ�˲�
			for(k=0; k<filterH; k++)
			{
				for(l=0; l<filterW; l++)
				{
					//�ֱ�ͳ��ǰ���ͱ����ĸ���
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
	
	//����Ե����Ϊ����
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

//ȥ����Ӱ
void RemoveShadow(foreImg* fore)
{
	int i, j;
	
	int hei[WIDTH];
	for(i=0; i<WIDTH; i++)
	{
		hei[i] = 0;
	}
	
	//ͳ�Ƹ��е�ǰ��������
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
	
	//�ҵ����ֵ
	int maxHeight = 0;
	for(j=0; j<WIDTH; j++)
	{
		if(hei[j] > maxHeight)
		{
			maxHeight = hei[j];
		}
	}
	
	//ȥ��ǰ��������С����ֵ����
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

// ����ĳ����ģ����ĳ��ͨ�����ƥ��ϵ��
double MatchPeopleToConn(int peopleNum, int nConnRgn, int nPixel, pplList L, foreImg* fore)
{
	int i;

	static double pplHistBins[4096];
	static double connHistBins[4096];

	double minHistSum = 0.0;
	double pplHistSum = 0.0;

	// ��ȡ����ֱ��ͼģ��
	GetPeopleHist(peopleNum, pplHistBins, L);

	// ��ȡ��ͨ�����ֱ��ͼģ��
	CreateHistBins(connHistBins, nConnRgn, nPixel, fore);

	for(i=0; i<4096; i++)
	{
		minHistSum += ((pplHistBins[i] < connHistBins[i])? pplHistBins[i]: connHistBins[i]);
		pplHistSum += pplHistBins[i];
	}

	return (minHistSum / pplHistSum);
}

// ����ѧϰ
void BgLearn(IplImage* pFrame, int frameNum, int watershed, double* mean, double* var)
{
	int i, j;
	static long int sum[PIXEL_BYTES_NUM];		// RGB�ۼӺ�
	static long int squSum[PIXEL_BYTES_NUM];	// RGBƽ����

	// ��ʼ��
	if(frameNum == 1)
	{
		for(i=0; i<PIXEL_BYTES_NUM; i++)
		{
			sum[i] = 0;
			squSum[i] = 0;
		}	
	}

	// �ۼ�
	for(j=0; j<PIXEL_BYTES_NUM; j++)
	{
		sum[j] += (*pFrame).imageData[j];
		squSum[j] += (*pFrame).imageData[j] * (*pFrame).imageData[j];
	}
	
	// �����ֵ�ͷ���
	if(frameNum == watershed)
	{
		for(i=0; i<PIXEL_BYTES_NUM; i++)
		{
			mean[i] = sum[i] / (double)watershed;
			var[i] = sqrtl(squSum[i] / (double)watershed - mean[i] * mean[i]);
		}
	}

	// ��ʾ
	ShowImg(pFrame, IDC_PICTURE_A);
	drawAllRects(pFrame);
	ShowImg(pFrame, IDC_PICTURE_B);
}

//�����������£�����ȡǰ��
void BgDiff(IplImage* pFrame, double* mean, double* var, foreImg* fore) 
{
	int i;
	
	//��ֵ
	double diffB, diffG, diffR;
	
	//��ʼ��������뱳������
	for(i=0; i<PIXEL_NUM; i++)
	{
		//���б������
		diffB = fabsl((*pFrame).imageData[i*3] - mean[i*3]);
		bool isBFore = (diffB > 3 * var[i*3]);
		if(isBFore)
		{
			//���Ϊǰ��������R��G��Bֵ
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
				//���Ϊǰ��������R��G��Bֵ
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
					//���Ϊǰ��������R��G��Bֵ
					fore[i].isFore = true;
					fore[i].bValue = (*pFrame).imageData[i*3];
					fore[i].gValue = (*pFrame).imageData[i*3+1];
					fore[i].rValue = (*pFrame).imageData[i*3+2];
				}
				else	//��Ϊ�������أ�����б�������
				{
					//���¾�ֵ
					mean[i*3] = BG_UPDATE_WEIGHT * mean[i*3] + (1 - BG_UPDATE_WEIGHT) * (*pFrame).imageData[i*3];
					mean[i*3+1] = BG_UPDATE_WEIGHT * mean[i*3+1] + (1 - BG_UPDATE_WEIGHT) * (*pFrame).imageData[i*3+1];
					mean[i*3+2] = BG_UPDATE_WEIGHT * mean[i*3+2] + (1 - BG_UPDATE_WEIGHT) * (*pFrame).imageData[i*3+2];
				}
			}
		}
	}
}

//����ɫ�ʵĸ���ֱ��ͼ����
void CreateHistBins(double* histBins, int nConnRgn, int nPixel, foreImg* fore) 
{
	int i;
	
	for(i=0; i<4096; i++)
	{
		histBins[i] = (double)0;
	}
	
	//������ɫֱ��ͼ
	for(i=0; i<PIXEL_NUM; i++)
	{
		if((fore[i].nConnRgn == nConnRgn) && (fore[i].isFore))
		{
			int index = (fore[i].rValue / 16) * 256 + (fore[i].gValue / 16) * 16 + (fore[i].bValue / 16) * 1;
			histBins[index]++;
		}
	}
	
	//�������ֱ��ͼ
	for(i=0; i<4096; i++)
	{
		histBins[i] /= nPixel;
	}
}

//�ж�ƥ������
int GetMatchType(int matchTag, pplConn* prePeos, pplConn* currPeos) 
{
	int i;
	
	//�ֱ������һ֡�͵�ǰ֡�е�ƥ������ĸ���
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
	
	//1��1
	if((preMatchNum == 1) && (currMatchNum == 1))
	{
		matchType = MATCH_TYPE_1T1;
	}
	else if((preMatchNum == 1) && (currMatchNum > 1))	//1�Զ�
	{
		matchType = MATCH_TYPE_1TM;
	} 
	else if((preMatchNum > 1) && (currMatchNum == 1))	//���1
	{
		matchType = MATCH_TYPE_MT1;
	}
	
	return matchType;
}

//����ƥ��
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

	//����ǰ��ͼ��֡�ж�Ӧ���������ͨ�����ƥ��
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(currPeos[i].isPeople)
		{
			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				if(prePeos[j].isPeople)
				{
					//��ǰ֡�е�i�������������һ֡�е�j�������������ƥ��
					
					//����߽����ص����
					int lapLeft = (currPeos[i].left > prePeos[j].left)? currPeos[i].left: prePeos[j].left;
					int lapRight = (currPeos[i].right < prePeos[j].right)? currPeos[i].right: prePeos[j].right;
					//int lapUp = (currPeos[i].up > prePeos[j].up)? currPeos[i].up: prePeos[j].up;
					//int lapDown = (currPeos[i].down < prePeos[j].down)? currPeos[i].down: prePeos[j].down;
					//int lapArea = (lapRight - lapLeft) * (lapDown - lapUp);
					
					//����ƥ��ϵ��
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
					str.Format("��ǰ����%d����һ����%d:%f", i, j, matchFactor);
					AfxMessageBox(str.GetBuffer(0));
					*/

					//��ƥ��
					if(matchFactor > THRESHOLD_MATCH)
					{
						//������ֵ�ǰ֡�Ĵ�������ƥ�䣬��
						if(currPeos[i].matchTag != 0)
						{
							prePeos[j].matchTag = currPeos[i].matchTag;
						}
						else if(prePeos[j].matchTag != 0)
						{
							currPeos[i].matchTag = prePeos[j].matchTag;
						}
						else if((prePeos[j].matchTag == 0) && (currPeos[i].matchTag == 0))	// ����û��ƥ��
						{
							prePeos[j].matchTag = matchTag;
							currPeos[i].matchTag = matchTag;
							matchTag++;
						}

						//�ж��ƶ�����
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

						//����ʱ��
						currPeos[i].speed = fabsl(moveLength / (timeGetTime() - prePeos[i].time) * 1000);
					}	
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// ����3������������1��1�Զࡢ1��1

	for(i=1; i<matchTag; i++)
	{
		matchType = GetMatchType(i, prePeos, currPeos);
		
		//1��1
		switch(matchType)
		{
		case MATCH_TYPE_1T1:
			//�ҵ���һ֡��ƥ����Ϊ i ������
			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				if(prePeos[j].matchTag == i)
				{
					//�ҵ���ǰ֡��ƥ����Ϊ i ������
					for(k=0; k<MAX_TRACKING_PEOPLE; k++)
					{
						if(currPeos[k].matchTag == i)
						{
							//��ΪȺ���1��1ƥ��
							if(prePeos[j].isGroup)	
							{
								currPeos[k].isGroup = true;
								currPeos[k].groupNum = prePeos[j].groupNum;
								for(l=0; l<MAX_TRACKING_PEOPLE; l++)
								{
									currPeos[k].peopleNum[l] = prePeos[j].peopleNum[l];
								}
								
								//��������Ķ���ʱ����ٶ�
								for(l=0; l<MAX_TRACKING_PEOPLE; l++)
								{
									if(currPeos[k].peopleNum[l] != 0)
									{
										UpdatePeopleExHist(currPeos[k].peopleNum[l], currPeos[k].time, currPeos[k].speed, pList);
									}
								}
							}
							else	//��Ϊ���������1��1ƥ��
							{
								currPeos[k].peopleNum[0] = prePeos[j].peopleNum[0];

								//��������ĸ���ֱ��ͼģ��
								CreateHistBins(histBins, currPeos[k].nConnRgn, currPeos[k].nPixel, fore);
								UpdatePeople(currPeos[k].peopleNum[0], histBins, currPeos[k].time, currPeos[k].speed, pList);
							}
						}
					}
				}
			}	
			break;

		case MATCH_TYPE_MT1:	//���1

			//AfxMessageBox("�γ�Ⱥ��");

			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				//�ҵ���ǰ֡���γɵ�Ⱥ�������½�Ⱥ��ģ��
				if(currPeos[j].matchTag == i)
				{
					//���ΪȺ�飬�������µ�Ⱥ����
					currPeos[j].isGroup = true;
					currPeos[j].groupNum = groupNum;

					m = 0;

					//����һ֡��Ⱥ���е�������
					//ȫ����۵����γɵ�Ⱥ����
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

		case MATCH_TYPE_1TM:	//1�Զ�
			
			//�ҵ���һ֡�з��ѵ�Ⱥ������
			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				if(prePeos[j].matchTag == i)
				{	
					//ʹ�� ֱ��ͼ����ƥ�䷨ ������ģ�Ͷ�Ӧ����Щ��Ⱥ����
					for(k=0; k<MAX_TRACKING_PEOPLE; k++)
					{
						if(prePeos[j].peopleNum[k] != 0)
						{
							double matchValue = 0.0;
							double max = 0.0;
							int maxConnIndex;	//ƥ��ֵ�����������������

							for(l=0; l<MAX_TRACKING_PEOPLE; l++)
							{
								if(currPeos[l].matchTag == i)
								{									
									//������Ϊ prePeos[j].peopleNum[k] ������ģ��
									//����Ϊ currPeos[l].nConnRgn ����ͨ�����ƥ��ֵ
									matchValue = MatchPeopleToConn(prePeos[j].peopleNum[k], currPeos[l].nConnRgn, currPeos[l].nPixel, pList, fore);
									
									if(matchValue > max)
									{
										max = matchValue;
										maxConnIndex = l;
									}
								}
							}

							//���������Ӧ������Ϊ l ������

							//��������û��������֮��Ӧ
							if(currPeos[maxConnIndex].peopleNum[0] == 0)
							{
								currPeos[maxConnIndex].peopleNum[0] = prePeos[j].peopleNum[k];
							}
							else //������������������֮��Ӧ
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
					}//k����һ֡ĳȺ���е�ÿһ��������


					//����Ⱥ������Ⱥ�飬���ʼ���µ�Ⱥ��ģ��
					for(k=0; k<MAX_TRACKING_PEOPLE; k++)
					{
						if(currPeos[k].matchTag == i)
						{
							if(currPeos[k].isGroup)
							{
								//��ʼ��Ⱥ��ģ��
								currPeos[k].groupNum = groupNum;
								groupNum++;
							}
						}
					}
					
				}
			}//j����һ֡�е�Ⱥ��
			break;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// ���������2�������1��0��0��1

	//�ҵ���һ֡��δƥ�������
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(prePeos[i].isPeople)
		{
			//������δƥ��������򴥷��¼������˴ӳ�������ʧ
			if(prePeos[i].matchTag == 0)
			{
				if(prePeos[i].isGroup)
				{
					//ɾ��Ⱥ���а���������
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
					//ɾ������
					DeletePeople(prePeos[i].peopleNum[0], pList);
				}
			}
		}
	}

	//�ҵ���ǰ֡��δƥ�������
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(currPeos[i].isPeople)
		{
			if(currPeos[i].matchTag == 0)
			{				
				//Ϊ���������µ�������
				currPeos[i].peopleNum[0] = peopleNum;

				//��������ģ��
				CreateHistBins(histBins, currPeos[i].nConnRgn, currPeos[i].nPixel, fore);
				AddPeople(currPeos[i].peopleNum[0], histBins, currPeos[i].time, pList);
				peopleNum++;
			}
		}
	}

	// ������
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(currPeos[i].isPeople)
		{
			for(j=0; j<MAX_TRACKING_PEOPLE; j++)
			{
				if(prePeos[j].isPeople
					&& prePeos[j].matchTag == currPeos[i].matchTag)
					
				{
					// ��һ֡����δ��������
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

						// ��һ֡���˽�������
						if (isEntered)
						{
							currPeos[i].areaState = PEOPLE_IN;
							
							// ��ʾ���ٽ��
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
						// ��һ֡������δ��������
						else	
						{
							currPeos[i].areaState = PEOPLE_NOTIN;	// ����δ����״̬
						}
					}
					// ��һ֡�����ѽ�������
					else if(prePeos[j].areaState == PEOPLE_IN)
					{
						// ��һ֡�����뿪����
						if(checkLeaveRect(detectRect[finalRectNum], &currPeos[i]))
						{
							//firstFlag = 1;
							currPeos[i].areaState = PEOPLE_OUT;
							
							// ��ʾ���ٽ��
							CEdit* pEdit = (CEdit*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_EDIT_RESULT + finalRectNum);
							pEdit->GetWindowText(originText);
							time2 = (int)(g_frameNum * g_frameTimeGap);
							//newText.Format("P%d leaves at 00:%02d:%02d:%02d\r\n\r\n", currPeos[i].peopleNum[0], time2 / 3600, time2 / 60, time2 % 60);
							newText.Format("P%d leaves at 00:%02d:%02d:%02d\r\n", currPeos[i].peopleNum[0], time2 / 3600, time2 / 60, time2 % 60);
							pEdit->SetWindowText(originText + newText);
						}
						// ��һ֡��������������
						else
						{
							currPeos[i].areaState = PEOPLE_IN;	// ���ֽ���״̬
						}
					}
					// ����һ֡�����Ѿ��뿪�����򱣳��뿪״̬
					else if(prePeos[j].areaState == PEOPLE_OUT)
					{
						currPeos[i].areaState = PEOPLE_OUT;
					}
				}
			}
		}
	}

	//��ǰ֡�����ݸ�����һ֡������
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

// �ж�������Ƿ��һ�ν���ĳ����
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

	// �߶κ�������߽��ཻ
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

// �ж������Ƿ�������еļ����е�ĳ��
// ����⵽���룬�򽫴˼���ı�Ŵ���ȫ�ֱ�����
bool checkEnterMultiRect(pplConn* people)
{
	int i;
	
	// �������нڵ�
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

// �ж����Ƿ��뿪ĳ�߶�
bool checkLeaveLine(_Line line, pplConn* people)
{
	// ��������ұ߽粻�ཻ�������ұ߽����߶ε����
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

// ���������ұ߽���߶ε���С����
double calDistance(_Line line, pplConn* people)
{
	double distanceA = 0.0;
	double distanceB = 0.0;
	
	distanceA = fabs(line.a * (people->right - 0.5) - people->up + line.b) / sqrt(line.a * line.a + 1);
	distanceB = fabs(line.a * (people->right - 0.5) - people->down + line.b) / sqrt(line.a * line.a + 1);

	return min(distanceA, distanceB);
}

// �ж����Ƿ��뿪ĳ����
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

	if (leaveCount == 3)	// �뿪3���߶Σ���ʾ��ȫ�뿪���򣬼�ʱ��ʼ
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
  
//��ȡ��ͨ�������Ϣ
void GetConnsInfo(foreImg* fore, connRgn* conns)
{
	int i, j;

	//��ʼ����ǰ֡����ͨ�������Ϣ
	for(i=0; i<MAX_CONN_RGN; i++)
	{
		conns[i].nPixel = 0;
		conns[i].left = WIDTH - 1;
		conns[i].right = 0;
		conns[i].up = HEIGHT - 1;
		conns[i].down = 0;
	}
	
	//Ϊ�ṹ�����������Ϣ
	for(i=0; i<HEIGHT; i++)
	{
		for(j=0; j<WIDTH; j++)
		{
			//�������ر����
			if(fore[i*WIDTH+j].isNumbered)
			{
				//��ȡ��ͨ����ı��
				char index = fore[i*WIDTH+j].nConnRgn;
				
				//�ۼ�������
				conns[index].nPixel++;
				
				//������ͨ����ı��
				conns[index].nConnRgn = index;
				
				//���±߽�ֵ
				if(i < conns[index].up)		//�ϱ߽�
				{
					conns[index].up = i;
				}
				if(i > conns[index].down)	//�±߽�
				{
					conns[index].down = i;
				}
				if(j < conns[index].left)	//��߽�
				{
					conns[index].left = j;
				}
				if(j > conns[index].right)	//�ұ߽�
				{
					conns[index].right = j;
				}
			}
		}//��
	}//��
}

//��ȡ��ͨ�������Ϣ
void GetConnsInfoByTour(struct tour* tours, struct connRgn* conns)
{
	int i;
	
	//��ʼ����ǰ֡����ͨ�������Ϣ
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
		
		//���±߽�ֵ
		if(tours[i].rowNum < conns[index].up)	//�ϱ߽�
		{
			conns[index].up = tours[i].rowNum;
		}
		if(tours[i].rowNum > conns[index].down)	//�±߽�
		{
			conns[index].down = tours[i].rowNum;
		}
		if(tours[i].start < conns[index].left)	//��߽�
		{
			conns[index].left = tours[i].start;
		}
		if(tours[i].end > conns[index].right)	//�ұ߽�
		{
			conns[index].right = tours[i].end;
		}
	}
}

//��ȡ�����������Ϣ
void GetPeopleConns(connRgn* conns, pplConn* currPeos, foreImg* fore) 
{
	int i, j, k;
	
	//���Ϊ�������򣬼������
	for(i=0,j=0; i<MAX_CONN_RGN; i++)
	{
		//��������Ŀ������ֵ�����ӦΪ����
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
			
			//����߽�����
			currPeos[j].area = (conns[i].down - conns[i].up) * (conns[i].right - conns[i].left);
			
			/* author:hustlzp date:2012/7/3 */
			//����ÿһ�������ɫ��ֱ��ͼ����ģ��
			for(k=0; k<4096; k++)
			{
				currPeos[j].histBins[k] = (double)0;
			}
			
			//������ɫֱ��ͼ
			for(k=0; k<PIXEL_NUM; k++)
			{
				if((fore[k].nConnRgn == currPeos[j].nConnRgn) && (fore[k].isFore))
				{
					int index = (fore[k].rValue / 16) * 256 + (fore[k].gValue / 16) * 16 + (fore[k].bValue / 16) * 1;
					currPeos[j].histBins[index]++;
				}
			}
			
			//�������ֱ��ͼ
			for(k=0; k<4096; k++)
			{
				currPeos[j].histBins[k] /= currPeos[j].nPixel;
			}
			/*end*/

			j++;
		}
	}
}

//��ȡʱ���ֵ����λ��us��
int GetTimeDiff(LARGE_INTEGER t2, LARGE_INTEGER t1, LARGE_INTEGER tc) 
{
	return (int)((double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart * 1000000);
}

//��ȡ��ֹ�����붯̬�����ķ�ˮ��֡��
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

//��ȡ��Ƶ�ļ���ַ
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

//��ĳ�㿪ʼ��Ǵ���ͨ����
void label(foreImg* fore, int i, int j, int nConnNum)
{
	int k, l;
	for(k=0; k<3; k++)
	{
		for(l=0; l<3; l++)
		{
			//�������߽磬�򷵻�
			if((i-1+k) < 0 || (i-1+k)>= HEIGHT || (j-1+l) < 0 || (j-1+l) >= WIDTH)
				break; 
			//������ĵ�
			if((k == 1) && (l == 1))
			{
				fore[(i-1+k)*WIDTH+j-1+l].nConnRgn = nConnNum;
				fore[(i-1+k)*WIDTH+j-1+l].isNumbered = true;
			}
			else if(fore[(i-1+k)*WIDTH+j-1+l].isFore && (fore[(i-1+k)*WIDTH+j-1+l].isNumbered == false))
			{
				//��ʼ��һ�ֱ��
				label(fore, i-1+k, j-1+l, nConnNum);
			}
		}
	}
}

//����㷨
void LabelConnRgn(foreImg* fore)
{
	int i, j;
	int nConnNum = 0;

	for(i=0; i<HEIGHT; i++)
	{
		for(j=0; j<WIDTH; j++)
		{
			//���˵�Ϊǰ����δ����ǣ���ʼ�˴α��
			if(fore[i*WIDTH+j].isFore && (fore[i*WIDTH+j].isNumbered == false))
			{
				label(fore, i, j, nConnNum);
				nConnNum++;
			}
		}
	}
}

//����ǰ������ IplImage �ṹ��
void DrawForeImg(foreImg * fore, IplImage* pForeImg)
{
	int i;
	
	//������ͨ����
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

//����ĳ�������ǰ����ͨ����
void DrawPeopleForeImg(foreImg * fore, IplImage* pForeImg, char nConnRgn)
{
	int i;
	
	//������ͨ����
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

//����������
void DrawPeopleNum(IplImage* pForeImg, int peopleNum, int left, int bottom) 
{
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1);
	CString str;
	str.Format("P%d", peopleNum);
	cvPutText(pForeImg, str.GetBuffer(0), cvPoint(left, bottom), &font, CV_RGB(255,255,255));
}

//����Ⱥ����
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

//����������˶�����
void DrawDirection(IplImage* pForeImg, int direction, int left, int bottom) 
{
	CvFont font;
	CString str;
	
	//��ʼ������
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1);
	
	//��ͬ���ƶ�������ʾ��ͬ�ļǺ�
	if(direction > 0)
	{
		str = "<-";
	} else {
		str = "->";
	}
	
	cvPutText(pForeImg, str.GetBuffer(0), cvPoint(left, bottom), &font, CV_RGB(0,0,0));
}

//���������ٶ�
void DrawSpeed(IplImage* pForeImg, double speed, int left, int bottom)
{
	CvFont font;
	CString str;
	
	//��ʼ������
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1);

	str.Format("%.1f", speed);
	str += "p/s";
	cvPutText(pForeImg, str.GetBuffer(0), cvPoint(left, bottom), &font, CV_RGB(0,255,255));
}

//���Ʊ߽��
void DrawRect(IplImage* img, int up, int down, int left, int right) 
{
	int	i, j;
	
	//�������ұ߽�
	for(i=up; i<down; i++)
	{
		(*img).imageData[i*WIDTH*3 + left*3] = (char)255;
		(*img).imageData[i*WIDTH*3 + left*3+1] = (char)255;
		(*img).imageData[i*WIDTH*3 + left*3+2] = (char)255;
		
		(*img).imageData[i*WIDTH*3 + right*3] = (char)255;
		(*img).imageData[i*WIDTH*3 + right*3+1] = (char)255;
		(*img).imageData[i*WIDTH*3 + right*3+2] = (char)255;
	}
	
	//�������±߽�
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

//�������ĵ�
void DrawCentroid(IplImage* img, int up, int down, int left, int right)
{
	//�����е�����
	int midX = (left + right) / 2;
	int midY = (up + down) / 2;
	
	//�����е�
	(*img).imageData[midY*WIDTH*3 + midX*3] = (char)0;
	(*img).imageData[midY*WIDTH*3 + midX*3+1] = (char)0;   
	(*img).imageData[midY*WIDTH*3 + midX*3+2] = (char)255;
}

#define COLOR_BLUE   0
#define COLOR_GREEN  1
#define COLOR_RED    2

// �����߶�
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

// ���Ƶ�������
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

// �������м���
void drawAllRects(IplImage* pFrame)
{
	int i;
	CString str;

	for (i = 0; i < 16; i++)
	{
		if(detectRect[i].isDrew == 1)
		{
			// ����ȷ�����յļ��������Ϊ��ɫ
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

//����������Ϣ
void DrawPeople(pplConn* ppl, foreImg* fore, IplImage* pFrame) 
{
	int i;

	//����������Ϣ���߽�����ĵ㡢��š��˶�����
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		if(ppl[i].isPeople)
		{
			//���������ǰ������
			//DrawPeopleForeImg(fore, pForeImg, ppl[i].nConnRgn);
			//���Ʊ߽��
			DrawRect(pFrame, ppl[i].up, ppl[i].down, ppl[i].left, ppl[i].right);
			//�������ĵ�
			//DrawCentroid(pForeImg, ppl[i].up, ppl[i].down, ppl[i].left, ppl[i].right);
			//�����˶�����
			DrawDirection(pFrame, ppl[i].direction, ppl[i].left, ppl[i].up-15);
			//�����˶��ٶ�
			//DrawSpeed(pFrame, ppl[i].speed, ppl[i].left, ppl[i].up-27);

			//����������
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

//��ʾͼƬ
void ShowImg(IplImage* pForeImage, int repID) 
{
	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	CWnd *pic = AfxGetApp()->m_pMainWnd->GetDlgItem(repID);
	mWin->sgImage.CopyOf(pForeImage, 3);
	HDC camPicHDC = pic->GetDC()->GetSafeHdc();
	pic->GetClientRect(&rect);
	mWin->sgImage.DrawToHDC(camPicHDC, &rect);
}