// Curve.h : Curve DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCurveApp
// �йش���ʵ�ֵ���Ϣ������� Curve.cpp
//

class CCurveApp : public CWinApp
{
public:
	CCurveApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnImageCurve();
};
