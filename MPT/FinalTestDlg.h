// FinalTestDlg.h : header file
//

#if !defined(AFX_FINALTESTDLG_H__BC83C153_31DB_4EEC_B770_D5DF150C623D__INCLUDED_)
#define AFX_FINALTESTDLG_H__BC83C153_31DB_4EEC_B770_D5DF150C623D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFinalTestDlg dialog


#include <highgui.h>
#include <windows.h>
#include <mmsystem.h>
#include "MyProc.h"
#include "resource.h"

class CFinalTestDlg : public CDialog
{
// Construction
public:
	CFinalTestDlg(CWnd* pParent = NULL);	// standard constructor

	//全局变量
	CvvImage sgImage;	//保存所要显示的图像
	CWinThread *m_pThread;	//工作线程指针
	int m_stopRun;	//线程暂停/启动标志位

// Dialog Data
	//{{AFX_DATA(CFinalTestDlg)
	enum { IDD = IDD_FINALTEST_DIALOG };
	CButton	m_orientationSelect;
	CButton	m_detectOrientation;
	CSliderCtrl	m_slider;
	CButton	m_RUN_STOP_FLAG;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFinalTestDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	
	CPoint m_ptLeftTop;
	CPoint m_ptRightBottom;
	CPoint m_ptLeftBottom;
	CPoint m_ptRightTop;


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFinalTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCancelMode();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonPauseContinue();
	afx_msg void OnButtonDraw();
	afx_msg void OnButtonRotate();
	afx_msg void OnButtonAbrase();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnCheck8();
	afx_msg void OnCheck9();
	afx_msg void OnCheck10();
	afx_msg void OnCheck11();
	afx_msg void OnCheck12();
	afx_msg void OnCheck13();
	afx_msg void OnCheck14();
	afx_msg void OnCheck15();
	afx_msg void OnCheck16();
	afx_msg void OnButtonMoveup();
	afx_msg void OnButtonMoveDown();
	afx_msg void OnButtonMoveLeft();
	afx_msg void OnButtonMoveRight();
	afx_msg void OnButtonRect1();
	afx_msg void OnButtonRect2();
	afx_msg void OnButtonRect3();
	afx_msg void OnButtonRect4();
	afx_msg void OnButtonRect5();
	afx_msg void OnButtonRect6();
	afx_msg void OnButtonRect7();
	afx_msg void OnButtonRect8();
	afx_msg void OnButtonRect9();
	afx_msg void OnButtonRect10();
	afx_msg void OnButtonRect11();
	afx_msg void OnButtonRect12();
	afx_msg void OnButtonRect13();
	afx_msg void OnButtonRect14();
	afx_msg void OnButtonRect15();
	afx_msg void OnButtonRect16();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINALTESTDLG_H__BC83C153_31DB_4EEC_B770_D5DF150C623D__INCLUDED_)
