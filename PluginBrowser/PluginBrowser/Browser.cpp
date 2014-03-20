#include "stdafx.h"
#include "Browser.h"
#include "PluginBrowser.h"

CBrowserPlugin::CBrowserPlugin()
{

}

void CBrowserPlugin::Init(CWinApp* pApp, int nPluginIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT(pApp != NULL);

	// merge menu
	CMenu append;
	append.LoadMenu(IDR_BROWSER_MENU);
	MergeMenu(nPluginIndex, &append, TRUE);
}

void CBrowserPlugin::Release()
{

}

void CBrowserPlugin::Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("PluginBrowser");
	plugininfo.m_strBlurb = _T("Show Plugin Information");
	plugininfo.m_strHelp = _T("PluginBrowser");
	plugininfo.m_strAuthor = _T("Rolin");
	plugininfo.m_strCopyRight = _T("Copyright Rolin");
	plugininfo.m_strDate = _T("2014.3.17");
	CString str;
	str.LoadString(IDS_STRING_MENU_LABEL);
	plugininfo.m_strMenuLabel = str;
}

void CBrowserPlugin::Draw(CDC* pDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}

int CBrowserPlugin::GetDocTemplateCount()
{
	return 0;
}

CPIMultiDocTemplate* CBrowserPlugin::GetDocTemplate(int nIndex)
{
	return NULL;
}

PLUGIN_EXPORT CBrowserPlugin* GetInstance()
{
	CBrowserPlugin* pInstance = CBrowserPlugin::Instance();
	return pInstance;
}