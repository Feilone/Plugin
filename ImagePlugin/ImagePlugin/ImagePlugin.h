// ImagePlugin.h : ImagePlugin DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImagePluginApp
// �йش���ʵ�ֵ���Ϣ������� ImagePlugin.cpp
//

class CImagePluginApp : public CWinApp
{
public:
	CImagePluginApp();

public:
	CPIMultiDocTemplate* GetDocTemplate()	{ return m_pDocTemplate; }

private:
	CPIMultiDocTemplate* m_pDocTemplate;

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
