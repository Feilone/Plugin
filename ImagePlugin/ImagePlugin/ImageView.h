#pragma once

class CImageDocument;

// CImageView ��ͼ

class CImageView : public CScrollView
{
	DECLARE_DYNCREATE(CImageView)

protected:
	CImageView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CImageView();

public:
	CImageDocument* GetDocument() const;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CImageView.cpp �еĵ��԰汾
inline CImageDocument* CImageView::GetDocument() const
{ return reinterpret_cast<CImageDocument*>(m_pDocument); }
#endif
