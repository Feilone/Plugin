// ImageView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image.h"
#include "ImageView.h"

#include "ImageDocument.h"

// CImageView

IMPLEMENT_DYNCREATE(CImageView, CScrollView)

CImageView::CImageView()
{

}

CImageView::~CImageView()
{

}

BEGIN_MESSAGE_MAP(CImageView, CScrollView)
END_MESSAGE_MAP()

// CImageView ��ͼ

void CImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageView::OnDraw(CDC* pDC)
{
	CImageDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
	CImage* pImage = pDoc->GetImage();
	if(pImage)
	{
		pImage->Draw(pDC->GetSafeHdc(), 0, 0);
	}
}

// CImageView ���

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif

CImageDocument* CImageView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDocument)));
	return (CImageDocument*)m_pDocument;
}
#endif //_DEBUG

// CImageView ��Ϣ�������
