// Curve.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Curve.h"
#include "CurveDlg.h"

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

// CCurveApp

BEGIN_MESSAGE_MAP(CCurveApp, CWinApp)
	ON_COMMAND(ID_CURVE, &CCurveApp::OnImageCurve)
END_MESSAGE_MAP()


// CCurveApp ����

CCurveApp::CCurveApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCurveApp ����

CCurveApp theApp;


// CCurveApp ��ʼ��

BOOL CCurveApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void CCurveApp::OnImageCurve()
{
	CCurveDlg dlg;
	dlg.DoModal();
}