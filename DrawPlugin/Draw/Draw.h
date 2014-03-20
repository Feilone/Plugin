// Draw.h : Draw DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include <map>
#include "Shape.h"

// CDrawApp
// �йش���ʵ�ֵ���Ϣ������� Draw.cpp
//

class CDrawApp : public CWinApp
{
public:
	CDrawApp();

public:
	CShape::shape GetCurrentShapeFlag();

	void SetCurrentShapeFlag(CShape::shape shpFlag);

	CShape* GetCurrentShape(CWnd* pWnd);

	// set current shape
	CShape* NewShape(CWnd* pWnd);

	void ReleaseShape();

	void ChangeMouseListenState();

public:
	// plugin index
	int m_nPluginIndex;

	// shape flag
	CShape::shape m_shpFlag;

	std::map<CWnd*, CShape*> m_ShapeMap;


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	afx_msg void OnDrawLine();
	afx_msg void OnUpdateDrawLine(CCmdUI *pCmdUI);
	afx_msg void OnDrawStroke();
	afx_msg void OnUpdateDrawStroke(CCmdUI *pCmdUI);
	afx_msg void OnDrawRectangle();
	afx_msg void OnUpdateDrawRectangle(CCmdUI *pCmdUI);
	afx_msg void OnDrawEllipse();
	afx_msg void OnUpdateDrawEllipse(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()
};
