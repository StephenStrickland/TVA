// FinalTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FinalTest.h"
#include "FinalTestDlg.h"

/////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���

// ��ǰ֡��
int g_frameNum;

// ֡��ʱ����
float g_frameTimeGap = 2;

//������˹ģ��
static double mean[PIXEL_BYTES_NUM];	//RGB��ֵ
static double var[PIXEL_BYTES_NUM];		//RGB����

//ǰ��ͼ��
static foreImg fore[PIXEL_NUM];

//��ͨ����
static tour tours[240*160];
connRgn conns[MAX_CONN_RGN];

//��ͨ������
int connNum;
//�γ̱��
int tourNum;

//�������ͨ����
static pplConn currPeos[MAX_TRACKING_PEOPLE];	//��ǰ֡��������Ϣ
static pplConn prePeos[MAX_TRACKING_PEOPLE];	//��һ֡��������Ϣ

//������
int peopleNum;

int firstFlag = 0;

//Ⱥ����
int groupNum;

//����ģ��
pplList pList;

//ֱ���������Ʊ���
CRect pictureRectA;
CRect pictureRectB;
_DetectRect detectRect[16];	// 16������
int usedDetectRectNum = 0;
int leftBorder = 319;
POINT linePos[4];
int clickTimes = 0;
CRect rect;
BOOL drawFlag = FALSE;
extern bool isRectConfirmed;

// ��Ƶ�ļ���·��
CString videoPath;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinalTestDlg dialog

CFinalTestDlg::CFinalTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFinalTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFinalTestDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFinalTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFinalTestDlg)
	DDX_Control(pDX, IDC_RADIO1, m_orientationSelect);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFinalTestDlg, CDialog)
	//{{AFX_MSG_MAP(CFinalTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_PAUSE_CONTINUE, OnButtonPauseContinue)
	ON_BN_CLICKED(IDC_BUTTON_DRAW, OnButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, OnButtonRotate)
	ON_BN_CLICKED(IDC_BUTTON_ABRASE, OnButtonAbrase)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
	ON_BN_CLICKED(IDC_CHECK13, OnCheck13)
	ON_BN_CLICKED(IDC_CHECK14, OnCheck14)
	ON_BN_CLICKED(IDC_CHECK15, OnCheck15)
	ON_BN_CLICKED(IDC_CHECK16, OnCheck16)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnButtonMoveup)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, OnButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_LEFT, OnButtonMoveLeft)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_RIGHT, OnButtonMoveRight)
	ON_BN_CLICKED(IDC_BUTTON_RECT1, OnButtonRect1)
	ON_BN_CLICKED(IDC_BUTTON_RECT2, OnButtonRect2)
	ON_BN_CLICKED(IDC_BUTTON_RECT3, OnButtonRect3)
	ON_BN_CLICKED(IDC_BUTTON_RECT4, OnButtonRect4)
	ON_BN_CLICKED(IDC_BUTTON_RECT5, OnButtonRect5)
	ON_BN_CLICKED(IDC_BUTTON_RECT6, OnButtonRect6)
	ON_BN_CLICKED(IDC_BUTTON_RECT7, OnButtonRect7)
	ON_BN_CLICKED(IDC_BUTTON_RECT8, OnButtonRect8)
	ON_BN_CLICKED(IDC_BUTTON_RECT9, OnButtonRect9)
	ON_BN_CLICKED(IDC_BUTTON_RECT10, OnButtonRect10)
	ON_BN_CLICKED(IDC_BUTTON_RECT11, OnButtonRect11)
	ON_BN_CLICKED(IDC_BUTTON_RECT12, OnButtonRect12)
	ON_BN_CLICKED(IDC_BUTTON_RECT13, OnButtonRect13)
	ON_BN_CLICKED(IDC_BUTTON_RECT14, OnButtonRect14)
	ON_BN_CLICKED(IDC_BUTTON_RECT15, OnButtonRect15)
	ON_BN_CLICKED(IDC_BUTTON_RECT16, OnButtonRect16)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinalTestDlg message handlers

