#pragma once


// CCurveDlg �Ի���

class CCurveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCurveDlg)

public:
	CCurveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCurveDlg();

// �Ի�������
	enum { IDD = IDD_CURVE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
