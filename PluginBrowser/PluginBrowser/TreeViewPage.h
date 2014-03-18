#pragma once
#include "afxcmn.h"


// CTreeViewPage �Ի���

typedef CArray<CPluginInfo*, CPluginInfo*> PluginInfoArray;

class CTreeViewPage : public CDialogEx
{
	DECLARE_DYNAMIC(CTreeViewPage)

public:
	CTreeViewPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTreeViewPage();

public:
	void DisplayPluginInfo(const PluginInfoArray& array);

public:
// �Ի�������
	enum { IDD = IDD_TREE_VIEW_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_ctrlPluginTree;
	virtual BOOL OnInitDialog();
};