BOOL CFinalTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_BUTTON_PAUSE_CONTINUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ABRASE)->EnableWindow(FALSE);
	
	m_orientationSelect.SetCheck(TRUE);

	m_slider.SetRange(0,100);
	m_slider.SetLineSize(10);

	GetDlgItem(IDC_EDIT_ANGLE)->SetWindowText("0");

	for (int i = 0; i < 16; i++)
	{
		detectRect[i].isDrew = FALSE;
		detectRect[i].rectNum = i + 1;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFinalTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFinalTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFinalTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CFinalTestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	//��ʼ��ȫ�ֱ���
	m_stopRun = 0;
	m_pThread = NULL;

	//ÿ��һ��ʱ���������
	SetTimer(1, 10, NULL);

	return 0;
}


void CFinalTestDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//��ʱ���½���
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}

//��ʼ��ȫ�ֱ���
void InitGlobalVars() 
{
	int i;

	//��ʼ����ֵ�ͷ�������
	for(i=0; i<PIXEL_BYTES_NUM; i++)
	{
		mean[i] = 0.0;
		var[i] = 0.0;
	}

	//��ʼ����һ֡�����������
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		prePeos[i].isPeople = false;
		prePeos[i].areaState = PEOPLE_NOTIN;
	}	

	peopleNum = 1;
	groupNum = 1;

	//��������ģ�͵Ŀ�����
	pList = CreatePeopleList();

	isRectConfirmed = false;
}

void InitVarsPerFrame(IplImage* pForeImg, foreImg* fore, pplConn* currPeos, tour* tours) 
{
	int i, j;

	//��ʼ��������ʾ��ͼ��
	for(i=0; i<PIXEL_BYTES_NUM; i++)
	{
		(*pForeImg).imageData[i] = 0;
	}
	
	//��ʼ��ǰ�����ؽṹ������
	for(i=0; i<PIXEL_NUM; i++)
	{
		fore[i].bValue = 0;
		fore[i].gValue = 0;
		fore[i].rValue = 0;
		fore[i].isFore = false;
		fore[i].isNumbered = false;
		fore[i].nConnRgn = 0;
	}
	
	//��ʼ����ǰ֡���������Ϣ
	for(i=0; i<MAX_TRACKING_PEOPLE; i++)
	{
		currPeos[i].up = 0;
		currPeos[i].down = 0;
		currPeos[i].left = 0;
		currPeos[i].right = 0;
		currPeos[i].area = 0;
		currPeos[i].nPixel = 0;
		
		currPeos[i].isPeople = false;
		currPeos[i].isGroup = false;
		
		for(j=0; j<MAX_TRACKING_PEOPLE; j++)
		{
			currPeos[i].peopleNum[j] = 0;
		}
		currPeos[i].groupNum = 0;
		
		currPeos[i].matchTag = 0;
		currPeos[i].direction = 0;
		
		currPeos[i].time = 0;
		currPeos[i].speed = 0.0;

		currPeos[i].areaState = PEOPLE_NOTIN;
	}
	
	//��ʼ���γ�
	for(i=0; i<240*160; i++)
	{
		tours[i].rowNum = 0;
		tours[i].start = 0;
		tours[i].end = 0;
		tours[i].connNum = 0;
		tours[i].isMatched = false;
	}
	
	tourNum = 0;
	connNum = 0;
}

