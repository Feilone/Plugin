#pragma once
#include "afxcmn.h"


// CProgressDlg �Ի���

class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgressDlg();

public:
	void SetPercent(int nPercent);

// �Ի�������
	enum { IDD = IDD_PROGRESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ctrlProgress;
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
