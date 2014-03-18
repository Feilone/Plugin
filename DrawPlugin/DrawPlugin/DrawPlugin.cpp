// DrawPlugin.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DrawPlugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CDrawPluginApp

BEGIN_MESSAGE_MAP(CDrawPluginApp, CWinApp)
	ON_COMMAND(ID_DRAW_LINE, &CDrawPluginApp::OnDrawLine)
	ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, &CDrawPluginApp::OnUpdateDrawLine)
	ON_COMMAND(ID_DRAW_STROKE, &CDrawPluginApp::OnDrawStroke)
	ON_UPDATE_COMMAND_UI(ID_DRAW_STROKE, &CDrawPluginApp::OnUpdateDrawStroke)
	ON_COMMAND(ID_DRAW_RECTANGLE, &CDrawPluginApp::OnDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, &CDrawPluginApp::OnUpdateDrawRectangle)
	ON_COMMAND(ID_DRAW_ELLIPSE, &CDrawPluginApp::OnDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, &CDrawPluginApp::OnUpdateDrawEllipse)
END_MESSAGE_MAP()

// CDrawPluginApp ����

CDrawPluginApp::CDrawPluginApp()
{
	m_shpFlag = CShape::NONE;
}

// Ψһ��һ�� CDrawPluginApp ����

CDrawPluginApp theApp;


// CDrawPluginApp ��ʼ��

