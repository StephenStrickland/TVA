#ifndef _MYPROC_H_
#define _MYPROC_H_

#include "FinalTestDlg.h"

/////////////////////////////////////////////////////////////////////////////
// 宏定义

//定义图像信息
#define WIDTH 320	//图像的宽
#define HEIGHT 240	//图像的高
#define PIXEL_NUM (WIDTH * HEIGHT)	//像素数目
#define PIXEL_BYTES_NUM (PIXEL_NUM * 3)	//像素字节数

//定义各种系数
#define BG_UPDATE_WEIGHT 0.9	//背景更新系数
#define HIST_UPDATE_WEIGHT 0.8	//色彩直方图更新系数

//定义能够检测到的连通区域的个数
#define  MAX_CONN_RGN 30
//定义能够跟踪的人数上限
#define MAX_TRACKING_PEOPLE 20

//定义各种异常的阈值
#define ABNORMAL_STAY_TIME 5000	//逗留时间异常
#define ABNORMAL_SPEED 50000	//移动速度

//定义各种阈值
#define THRESHOLD_HUMAN_PIXEL_NUM 500	//对应于人体的连通区域的像素数目的阈值
#define THRESHOLD_MATCH	0.75	//匹配系数的阈值

//定义视频文件的类型
#define VIDEO_TYPE_1P  0
#define VIDEO_TYPE_2P  1
#define VIDEO_TYPE_3P  2

//移动方向
#define MOVE_LEFT  0	//左移
#define MOVE_STOP  1	//停止移动
#define MOVE_RIGHT 2	//右移

//匹配类型
#define MATCH_TYPE_0T1	0	//0对1
#define MATCH_TYPE_1T0	1	//1对0
#define MATCH_TYPE_1T1	2	//1对1
#define MATCH_TYPE_MT1	3	//多对1
#define MATCH_TYPE_1TM	4	//1对多

// 区域边界
#define AREA_LEFT  120	// 区域左边界
#define AREA_RIGHT 200	// 区域右边界

// 是否进入区域的状态标识
#define PEOPLE_NOTIN   0	// 尚未进入区域
#define PEOPLE_IN      1	// 处于区域中
#define PEOPLE_OUT     2	// 离开区域

/////////////////////////////////////////////////////////////////////////////
// 数据结构定义

//点结构体
struct _Point{
	double x;
	double y;
};
//直线结构体
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
//矩形结构体
struct _DetectRect{
	_Line enterLine;
	_Line leaveLine[3];
	_Point corners[4];
	BOOL isDrew;
	int enterType;
	int rectNum;
	int currentLeaveLine;	// 当前被激活的离开line
	int delayCount;		// 延时时间
};

#define RECT_MOVE_LEFT 0
#define RECT_MOVE_UP 1
#define RECT_MOVE_RIGHT 2
#define RECT_MOVE_DOWN 3

//计算矩形内容
void InitRectCalc(POINT* linePos, int enterType, _DetectRect* newRect);
//计算旋转公式
void RotatePointCalc(_Point pSource, _Point* pResult, _Point center, long double angle);
//计算旋转后矩形四个角坐标
void RotateRectCalc(_DetectRect* rotateRect, int angleDu);
//计算矩形四条边斜率截距
void ABRectCalc(_Line* rectLine);
//16个矩形修正
void TotalRectMend(_DetectRect* detectRectArray);
//绘制检测区域
void DrawDetectRect(_DetectRect dRect, int rectID, int drawMode);
//平移检测框
void MoveDetectRect(_DetectRect* moveRect, int moveMode);
//显示检测框信息
void DispDetectRectInfo(int rectID);


//前景图像
struct foreImg{
	//B、G、R值
	unsigned char bValue;
	unsigned char gValue;
	unsigned char rValue;
	//前景标记
	bool isFore;
	//是否已被编号
	bool isNumbered;
	//连通区域的编号
	char nConnRgn;
};

//连通区域
struct connRgn{
	//像素数目
	int nPixel;
	//上边界
	int up;
	//下边界
	int down;
	//左边界
	int left;
	//右边界
	int right;
	//连通区域的编号
	char nConnRgn;
};

//游程结构体
struct tour{
	int rowNum;	//行号
	int start;		//起始列号
	int end;		//终止列号
	int connNum;	//连通区域标号
	bool isMatched;			//是否已匹配
};

//人体连通区域
struct pplConn{

	int nPixel; //像素总数
	int up; 	//上边界
	int down;	//下边界
	int left;	//左边界
	int right;	//右边界
	int area;	//边界框的面积

	bool isPeople;	//是否为人体
	bool isGroup;	//是否为群体

	int peopleNum[MAX_TRACKING_PEOPLE];	//人体编号
	int groupNum;	//群组编号

	int matchTag;	//匹配标号

	char nConnRgn;	//对应的连通区域的编号

	long direction;	//运动方向

	unsigned long time;	//当前的时间
	double speed;	//运动速度（单位：像素/秒）

