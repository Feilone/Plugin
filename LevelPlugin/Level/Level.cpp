// Level.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Level.h"
#include "LevelDlg.h"

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

// CLevelApp

BEGIN_MESSAGE_MAP(CLevelApp, CWinApp)
	ON_COMMAND(ID_IMAGE_LEVEL, &CLevelApp::OnImageLevel)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_LEVEL, &CLevelApp::OnUpdateImageLevel)
END_MESSAGE_MAP()


// CLevelApp ����

CLevelApp::CLevelApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLevelApp ����

CLevelApp theApp;


// CLevelApp ��ʼ��

BOOL CLevelApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void CLevelApp::OnImageLevel()
{
	CDocument* pDoc = PIGetActiveDocument();
	if (pDoc == NULL)
		return;
	if (!pDoc->IsKindOf(RUNTIME_CLASS(CPIDocument)))
		return;

	CLevelDlg dlg;
	dlg.DoModal();
}

void CLevelApp::OnUpdateImageLevel(CCmdUI *pCmdUI)
{
	CDocument* pDoc = PIGetActiveDocument();
	PIEnableUI(pCmdUI, pDoc != NULL);
}
