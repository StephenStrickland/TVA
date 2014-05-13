#ifndef _MYPROC_H_
#define _MYPROC_H_

#include "FinalTestDlg.h"

/////////////////////////////////////////////////////////////////////////////
// �궨��

//����ͼ����Ϣ
#define WIDTH 320	//ͼ��Ŀ�
#define HEIGHT 240	//ͼ��ĸ�
#define PIXEL_NUM (WIDTH * HEIGHT)	//������Ŀ
#define PIXEL_BYTES_NUM (PIXEL_NUM * 3)	//�����ֽ���

//�������ϵ��
#define BG_UPDATE_WEIGHT 0.9	//��������ϵ��
#define HIST_UPDATE_WEIGHT 0.8	//ɫ��ֱ��ͼ����ϵ��

//�����ܹ���⵽����ͨ����ĸ���
#define  MAX_CONN_RGN 30
//�����ܹ����ٵ���������
#define MAX_TRACKING_PEOPLE 20

//��������쳣����ֵ
#define ABNORMAL_STAY_TIME 5000	//����ʱ���쳣
#define ABNORMAL_SPEED 50000	//�ƶ��ٶ�

//���������ֵ
#define THRESHOLD_HUMAN_PIXEL_NUM 500	//��Ӧ���������ͨ�����������Ŀ����ֵ
#define THRESHOLD_MATCH	0.75	//ƥ��ϵ������ֵ

//������Ƶ�ļ�������
#define VIDEO_TYPE_1P  0
#define VIDEO_TYPE_2P  1
#define VIDEO_TYPE_3P  2

//�ƶ�����
#define MOVE_LEFT  0	//����
#define MOVE_STOP  1	//ֹͣ�ƶ�
#define MOVE_RIGHT 2	//����

//ƥ������
#define MATCH_TYPE_0T1	0	//0��1
#define MATCH_TYPE_1T0	1	//1��0
#define MATCH_TYPE_1T1	2	//1��1
#define MATCH_TYPE_MT1	3	//���1
#define MATCH_TYPE_1TM	4	//1�Զ�

// ����߽�
#define AREA_LEFT  120	// ������߽�
#define AREA_RIGHT 200	// �����ұ߽�

// �Ƿ���������״̬��ʶ
#define PEOPLE_NOTIN   0	// ��δ��������
#define PEOPLE_IN      1	// ����������
#define PEOPLE_OUT     2	// �뿪����

/////////////////////////////////////////////////////////////////////////////
// ���ݽṹ����

//��ṹ��
struct _Point{
	double x;
	double y;
};
//ֱ�߽ṹ��
struct _Line{
	double a;
	double b;
	_Point p0;
	_Point p1;
	double minX;
	double maxX;
	double minY;
	double maxY;
};
//���νṹ��
struct _DetectRect{
	_Line enterLine;
	_Line leaveLine[3];
	_Point corners[4];
	BOOL isDrew;
	int enterType;
	int rectNum;
	int currentLeaveLine;	// ��ǰ��������뿪line
	int delayCount;		// ��ʱʱ��
};

#define RECT_MOVE_LEFT 0
#define RECT_MOVE_UP 1
#define RECT_MOVE_RIGHT 2
#define RECT_MOVE_DOWN 3

//�����������
void InitRectCalc(POINT* linePos, int enterType, _DetectRect* newRect);
//������ת��ʽ
void RotatePointCalc(_Point pSource, _Point* pResult, _Point center, long double angle);
//������ת������ĸ�������
void RotateRectCalc(_DetectRect* rotateRect, int angleDu);
//�������������б�ʽؾ�
void ABRectCalc(_Line* rectLine);
//16����������
void TotalRectMend(_DetectRect* detectRectArray);
//���Ƽ������
void DrawDetectRect(_DetectRect dRect, int rectID, int drawMode);
//ƽ�Ƽ���
void MoveDetectRect(_DetectRect* moveRect, int moveMode);
//��ʾ������Ϣ
void DispDetectRectInfo(int rectID);


//ǰ��ͼ��
struct foreImg{
	//B��G��Rֵ
	unsigned char bValue;
	unsigned char gValue;
	unsigned char rValue;
	//ǰ�����
	bool isFore;
	//�Ƿ��ѱ����
	bool isNumbered;
	//��ͨ����ı��
	char nConnRgn;
};

//��ͨ����
struct connRgn{
	//������Ŀ
	int nPixel;
	//�ϱ߽�
	int up;
	//�±߽�
	int down;
	//��߽�
	int left;
	//�ұ߽�
	int right;
	//��ͨ����ı��
	char nConnRgn;
};

//�γ̽ṹ��
struct tour{
	int rowNum;	//�к�
	int start;		//��ʼ�к�
	int end;		//��ֹ�к�
	int connNum;	//��ͨ������
	bool isMatched;			//�Ƿ���ƥ��
};

//������ͨ����
struct pplConn{

	int nPixel; //��������
	int up; 	//�ϱ߽�
	int down;	//�±߽�
	int left;	//��߽�
	int right;	//�ұ߽�
	int area;	//�߽������

	bool isPeople;	//�Ƿ�Ϊ����
	bool isGroup;	//�Ƿ�ΪȺ��

	int peopleNum[MAX_TRACKING_PEOPLE];	//������
	int groupNum;	//Ⱥ����

	int matchTag;	//ƥ����

	char nConnRgn;	//��Ӧ����ͨ����ı��

	long direction;	//�˶�����

	unsigned long time;	//��ǰ��ʱ��
	double speed;	//�˶��ٶȣ���λ������/�룩

	//�����ɫ��ֱ��ͼģ��
	double histBins[4096];

	int areaState;	// ����״̬
};

/////////////////////////////////////////////////////////////////////////////
// ����ģ���������������

//����ģ��
struct peopleModel{
	int peopleNum;	//������
	double histBins[4096];	//����Ĳ�ɫֱ��ͼģ��
	unsigned long appearTime;	//����ʱ��
	unsigned long stayTime;	//����ʱ��
	double speed;	//�ٶ�
	peopleModel* nextPeople;	//ָ����һ������ģ��
};
typedef peopleModel pplModel;	//��������
typedef peopleModel* pplList;	//����׵�ַ
typedef peopleModel* pplNode;	//����ĳ�����׵�ַ

//�����յ������б�
pplList CreatePeopleList();
//��������
void AddPeople(int peopleNum, double* histBins, unsigned long time, pplList L);
//�������壨����ֱ��ͼ��ʱ�䡢�ٶȣ������������1��1ƥ�䣩
void UpdatePeople(int peopleNum, double* histBins, unsigned long time, double speed, pplList L);
//�������壨����ʱ�䡢�ٶȣ�������ֱ��ͼ����������Ⱥ���1��1ƥ�䣩
void UpdatePeopleExHist(int peopleNum, unsigned long time, double speed, pplList L);
//��ȡֱ��ͼ��Ϣ
void GetPeopleHist(int peopleNum, double* histBins, pplList L);
//ɾ������
void DeletePeople(int peopleNum, pplList L);
//��ӡ����
void PrintPeopleList(pplList L);

/////////////////////////////////////////////////////////////////////////////
// �㷨����

//����ѧϰ
void BgLearn(IplImage* pFrame, int frameNum, int watershed, double* mean, double* var);

//�����������£�����ȡǰ��
void BgDiff(IplImage* pFrame, double* mean, double* var, foreImg* fore);

//��ֵ�˲�
void MiddleFilter(foreImg* img, int filterH, int filterW, int filterMX, int filterMY);

//ȥ����Ӱ
void RemoveShadow(foreImg* fore);

//���ڵݹ����ͨ�������㷨
void LabelConnRgn(foreImg* fore);
void label(foreImg* fore, int i, int j, int nConnNum);	//��ǵ�������

//�����γ̵���ͨ�������㷨
void LabelConnByTour(struct foreImg* fore, struct tour* tours);

//��ȡ��ͨ�������Ϣ
void GetConnsInfo(foreImg* fore, connRgn* conns);
//��ȡ��ͨ�������Ϣ
void GetConnsInfoByTour(struct tour* tours, struct connRgn* conns);

//��ȡ�����������Ϣ
void GetPeopleConns(connRgn* conns, pplConn* currPeos, foreImg* fore);

//����ƥ��
void MatchPeople(pplConn* prePeos, pplConn* currPeos, foreImg* fore, pplList pList);

//����ɫ�ʵĸ���ֱ��ͼ����
void CreateHistBins(double* histBins, int nConnRgn, int nPixel, foreImg* fore);

//�ж�ƥ������
int GetMatchType(int matchTag, pplConn* prePeos, pplConn* currPeos);

//����ĳ����ģ����ĳ��ͨ�����ƥ��ϵ��
double MatchPeopleToConn(int peopleNum, int nConnRgn, int nPixel, pplList L, foreImg* fore);

/////////////////////////////////////////////////////////////////////////////
// ���ߺ���

//��ȡ��Ƶ�ļ��ķ�ˮ��
int GetFrameWaterShed(int videoType);

//��ȡ��Ƶ�ļ���·��
CString GetFilePath(int videoType);

//��ȡʱ���
int GetTimeDiff(LARGE_INTEGER t2, LARGE_INTEGER t1, LARGE_INTEGER tc);

/////////////////////////////////////////////////////////////////////////////
// ��ͼ����

//����ǰ������ IplImage �ṹ��
void DrawForeImg(foreImg * fore, IplImage* pForeImg);

//�������嵽 IplImage �ṹ��
void DrawPeople(pplConn* ppl, foreImg* fore, IplImage* pForeImg);				//��������
void DrawRect(IplImage* img, int up, int down, int left, int right);			//������������ı߽��
void DrawPeopleForeImg(foreImg * fore, IplImage* pForeImg, char nConnRgn);		//�������������ǰ������
void DrawPeopleNum(IplImage* pForeImg, int peopleNum, int left, int bottom);	//��������ı��
void DrawGroupNum(IplImage* pForeImg, int groupNum, int* peopleNum, int left, int bottom);	//����Ⱥ��ı��
void DrawDirection(IplImage* pForeImg, int direction, int left, int bottom);	//��������������˶�����
void DrawSpeed(IplImage* pForeImg, double speed, int left, int bottom);			//��������������ٶ�

//��ʾ IplImage ͼ�� picture �ؼ���
void ShowImg(IplImage* pForeImage, int repID);

bool checkEnterRect(_DetectRect rect, pplConn* people);
bool checkEnterMultiRect(pplConn* people);
bool checkLeaveRect(_DetectRect rect, pplConn* people);

void drawAllRects(IplImage* pFrame);

void TotalRectMend(_DetectRect* detectRectArray);
#endif