// CurveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Curve.h"
#include "CurveDlg.h"
#include "afxdialogex.h"


// CCurveDlg �Ի���

IMPLEMENT_DYNAMIC(CCurveDlg, CDialogEx)

CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCurveDlg::IDD, pParent)
{

}

CCurveDlg::~CCurveDlg()
{

}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CCurveDlg ��Ϣ�������

int CCurveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect(0, 0, 200, 300);
	m_wndCurve.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, 1000);

	return 0;
}

BOOL CCurveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
