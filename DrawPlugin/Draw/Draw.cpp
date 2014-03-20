// Draw.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Draw.h"

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

// CDrawApp

BEGIN_MESSAGE_MAP(CDrawApp, CWinApp)
	ON_COMMAND(ID_DRAW_LINE, &CDrawApp::OnDrawLine)
	ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, &CDrawApp::OnUpdateDrawLine)
	ON_COMMAND(ID_DRAW_STROKE, &CDrawApp::OnDrawStroke)
	ON_UPDATE_COMMAND_UI(ID_DRAW_STROKE, &CDrawApp::OnUpdateDrawStroke)
	ON_COMMAND(ID_DRAW_RECTANGLE, &CDrawApp::OnDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, &CDrawApp::OnUpdateDrawRectangle)
	ON_COMMAND(ID_DRAW_ELLIPSE, &CDrawApp::OnDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, &CDrawApp::OnUpdateDrawEllipse)
END_MESSAGE_MAP()


// CDrawApp ����

CDrawApp::CDrawApp()
{
	m_shpFlag = CShape::NONE;
}

// Ψһ��һ�� CDrawApp ����

CDrawApp theApp;

// CDrawApp ��ʼ��

BOOL CDrawApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int CDrawApp::ExitInstance()
{
	ReleaseShape();

	return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////

CShape::shape CDrawApp::GetCurrentShapeFlag()
{
	return m_shpFlag;
}

void CDrawApp::SetCurrentShapeFlag(CShape::shape shpFlag)
{
	m_shpFlag = shpFlag;
}

CShape* CDrawApp::GetCurrentShape(CWnd* pWnd)
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

CShape* CDrawApp::NewShape(CWnd* pWnd)
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

void CDrawApp::ReleaseShape()
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

	CDrawApp* pApp = (CDrawApp*)AfxGetApp();
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

		CDrawApp* pApp = (CDrawApp*)AfxGetApp();
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

		CDrawApp* pApp = (CDrawApp*)AfxGetApp();
		pApp->GetCurrentShape(pView)->UpdatePoint(point);

		pWnd->Invalidate(FALSE);

		ReleaseCapture();

		return 1;
	}

	return 0;
}

void CDrawApp::ChangeMouseListenState()
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

void CDrawApp::OnDrawLine()
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

void CDrawApp::OnUpdateDrawLine(CCmdUI *pCmdUI)
{
	PICheckUI(pCmdUI, GetCurrentShapeFlag() == CShape::LINE);
}

void CDrawApp::OnDrawStroke()
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


void CDrawApp::OnUpdateDrawStroke(CCmdUI *pCmdUI)
{
	PICheckUI(pCmdUI, GetCurrentShapeFlag() == CShape::STROKE);
}

void CDrawApp::OnDrawRectangle()
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

void CDrawApp::OnUpdateDrawRectangle(CCmdUI *pCmdUI)
{
	PICheckUI(pCmdUI, GetCurrentShapeFlag() == CShape::RECTANGLE);
}

void CDrawApp::OnDrawEllipse()
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

void CDrawApp::OnUpdateDrawEllipse(CCmdUI *pCmdUI)
{
	PICheckUI(pCmdUI, GetCurrentShapeFlag() == CShape::ELLIPSE);
}
