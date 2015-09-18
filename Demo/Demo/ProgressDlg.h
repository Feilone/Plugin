#pragma once
#include "afxcmn.h"


// CProgressDlg �Ի���

class CProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgressDlg();

public:
	void SetPercent(int nPercent);

	void SetCaption(CString strCaption)		{ m_strCaption = strCaption; }

private:
	CString m_strCaption;

public:
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
