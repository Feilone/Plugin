// TreeViewPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PluginBrowser.h"
#include "TreeViewPage.h"
#include "afxdialogex.h"


// CTreeViewPage �Ի���

IMPLEMENT_DYNAMIC(CTreeViewPage, CDialogEx)

CTreeViewPage::CTreeViewPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTreeViewPage::IDD, pParent)
{

}

CTreeViewPage::~CTreeViewPage()
{

}

void CTreeViewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGIN_TREE, m_ctrlPluginTree);
}


BEGIN_MESSAGE_MAP(CTreeViewPage, CDialogEx)
END_MESSAGE_MAP()


// CTreeViewPage ��Ϣ�������

void CTreeViewPage::DisplayPluginInfo(const PluginInfoArray& array)
{
	for (int i=0; i<array.GetSize(); i++)
	{
		CPluginInfo* pInfo = array.GetAt(i);

	}
}

BOOL CTreeViewPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	m_ctrlPluginTree.MoveWindow(rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