//���ļ��ж�ȡ��Ƶ
UINT CaptureFrameFromFile()
{	
	int i, j;
	IplImage *pTempImg = NULL;
	
	CvSize size;
	size.width = WIDTH;
	size.height = HEIGHT;
	IplImage* pForeImg = cvCreateImage(size, 8, 3);
	IplImage* pFrame = cvCreateImage(size, 8, 3);

	CoInitialize(NULL);

	for (i = 0; i < 16; i++)
	{
		CEdit* pEdit = (CEdit*)AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_EDIT_RESULT + i);
		pEdit->SetWindowText("");
	}
	
	InitGlobalVars();
	
	// ���㲢�����������б��
	TotalRectMend(detectRect);

	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	mWin->GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
	
	unsigned long nFrame = 1;
	unsigned int watershed = 200;
	CvCapture* cap = cvCreateFileCapture(videoPath.GetBuffer(0));

	while(pTempImg = cvQueryFrame(cap))
	{
		// ��ͼ���������ת��
		for(i=0; i<HEIGHT; i++)
		{
			for(j=0; j<WIDTH; j++)
			{
				(*pFrame).imageData[(HEIGHT-1-i)*WIDTH*3+j*3] = (*pTempImg).imageData[i*WIDTH*3+j*3];
				(*pFrame).imageData[(HEIGHT-1-i)*WIDTH*3+j*3+1] = (*pTempImg).imageData[i*WIDTH*3+j*3+1];
				(*pFrame).imageData[(HEIGHT-1-i)*WIDTH*3+j*3+2] = (*pTempImg).imageData[i*WIDTH*3+j*3+2];
			}
		}

		// ��ʼ����
		if(nFrame <= watershed)
		{
			AfxGetApp()->GetMainWnd()->SetWindowText("Background Learning  --  " + videoPath);
			// ����ѧϰ
			BgLearn(pFrame, nFrame, watershed, mean, var);
		}
		else
		{
			AfxGetApp()->GetMainWnd()->SetWindowText("Tracking  --  " + videoPath);
			
			InitVarsPerFrame(pForeImg, fore, currPeos, tours);
			
			// ��ʾԭͼ��
			ShowImg(pFrame, IDC_PICTURE_A);
			
			// ������֡��������¡�ǰ����ȡ
			BgDiff(pFrame, mean, var, fore);
			
			// ��ֵ�˲�
			MiddleFilter(fore, 5, 5, 2, 2);
			
			// ȥ����Ӱ
			RemoveShadow(fore);
			
			// ��ͨ������
			LabelConnByTour(fore, tours);
			
			// ��ȡ��ͨ������Ϣ
			GetConnsInfoByTour(tours, conns);
			
			// ����ͨ��������ȡ������Ϣ
			GetPeopleConns(conns, currPeos, fore);
			
			// ����ƥ��
			MatchPeople(prePeos, currPeos, fore, pList);
			
			// ����������Ϣ
			DrawPeople(currPeos, fore, pFrame);
			
			// ��ʾ������ͼ��
			ShowImg(pFrame, IDC_PICTURE_B);
			
			// ��ʱ
			int slidervalue = mWin->m_slider.GetPos();							
			Sleep(slidervalue);
		}

		//��һ��ͼƬ
		nFrame++;
		g_frameNum = nFrame;
	}
	
	//�̺߳�������
	//���ÿ�ʼ��ť
	mWin->GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
	mWin->GetDlgItem(IDC_BUTTON_PAUSE_CONTINUE)->EnableWindow(FALSE);
	mWin->GetDlgItem(IDC_BUTTON_DRAW)->EnableWindow(TRUE);
	
	//�ͷ�ͼ����Դ
	cvReleaseImage(&pForeImg);
	cvReleaseImage(&pFrame);
	cvReleaseCapture(&cap);

	AfxGetApp()->GetMainWnd()->SetWindowText("Tracking Finished  --  " + videoPath);

	return 0;
}

void CFinalTestDlg::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
	//������Ƶ�ļ��Ĵ����߳�
	firstFlag = 0;

	GetDlgItem(IDC_BUTTON_DRAW)->EnableWindow(FALSE);

	m_pThread = AfxBeginThread((AFX_THREADPROC)CaptureFrameFromFile, NULL);
	GetDlgItem(IDC_BUTTON_PAUSE_CONTINUE)->EnableWindow(TRUE);
}

void CFinalTestDlg::OnButtonPauseContinue() 
{
	// TODO: Add your control notification handler code here
	static int n1 = 0;
	n1 ++;
	if (n1%2 == 1)
	{
		m_pThread->SuspendThread();
	}
	else
	{
		m_pThread->ResumeThread();
	}
}

void CFinalTestDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CFinalTestDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
}

// ������Ƶ
void CFinalTestDlg::OnButtonLoad() 
{
	CFileDialog fileDlg(TRUE);
	if (IDOK == fileDlg.DoModal())
	{   
		videoPath = fileDlg.GetPathName();
	}
	
	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	CWnd *pic = AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_PICTURE_A);

	// ��ʾͼƬ
	CvCapture* cap = cvCreateFileCapture(videoPath.GetBuffer(0));
	IplImage* pImage = cvQueryFrame(cap);
	ShowImg(pImage, IDC_PICTURE_A);

	AfxMessageBox("Draw the diagonal points of the rect");

	for (int i = 0; i < 16; i++)
	{
		detectRect[i].isDrew = FALSE;
		GetDlgItem(IDC_CHECK1 + i)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_BUTTON_DRAW)->EnableWindow(TRUE);
	cvReleaseCapture(&cap);

	AfxGetApp()->GetMainWnd()->SetWindowText("Moving Objects Detecting and Tracking System  --  " + videoPath);
}

void CFinalTestDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (drawFlag){	

		if (usedDetectRectNum < 16)
		{
			UpdateData(TRUE);

			// ��ȡ���pictureA�ؼ����꣬�ؼ���
			GetDlgItem(IDC_PICTURE_A)->GetWindowRect(&pictureRectA);
			ScreenToClient(&pictureRectA);

			CPoint pos(point.x - pictureRectA.left, point.y - pictureRectA.top);

			// ��ֹ���Ƴ����߽�
			if (pos.x < 1){
				pos.x = 0;
			}
			if (pos.y < 1){
				pos.y = 0;
			}
			if (pos.x > pictureRectA.Width() - 1){
				pos.x = pictureRectA.Width() - 1;
			}
			if (pos.y > pictureRectA.Height() - 1){
				pos.y = pictureRectA.Height() - 1;
			}
			
			// ��������һ�Σ�ȷ�Ͽ����Ͻǵ�
			if (clickTimes == 0){
				linePos[0] = pos;
				m_ptLeftTop.x = pos.x + pictureRectA.left;
				m_ptLeftTop.y = pos.y + pictureRectA.top;

				CPen pen(PS_SOLID, 3, RGB(0,0,255));
				CClientDC pDC(this);
				CPen* oldPen = pDC.SelectObject(&pen); 
				pDC.MoveTo(m_ptLeftTop);
				pDC.LineTo(m_ptLeftTop);
			}
			// �ڶ�����ȷ�������½ǵ㣬ͬʱ�Զ���������ת�����������������
			else if (clickTimes == 1){
				// �������½ǵ�
				linePos[2] = pos;

				// �������Ͻǵ�
				linePos[1].x = linePos[2].x;
				linePos[1].y = linePos[0].y;

				// �������½ǵ�
				linePos[3].x = linePos[0].x;
				linePos[3].y = linePos[2].y;

				// �����ĸ���������꣨ͼƬ���������꣩
				for (int i = 0; i < 4; i++)
				{ 
					linePos[i].x = linePos[i].x * WIDTH / pictureRectA.Width();
					linePos[i].y = linePos[i].y * HEIGHT / pictureRectA.Height();
				}
			
				// ����¿�ֵ����������
				int enterType = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO4) - IDC_RADIO1;
				for (int k = 0; k < 16; k ++)
				{
					if (! GetDlgItem(IDC_CHECK1 + k)->IsWindowEnabled())
					{
						break;
					}
				}
				InitRectCalc(linePos, enterType, &detectRect[k]);

				// ��������ת���򣨳�ʼ����
				DrawDetectRect(detectRect[k], IDC_PICTURE_A, 0);

				// �˴λ��ƽ���
				drawFlag = FALSE;

				// �����ڴ˷����������ת
				GetDlgItem(IDC_BUTTON_ROTATE)->EnableWindow(TRUE);
				GetDlgItem(IDC_CHECK1 + k)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_DRAW)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ABRASE)->EnableWindow(TRUE);

				usedDetectRectNum ++;
			}

			clickTimes ++;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CFinalTestDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonUp(nFlags, point);
}

void CFinalTestDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);

	// TODO: Add your message handler code here and/or call default
	if (drawFlag){	

		if(usedDetectRectNum < 16){
			HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
			::SetCursor(hCur);
		}
	}
}

