// Process.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Process.h"

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

// CProcessApp

BEGIN_MESSAGE_MAP(CProcessApp, CWinApp)
	ON_COMMAND(ID_IMAGE_INVERSE, &CProcessApp::OnImageInverse)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_INVERSE, &CProcessApp::OnUpdateImageInverse)
END_MESSAGE_MAP()


// CProcessApp ����

CProcessApp::CProcessApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CProcessApp ����

CProcessApp theApp;


// CProcessApp ��ʼ��

BOOL CProcessApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

UINT InverseThread(LPVOID pParam)
{
	LANGID id = PIGetThreadUILanguage();
	SetThreadUILanguage(id);

	CString str;
	str.LoadString(ID_IMAGE_INVERSE);
	PIProgressInit(str);

	CImage* pImage = (CImage*)pParam;

	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	BYTE* pImageData = (BYTE*)pImage->GetBits();
	int nPitch = pImage->GetPitch();
	int nBitCount = pImage->GetBPP() / 8;

	for (int j=0; j<nHeight; j++)
	{
		for (int i=0; i<nWidth; i++)
		{
			int nPixelIndex = j * nPitch + i * nBitCount;
			BYTE* pPixel = pImageData + j * nPitch + i * nBitCount;
			*(pPixel) = 255 - *(pPixel);
			*(pPixel + 1) = 255 - *(pPixel + 1);
			*(pPixel + 2) = 255 - *(pPixel + 2);
		}

		PIProgressPercent(j * 100 / nHeight);
	}

	PIProgressDone();

	// refresh view
	PIGetActiveView()->Invalidate(FALSE);

	return 0;
}

void CProcessApp::OnImageInverse()
{
	CDocument* pDoc = PIGetActiveDocument();
	if (pDoc == NULL)
		return;
	if (!pDoc->IsKindOf(RUNTIME_CLASS(CPIDocument)))
		return;

	CImage* pImage = ((CPIDocument*)pDoc)->GetImage();
	if (pImage == NULL)
		return;

	AfxBeginThread(InverseThread, pImage);
}

void CProcessApp::OnUpdateImageInverse(CCmdUI* pCmdUI)
{
	CDocument* pDoc = PIGetActiveDocument();
	PIEnableUI(pCmdUI, pDoc != NULL);
}