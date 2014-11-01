// LevelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Level.h"
#include "resource.h"
#include "LevelDlg.h"
#include "afxdialogex.h"


// CLevelDlg �Ի���

IMPLEMENT_DYNAMIC(CLevelDlg, CDialogEx)

CLevelDlg::CLevelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLevelDlg::IDD, pParent)
{

}

CLevelDlg::~CLevelDlg()
{

}

void CLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLevelDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CLevelDlg ��Ϣ�������

int CLevelDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect Rect;
	GetClientRect(&Rect);
	if (m_wndLevel.GetSafeHwnd()==NULL)
	{
		m_wndLevel.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, Rect, this, IDW_LEVEL);
	}

	return 0;
}

BOOL CLevelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLevelDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	m_wndLevel.MoveWindow(CRect(0, 0, cx, cy));
}

BOOL CLevelDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

//	return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}