BOOL CFinalTestDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (drawFlag){
		if (pMsg->message == WM_LBUTTONDOWN && GetDlgItem(IDC_PICTURE_A)->GetSafeHwnd() == pMsg->hwnd)
		{
			OnLButtonDown(MK_LBUTTON, pMsg->pt);
		}
		
		if (pMsg->message == WM_LBUTTONUP && GetDlgItem(IDC_PICTURE_A)->GetSafeHwnd() == pMsg->hwnd)
		{
			OnLButtonUp(MK_LBUTTON, pMsg->pt);
		}
		
		if (pMsg->message == WM_MOUSEMOVE && GetDlgItem(IDC_PICTURE_A)->GetSafeHwnd() == pMsg->hwnd)
		{
			OnMouseMove(MK_LBUTTON, pMsg->pt);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CFinalTestDlg::OnButtonDraw() 
{
	// TODO: Add your control notification handler code here	
	if (usedDetectRectNum < 16)
	{
		clickTimes = 0;
		drawFlag = TRUE;
		GetDlgItem(IDC_BUTTON_DRAW)->EnableWindow(FALSE);
	}
	else{
		AfxMessageBox("Maximum number of rects has been draw!");
		GetDlgItem(IDC_BUTTON_DRAW)->EnableWindow(FALSE);
	}
}

BOOL CFinalTestDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}

void CFinalTestDlg::OnButtonRotate() 
{
	int i;
	//_Point tmpPos[4];

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CvCapture* cap = NULL;
	IplImage* pImage = NULL;

	// ��ȡ��תֵ
	CString angleValueStr;
    GetDlgItem(IDC_EDIT_ANGLE)->GetWindowText(angleValueStr);
	int angleValue = atoi(angleValueStr);

	//������ת�˵Ŀ�ڵ�����
	for (i = 0; i < 16; i++)
	{
		if( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i) ){
			if (detectRect[i].isDrew == TRUE)
			{
				RotateRectCalc(&detectRect[i], angleValue);
			}
		}
	}
	
	//�ػ�ͼƬ
	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	CWnd *pic = AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_PICTURE_A);
	
	// ��ȡͼƬ
	for (i = 0; i < 16; i++)
	{
		if (detectRect[i].isDrew == TRUE)
		{
			// ��ʾͼƬ
			cap = cvCreateFileCapture(videoPath.GetBuffer(0));
			pImage = cvQueryFrame(cap);
			ShowImg(pImage, IDC_PICTURE_A);
			break;
		}
	}
	
	//�����������ػ����о���
	for (i = 0; i < 16; i++)
	{
		if(detectRect[i].isDrew == TRUE)
		{
			if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i))
			{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 1);
			}
			else{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 0);
			}
		}
	}

	cvReleaseCapture(&cap);
}

void CFinalTestDlg::OnButtonAbrase() 
{
	int i;
	CvCapture* cap = NULL;
	IplImage* pImage = NULL;

	//���½������ʾ
	//�ػ�ͼƬ
	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	CWnd *pic = AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_PICTURE_A);
	
	//��ȡͼƬ
	for (i = 0; i < 16; i++)
	{
		if (detectRect[i].isDrew == TRUE)
		{
			// ��ʾͼƬ
			cap = cvCreateFileCapture(videoPath.GetBuffer(0));
			pImage = cvQueryFrame(cap);
			ShowImg(pImage, IDC_PICTURE_A);
			break;
		}
	}
	
	//ɾ��ѡ�о���
	for (i = 0; i < 16; i++)
	{
		if( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i) ){
			if (detectRect[i].isDrew == TRUE)
			{
				detectRect[i].isDrew = FALSE;
				CButton* p = (CButton*)GetDlgItem(IDC_CHECK1 + i);
				p->SetCheck(0);
				GetDlgItem(IDC_CHECK1 + i)->EnableWindow(FALSE);
				usedDetectRectNum --;
			}
		}
	}

	//�����������ػ����о���
	for (i = 0; i < 16; i++)
	{
		if(detectRect[i].isDrew == TRUE)
		{
			DrawDetectRect(detectRect[i], IDC_PICTURE_A, 0);
		}
	}

	cvReleaseCapture(&cap);
}

void CFinalTestDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))
	{
		DrawDetectRect(detectRect[0], IDC_PICTURE_A, 1);
	}
	else{
		DrawDetectRect(detectRect[0], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2))
	{	
		DrawDetectRect(detectRect[1], IDC_PICTURE_A, 1);
	}
	else{
		DrawDetectRect(detectRect[1], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3))
	{
		DrawDetectRect(detectRect[2], IDC_PICTURE_A, 1);
	}
	else{
		DrawDetectRect(detectRect[2], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck4() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK4))
	{
		DrawDetectRect(detectRect[3], IDC_PICTURE_A, 1);		
	}
	else{
		DrawDetectRect(detectRect[3], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck5() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5))
	{
		DrawDetectRect(detectRect[4], IDC_PICTURE_A, 1);
	}
	else{
		DrawDetectRect(detectRect[4], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck6() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK6))
	{
		DrawDetectRect(detectRect[5], IDC_PICTURE_A, 1);
	}
	else{
		DrawDetectRect(detectRect[5], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck7() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK7))
	{
		DrawDetectRect(detectRect[6], IDC_PICTURE_A, 1);
	}
	else{
		DrawDetectRect(detectRect[6], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck8() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK8))
	{
		DrawDetectRect(detectRect[7], IDC_PICTURE_A, 1);		
	}
	else{
		DrawDetectRect(detectRect[7], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck9() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK9))
	{
		DrawDetectRect(detectRect[8], IDC_PICTURE_A, 1);
	}
	else{
		DrawDetectRect(detectRect[8], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck10() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK10))
	{
		DrawDetectRect(detectRect[9], IDC_PICTURE_A, 1);
	}
	else{
		DrawDetectRect(detectRect[9], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck11() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK11))
	{
		DrawDetectRect(detectRect[10], IDC_PICTURE_A, 1);		
	}
	else{
		DrawDetectRect(detectRect[10], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck12() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK12))
	{
		DrawDetectRect(detectRect[11], IDC_PICTURE_A, 1);		
	}
	else{
		DrawDetectRect(detectRect[11], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck13() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK13))
	{
		DrawDetectRect(detectRect[12], IDC_PICTURE_A, 1);		
	}
	else{
		DrawDetectRect(detectRect[12], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck14() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK14))
	{
		DrawDetectRect(detectRect[13], IDC_PICTURE_A, 1);		
	}
	else{
		DrawDetectRect(detectRect[13], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck15() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK15))
	{
		DrawDetectRect(detectRect[14], IDC_PICTURE_A, 1);		
	}
	else{
		DrawDetectRect(detectRect[14], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnCheck16() 
{
	// TODO: Add your control notification handler code here
	// ָ�������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK16))
	{
		DrawDetectRect(detectRect[15], IDC_PICTURE_A, 1);		
	}
	else{
		DrawDetectRect(detectRect[15], IDC_PICTURE_A, 0);
	}
}

void CFinalTestDlg::OnButtonMoveup() 
{
	// TODO: Add your control notification handler code here
	int i;
	//_Point tmpPos[4];

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CvCapture* cap = NULL;
	IplImage* pImage = NULL;

	// ��ȡ��תֵ
	CString angleValueStr;
    GetDlgItem(IDC_EDIT_ANGLE)->GetWindowText(angleValueStr);
	int angleValue = atoi(angleValueStr);

	//������ת�˵Ŀ�ڵ�����
	for (i = 0; i < 16; i++)
	{
		if( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i) ){
			if (detectRect[i].isDrew == TRUE)
			{
				MoveDetectRect(&detectRect[i], RECT_MOVE_UP);
			}
		}
	}
	
	//�ػ�ͼƬ
	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	CWnd *pic = AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_PICTURE_A);
	
	// ��ȡͼƬ
	for (i = 0; i < 16; i++)
	{
		if (detectRect[i].isDrew == TRUE)
		{
			// ��ʾͼƬ
			cap = cvCreateFileCapture(videoPath.GetBuffer(0));
			pImage = cvQueryFrame(cap);
			ShowImg(pImage, IDC_PICTURE_A);
			break;
		}
	}
	
	//�����������ػ����о���
	for (i = 0; i < 16; i++)
	{
		if(detectRect[i].isDrew == TRUE)
		{
			if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i))
			{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 1);
			}
			else{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 0);
			}
		}
	}

	cvReleaseCapture(&cap);	
}

void CFinalTestDlg::OnButtonMoveDown() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int i;
	//_Point tmpPos[4];
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CvCapture* cap = NULL;
	IplImage* pImage = NULL;
	
	// ��ȡ��תֵ
	CString angleValueStr;
    GetDlgItem(IDC_EDIT_ANGLE)->GetWindowText(angleValueStr);
	int angleValue = atoi(angleValueStr);
	
	//������ת�˵Ŀ�ڵ�����
	for (i = 0; i < 16; i++)
	{
		if( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i) ){
			if (detectRect[i].isDrew == TRUE)
			{
				MoveDetectRect(&detectRect[i], RECT_MOVE_DOWN);
			}
		}
	}
	
	//�ػ�ͼƬ
	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	CWnd *pic = AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_PICTURE_A);
	
	// ��ȡͼƬ
	for (i = 0; i < 16; i++)
	{
		if (detectRect[i].isDrew == TRUE)
		{
			// ��ʾͼƬ
			cap = cvCreateFileCapture(videoPath.GetBuffer(0));
			pImage = cvQueryFrame(cap);
			ShowImg(pImage, IDC_PICTURE_A);
			break;
		}
	}
	
	//�����������ػ����о���
	for (i = 0; i < 16; i++)
	{
		if(detectRect[i].isDrew == TRUE)
		{
			if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i))
			{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 1);
			}
			else{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 0);
			}
		}
	}
	
	cvReleaseCapture(&cap);		
}

