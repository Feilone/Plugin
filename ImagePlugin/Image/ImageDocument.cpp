// ImageDocument.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image.h"
#include "ImageDocument.h"


// CImageDocument

IMPLEMENT_DYNCREATE(CImageDocument, CPIDocument)

CImageDocument::CImageDocument()
	: m_pImage(NULL)
{

}

BOOL CImageDocument::OnNewDocument()
{
	if (!CPIDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CImageDocument::~CImageDocument()
{

}

BEGIN_MESSAGE_MAP(CImageDocument, CPIDocument)
END_MESSAGE_MAP()

// CImageDocument ���

#ifdef _DEBUG
void CImageDocument::AssertValid() const
{
	CPIDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CImageDocument::Dump(CDumpContext& dc) const
{
	CPIDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CImageDocument ���л�

void CImageDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}
#endif

// CImageDocument ����

BOOL CImageDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CPIDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  �ڴ������ר�õĴ�������
	CString strPathName = lpszPathName;
	int nPos = strPathName.ReverseFind('.');
	// ȡ�ú�׺��
	CString strExt = strPathName.Right(strPathName.GetLength() - nPos - 1);
	strExt.MakeLower();

	int nType = FindType(strExt);
	if (nType != IMAGE_FORMAT_UNKNOWN)
	{
		m_pImage = new CImage;
		m_pImage->Load(lpszPathName);
	}

	return TRUE;
}

int CImageDocument::FindType(const CString strExt)
{
	if (strExt.CompareNoCase(_T("bmp")))
	{
		return IMAGE_FORMAT_BMP;
	}
	else if (strExt.CompareNoCase(_T("jpg")))
	{
		return IMAGE_FORMAT_JPG;
	}
	else if (strExt.CompareNoCase(_T("gif")))
	{
		return IMAGE_FORMAT_GIF;
	}
	else if (strExt.CompareNoCase(_T("png")))
	{
		return IMAGE_FORMAT_PNG;
	}
	else if (strExt.CompareNoCase(_T("tif")))
	{
		return IMAGE_FORMAT_PNG;
	}

	return IMAGE_FORMAT_UNKNOWN;
}