BOOL CDrawPluginApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int CDrawPluginApp::ExitInstance()
{
	ReleaseShape();

	return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////

CShape::shape CDrawPluginApp::GetCurrentShapeFlag()
{
	return m_shpFlag;
}

void CDrawPluginApp::SetCurrentShapeFlag(CShape::shape shpFlag)
{
	m_shpFlag = shpFlag;
}

CShape* CDrawPluginApp::GetCurrentShape(CWnd* pWnd)
{
	if (pWnd == NULL)
	{
		return NULL;
	}

	std::map<CWnd*, CShape*>::iterator iter = m_ShapeMap.find(pWnd);

	if (iter == m_ShapeMap.end())
	{
		return NULL;
	}
	else
	{
		return iter->second;
	}
}

CShape* CDrawPluginApp::NewShape(CWnd* pWnd)
{
	CShape* pShape = NULL;

	std::map<CWnd*, CShape*>::iterator iter = m_ShapeMap.find(pWnd);
	if (iter != m_ShapeMap.end())
	{
		pShape = iter->second;
	}

	if (pShape)
	{
		delete pShape;
	}

	switch (m_shpFlag)
	{
	case CShape::NONE:
		break;
	case CShape::LINE:
		pShape = new CLine;
		break;
	case CShape::STROKE:
		pShape = new CStroke;
		break;
	case CShape::RECTANGLE:
		pShape = new CRectAngle;
		break;
	case CShape::ELLIPSE:
		pShape = new CEllipse;
		break;
	default:
		break;
	}

	if (iter != m_ShapeMap.end())
	{
		iter->second = pShape;
	}
	else
	{
		m_ShapeMap.insert(std::pair<CWnd*, CShape*>(pWnd, pShape));
	}
	return pShape;
}

void CDrawPluginApp::ReleaseShape()
{
	int mapSize = m_ShapeMap.size();

	std::map<CWnd*, CShape*>::iterator iter;

	for (iter=m_ShapeMap.begin();iter!=m_ShapeMap.end();iter++)
	{
		delete iter->second;
	}
	m_ShapeMap.clear();
}

LRESULT LButtonDown(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
	CView* pView = (CView*)pWnd;
	CClientDC dc(pView);
	PIPrepareDC(pView, &dc);

	CPoint point(pMsg->lParam);
	dc.DPtoLP(&point);

	pWnd->SetCapture();

	CDrawPluginApp* pApp = (CDrawPluginApp*)AfxGetApp();
	CShape* pShape = pApp->NewShape(pWnd);
	pShape->Init(point);

	return 1;
}

LRESULT MouseMove(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
	if (pWnd->GetCapture() == pWnd)
	{
		CView* pView = (CView*)pWnd;
		CClientDC dc(pView);
		PIPrepareDC(pView, &dc);

		CPoint point(pMsg->lParam);
		dc.DPtoLP(&point);

		CDrawPluginApp* pApp = (CDrawPluginApp*)AfxGetApp();
		pApp->GetCurrentShape(pView)->UpdatePoint(point);

		pWnd->Invalidate(FALSE);

		return 1;
	}
	return 0;
}

LRESULT LButtonUp(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
	if (pWnd->GetCapture() == pWnd)
	{
		CView* pView = (CView*)pWnd;
		CClientDC dc(pView);
		PIPrepareDC(pView, &dc);

		CPoint point(pMsg->lParam);
		dc.DPtoLP(&point);

		CDrawPluginApp* pApp = (CDrawPluginApp*)AfxGetApp();
		pApp->GetCurrentShape(pView)->UpdatePoint(point);

		pWnd->Invalidate(FALSE);

		ReleaseCapture();

		return 1;
	}

	return 0;
}

void CDrawPluginApp::ChangeMouseListenState()
{
	if (GetCurrentShapeFlag() == CShape::NONE)
	{
		PIUnregisterMessage(m_nPluginIndex, WM_LBUTTONDOWN);
		PIUnregisterMessage(m_nPluginIndex, WM_MOUSEMOVE);
		PIUnregisterMessage(m_nPluginIndex, WM_LBUTTONUP);
	}
	else
	{
		PIRegisterMessage(m_nPluginIndex, WM_LBUTTONDOWN, LButtonDown);
		PIRegisterMessage(m_nPluginIndex, WM_MOUSEMOVE, MouseMove);
		PIRegisterMessage(m_nPluginIndex, WM_LBUTTONUP, LButtonUp);
	}
}

void CDrawPluginApp::OnDrawLine()
{
	if (GetCurrentShapeFlag() == CShape::LINE)
	{
		SetCurrentShapeFlag(CShape::NONE);
	}
	else
	{
		SetCurrentShapeFlag(CShape::LINE);
	}
	ChangeMouseListenState();
}

void CDrawPluginApp::OnUpdateDrawLine(CCmdUI *pCmdUI)
{
	PICheckUI(pCmdUI, GetCurrentShapeFlag() == CShape::LINE);
}

void CDrawPluginApp::OnDrawStroke()
{
	if (GetCurrentShapeFlag() == CShape::STROKE)
	{
		SetCurrentShapeFlag(CShape::NONE);
	}
	else
	{
		SetCurrentShapeFlag(CShape::STROKE);
	}
	ChangeMouseListenState();
}


void CDrawPluginApp::OnUpdateDrawStroke(CCmdUI *pCmdUI)
{
	PICheckUI(pCmdUI, GetCurrentShapeFlag() == CShape::STROKE);
}

void CDrawPluginApp::OnDrawRectangle()
{
	if (GetCurrentShapeFlag() == CShape::RECTANGLE)
	{
		SetCurrentShapeFlag(CShape::NONE);
	}
	else
	{
		SetCurrentShapeFlag(CShape::RECTANGLE);
	}
	ChangeMouseListenState();
}

void CDrawPluginApp::OnUpdateDrawRectangle(CCmdUI *pCmdUI)
{
	PICheckUI(pCmdUI, GetCurrentShapeFlag() == CShape::RECTANGLE);
}

void CDrawPluginApp::OnDrawEllipse()
{
	if (GetCurrentShapeFlag() == CShape::ELLIPSE)
	{
		SetCurrentShapeFlag(CShape::NONE);
	}
	else
	{
		SetCurrentShapeFlag(CShape::ELLIPSE);
	}
	ChangeMouseListenState();
}

void CDrawPluginApp::OnUpdateDrawEllipse(CCmdUI *pCmdUI)
{
	PICheckUI(pCmdUI, GetCurrentShapeFlag() == CShape::ELLIPSE);
}