	//区域的色彩直方图模型
	double histBins[4096];

	int areaState;	// 区域状态
};

/////////////////////////////////////////////////////////////////////////////
// 人体模型链表及其操作函数

//人体模型
struct peopleModel{
	int peopleNum;	//人体编号
	double histBins[4096];	//人体的彩色直方图模型
	unsigned long appearTime;	//出现时间
	unsigned long stayTime;	//逗留时间
	double speed;	//速度
	peopleModel* nextPeople;	//指向下一个人体模型
};
typedef peopleModel pplModel;	//单个人体
typedef peopleModel* pplList;	//表的首地址
typedef peopleModel* pplNode;	//表中某结点的首地址

//创建空的人体列表
pplList CreatePeopleList();
//增加人体
void AddPeople(int peopleNum, double* histBins, unsigned long time, pplList L);
//更新人体（更新直方图、时间、速度，适用于人体的1对1匹配）
void UpdatePeople(int peopleNum, double* histBins, unsigned long time, double speed, pplList L);
//更新人体（更新时间、速度，不更新直方图——适用于群组的1对1匹配）
void UpdatePeopleExHist(int peopleNum, unsigned long time, double speed, pplList L);
//获取直方图信息
void GetPeopleHist(int peopleNum, double* histBins, pplList L);
//删除人体
void DeletePeople(int peopleNum, pplList L);
//打印人体
void PrintPeopleList(pplList L);

/////////////////////////////////////////////////////////////////////////////
// 算法函数

//背景学习
void BgLearn(IplImage* pFrame, int frameNum, int watershed, double* mean, double* var);

//背景差分与更新，以提取前景
void BgDiff(IplImage* pFrame, double* mean, double* var, foreImg* fore);

//中值滤波
void MiddleFilter(foreImg* img, int filterH, int filterW, int filterMX, int filterMY);

//去除阴影
void RemoveShadow(foreImg* fore);

//基于递归的连通区域标记算法
void LabelConnRgn(foreImg* fore);
void label(foreImg* fore, int i, int j, int nConnNum);	//标记单个区域

//基于游程的连通区域标记算法
void LabelConnByTour(struct foreImg* fore, struct tour* tours);

//提取连通区域的信息
void GetConnsInfo(foreImg* fore, connRgn* conns);
//提取连通区域的信息
void GetConnsInfoByTour(struct tour* tours, struct connRgn* conns);

//提取人体区域的信息
void GetPeopleConns(connRgn* conns, pplConn* currPeos, foreImg* fore);

//人体匹配
void MatchPeople(pplConn* prePeos, pplConn* currPeos, foreImg* fore, pplList pList);

//创建色彩的概率直方图容器
void CreateHistBins(double* histBins, int nConnRgn, int nPixel, foreImg* fore);

//判断匹配类型
int GetMatchType(int matchTag, pplConn* prePeos, pplConn* currPeos);

//计算某人体模型与某连通区域的匹配系数
double MatchPeopleToConn(int peopleNum, int nConnRgn, int nPixel, pplList L, foreImg* fore);

/////////////////////////////////////////////////////////////////////////////
// 工具函数

//获取视频文件的分水岭
int GetFrameWaterShed(int videoType);

//获取视频文件的路径
CString GetFilePath(int videoType);

//获取时间差
int GetTimeDiff(LARGE_INTEGER t2, LARGE_INTEGER t1, LARGE_INTEGER tc);

/////////////////////////////////////////////////////////////////////////////
// 绘图函数

//绘制前景区域到 IplImage 结构中
void DrawForeImg(foreImg * fore, IplImage* pForeImg);

//绘制人体到 IplImage 结构中
void DrawPeople(pplConn* ppl, foreImg* fore, IplImage* pForeImg);				//绘制人体
void DrawRect(IplImage* img, int up, int down, int left, int right);			//绘制人体区域的边界框
void DrawPeopleForeImg(foreImg * fore, IplImage* pForeImg, char nConnRgn);		//绘制人体区域的前景像素
void DrawPeopleNum(IplImage* pForeImg, int peopleNum, int left, int bottom);	//绘制人体的编号
void DrawGroupNum(IplImage* pForeImg, int groupNum, int* peopleNum, int left, int bottom);	//绘制群组的编号
void DrawDirection(IplImage* pForeImg, int direction, int left, int bottom);	//绘制人体区域的运动方向
void DrawSpeed(IplImage* pForeImg, double speed, int left, int bottom);			//绘制人体区域的速度

//显示 IplImage 图像到 picture 控件上
void ShowImg(IplImage* pForeImage, int repID);

bool checkEnterRect(_DetectRect rect, pplConn* people);
bool checkEnterMultiRect(pplConn* people);
bool checkLeaveRect(_DetectRect rect, pplConn* people);

void drawAllRects(IplImage* pFrame);

void TotalRectMend(_DetectRect* detectRectArray);
#endif