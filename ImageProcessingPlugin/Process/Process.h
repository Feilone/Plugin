// Process.h : Process DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CProcessApp
// �йش���ʵ�ֵ���Ϣ������� Process.cpp
//

class CProcessApp : public CWinApp
{
public:
	CProcessApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnImageInverse();
};
