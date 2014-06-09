// ProgressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CProgressDlg �Ի���

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{

}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CProgressDlg ��Ϣ�������

BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CProgressDlg::SetPercent(int nPercent)
{
	if (nPercent <= 100)
	{
		m_ctrlProgress.SetPos(nPercent);
	}
	else
	{
		SendMessage(WM_CLOSE);
	}
}

void CProgressDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	CDemoApp* pApp = (CDemoApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	pMainFrame->SetPluginThreadRunning(FALSE);

	CDialog::OnCancel();
}

LRESULT CProgressDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return HTCAPTION;
	return CDialog::OnNcHitTest(point);
}