void CFinalTestDlg::OnButtonMoveLeft() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int i;
	//_Point tmpPos[4];
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CvCapture* cap = NULL;
	IplImage* pImage = NULL;
	
	// ��ȡ��תֵ
	CString angleValueStr;
    GetDlgItem(IDC_EDIT_ANGLE)->GetWindowText(angleValueStr);
	int angleValue = atoi(angleValueStr);
	
	//������ת�˵Ŀ�ڵ�����
	for (i = 0; i < 16; i++)
	{
		if( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i) ){
			if (detectRect[i].isDrew == TRUE)
			{
				MoveDetectRect(&detectRect[i], RECT_MOVE_LEFT);
			}
		}
	}
	
	//�ػ�ͼƬ
	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	CWnd *pic = AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_PICTURE_A);
	
	// ��ȡͼƬ
	for (i = 0; i < 16; i++)
	{
		if (detectRect[i].isDrew == TRUE)
		{
			// ��ʾͼƬ
			cap = cvCreateFileCapture(videoPath.GetBuffer(0));
			pImage = cvQueryFrame(cap);
			ShowImg(pImage, IDC_PICTURE_A);
			break;
		}
	}
	
	//�����������ػ����о���
	for (i = 0; i < 16; i++)
	{
		if(detectRect[i].isDrew == TRUE)
		{
			if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i))
			{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 1);
			}
			else{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 0);
			}
		}
	}
	
	cvReleaseCapture(&cap);	
}

void CFinalTestDlg::OnButtonMoveRight() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int i;
	//_Point tmpPos[4];
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CvCapture* cap = NULL;
	IplImage* pImage = NULL;
	
	// ��ȡ��תֵ
	CString angleValueStr;
    GetDlgItem(IDC_EDIT_ANGLE)->GetWindowText(angleValueStr);
	int angleValue = atoi(angleValueStr);
	
	//������ת�˵Ŀ�ڵ�����
	for (i = 0; i < 16; i++)
	{
		if( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i) ){
			if (detectRect[i].isDrew == TRUE)
			{
				MoveDetectRect(&detectRect[i], RECT_MOVE_RIGHT);
			}
		}
	}
	
	//�ػ�ͼƬ
	CFinalTestDlg *mWin = (CFinalTestDlg *)AfxGetApp()->m_pMainWnd;
	CWnd *pic = AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_PICTURE_A);
	
	// ��ȡͼƬ
	for (i = 0; i < 16; i++)
	{
		if (detectRect[i].isDrew == TRUE)
		{
			// ��ʾͼƬ
			cap = cvCreateFileCapture(videoPath.GetBuffer(0));
			pImage = cvQueryFrame(cap);
			ShowImg(pImage, IDC_PICTURE_A);
			break;
		}
	}
	
	//�����������ػ����о���
	for (i = 0; i < 16; i++)
	{
		if(detectRect[i].isDrew == TRUE)
		{
			if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1 + i))
			{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 1);
			}
			else{
				DrawDetectRect(detectRect[i], IDC_PICTURE_A, 0);
			}
		}
	}
	
	cvReleaseCapture(&cap);		
}

void CFinalTestDlg::OnButtonRect1() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(1);
}

void CFinalTestDlg::OnButtonRect2() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(2);	
}

void CFinalTestDlg::OnButtonRect3() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(3);	
}

void CFinalTestDlg::OnButtonRect4() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(4);	
}

void CFinalTestDlg::OnButtonRect5() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(5);
}

void CFinalTestDlg::OnButtonRect6() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(6);	
}

void CFinalTestDlg::OnButtonRect7() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(7);	
}

void CFinalTestDlg::OnButtonRect8() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(8);	
}

void CFinalTestDlg::OnButtonRect9() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(9);
}

void CFinalTestDlg::OnButtonRect10() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(10);
}

void CFinalTestDlg::OnButtonRect11() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(11);	
}

void CFinalTestDlg::OnButtonRect12() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(12);	
}

void CFinalTestDlg::OnButtonRect13() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(13);	
}

void CFinalTestDlg::OnButtonRect14() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(14);	
}

void CFinalTestDlg::OnButtonRect15() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(15);	
}

void CFinalTestDlg::OnButtonRect16() 
{
	// TODO: Add your control notification handler code here
	DispDetectRectInfo(16);	
}
