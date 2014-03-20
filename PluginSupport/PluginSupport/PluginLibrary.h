
#pragma once

#ifdef __cplusplus
	#define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
	#define PLUGIN_EXPORT __declspec(dllexport)
#endif

#ifndef PLUGIN_NO_AUTO_LIB
	#ifdef _DEBUG
		#pragma comment(lib, "..\\..\\PluginSupport\\Debug\\PluginSupport.lib")
		#pragma message("Automatically link with PluginSupport Debug")
	#else
		#pragma comment(lib, "..\\..\\PluginSupport\\Release\\PluginSupport.lib")
		#pragma message("Automatically link with PluginSupport Release")
	#endif
#endif

PLUGIN_EXPORT int MergeMenu(int nPluginIndex, const CMenu* pMenuAdd, BOOL bTopLevel = FALSE);

PLUGIN_EXPORT int MergeToolbar(int nPluginIndex, UINT nIDResource);

PLUGIN_EXPORT int InsertToolbar(int nPluginIndex, UINT nIDResource, CString strText);

PLUGIN_EXPORT void PIEnableUI(CCmdUI* pCmdUI, BOOL bEnable);

PLUGIN_EXPORT void PICheckUI(CCmdUI* pCmdUI, BOOL bCheck);

PLUGIN_EXPORT void PIMessageBox(CString str, UINT uType = MB_OK | MB_ICONINFORMATION);

PLUGIN_EXPORT void PILog(CString str);

PLUGIN_EXPORT void PIPrepareDC(CView* pView, CDC* pDC);

// register message handler
typedef LRESULT (__cdecl* MESSAGEPROC)(MSG* pMsg);
PLUGIN_EXPORT void PIRegisterMessage(int nPluginIndex, UINT nMessage, MESSAGEPROC pFN);

PLUGIN_EXPORT void PIUnregisterMessage(int nPluginIndex, UINT nMessage);

PLUGIN_EXPORT CDocument* PIGetActiveDocument();

PLUGIN_EXPORT CView* PIGetActiveView();

#ifndef PLUGIN_EXT_CLASS
	#ifdef _AFXDLL
		#define PLUGIN_EXT_CLASS	AFX_CLASS_EXPORT
	#else
		#define PLUGIN_EXT_CLASS	// not used with static builds
	#endif
#endif

#include "Plugin.h"
//#include "PIMultiDocTemplate.h"
#include "PIDocument.h"