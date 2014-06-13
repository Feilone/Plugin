// LevelWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "Level.h"
#include "LevelWnd.h"
#include <cmath>

// CLevelWnd

IMPLEMENT_DYNAMIC(CLevelWnd, CWnd)

	CLevelWnd::CLevelWnd()
{
	m_CtrlFont.CreatePointFont(90, _T("����"));
	m_nHistogramIndex = 0;
	m_HistogramRect = CRect(40, 60, 320, 240);
	m_nBarIndex = 1;
	m_nChannelIndex = 3;
	m_nSliderXLeftIndex = 7;
	m_nSliderXRightIndex = 8;
	m_nSliderXInflectionIndex = 9;
	m_nSliderYTopIndex = 10;
	m_nSliderYBottomIndex = 11;
	memset(m_arrGrayValues, 0, 1024*sizeof(int));
	InitGrayMap();
	m_Items.push_back(CItemInfo(LEVEL_HISTOGRAM, LEFT_CHANGE, m_HistogramRect));//ֱ��ͼ0
	m_Items.push_back(CItemInfo(LEVEL_BAR, BOTTOM_CHANGE, CRect(40, 300, 320, 310)));//�Ҷ���1
	m_Items.push_back(CItemInfo(LEVEL_COMBOBOX, LEFT_STATIC,CRect(85, 8, 145, 40), IDC_CMB_CHANNEL, IDS_RGB, 4));//ͨ��ѡ��2
	InitMapRect(m_Items.at(m_nHistogramIndex).m_Rect);
	m_Items.push_back(CItemInfo(LEVEL_BUTTON, LEFT_STATIC, CRect(165,7,215,29), IDC_BTN_RESET));//����3
	m_Items.push_back(CItemInfo(LEVEL_TEXT, LEFT_STATIC, CRect(30, 12, 80, 27), IDC_STT_CHANNEL));//ͨ����4
	m_Items.push_back(CItemInfo(LEVEL_TEXT, LEFT_STATIC, CRect(30, 40, 120, 55), IDC_STT_INPUT));//���룺5
	m_Items.push_back(CItemInfo(LEVEL_TEXT, BOTTOM_STATIC, CRect(30, 275,120,290), IDC_STT_OUTPUT));//�����6
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(34, 240, 48, 250), IDC_SLDTRN_XLEFT,0,0,COL_BLACK, m_nHistogramIndex, FALSE, 0, m_nSliderXRightIndex, TRUE, 3/*m_nSliderXInflectionIndex, TRUE, 3*/));//x������7
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(314, 240, 328, 250), IDC_SLDTRN_XRIGHT,0,0,COL_WHITE, m_nSliderXLeftIndex, TRUE, 3, m_nHistogramIndex, FALSE, 0));//x������8
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(174, 240, 188, 250), IDC_SLDTRN_XINFLECTION,0,0,COL_GRAY, m_nSliderXLeftIndex, TRUE, 3, m_nSliderXRightIndex, TRUE, 2));//x������9
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(34, 310, 48, 320), IDC_SLDTRN_YTOP,0,0,COL_BLACK, m_nBarIndex, FALSE, 0, m_nBarIndex, FALSE, 0));//y������10
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(314, 310, 328, 320), IDC_SLDTRN_YBOTTOM,0,0,COL_WHITE, m_nBarIndex, FALSE, 0, m_nBarIndex, FALSE, 0));//y������11
	m_Items.push_back(CItemInfo(LEVEL_EDIT_NUM, BOTTOM_STATIC, CRect(35, 255, 60, 270), IDC_EDT_XLEFT));//x������12
	m_Items.push_back(CItemInfo(LEVEL_EDIT_NUM, BOTTOM_RIGHT, CRect(300, 255, 325, 270), IDC_EDT_XRIGHT));//x������13
	m_Items.push_back(CItemInfo(LEVEL_EDIT_CTRL, BOTTOM_MEDIAN, CRect(168, 255, 193, 270), IDC_EDT_PROPORTION));//x������14
	m_Items.push_back(CItemInfo(LEVEL_EDIT_NUM, BOTTOM_STATIC, CRect(35, 325, 60, 340), IDC_EDT_YTOP));//y������15
	m_Items.push_back(CItemInfo(LEVEL_EDIT_NUM, BOTTOM_RIGHT, CRect(300, 325, 325, 340), IDC_EDT_YBOTTOM));//y������16
}

CLevelWnd::~CLevelWnd()
{
	for(UINT nIndex=0; nIndex<m_Items.size();nIndex++)
	{
		if (m_Items.at(nIndex).m_pItem)
		{
			delete m_Items.at(nIndex).m_pItem;
		}
	}
}

BEGIN_MESSAGE_MAP(CLevelWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_CMB_CHANNEL, &CLevelWnd::OnSelchangeComboChannel)
	ON_BN_CLICKED(IDC_BTN_RESET, &CLevelWnd::OnButtonReset)
	ON_EN_CHANGE(IDC_EDT_XLEFT, &CLevelWnd::OnEditXLeft)
	ON_EN_CHANGE(IDC_EDT_XRIGHT, &CLevelWnd::OnEditXRight)
	ON_EN_CHANGE(IDC_EDT_PROPORTION, &CLevelWnd::OnEditProportion)
	ON_EN_CHANGE(IDC_EDT_YTOP, &CLevelWnd::OnEditYTop)
	ON_EN_CHANGE(IDC_EDT_YBOTTOM, &CLevelWnd::OnEditYBottom)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CLevelWnd ��Ϣ�������

/************************************************************************/
/* ��Ϣ��Ӧ�ͺ���                                                                                 */
/************************************************************************/
//ͨ��ѡ��Combobox
void CLevelWnd::OnSelchangeComboChannel()
{
	switch(((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL))->GetCurSel())
	{
	case 0://RGB
		if (m_nChannelIndex!=3)
		{
			OnButtonReset();
		}
		m_nChannelIndex = 3;
		break;
	case 1://R
		if (m_nChannelIndex == 3)
		{
			m_ftArrProportion[0] = m_ftArrProportion[1] = m_ftArrProportion[2] = m_ftArrProportion[3];
			m_strArrProportion[0] = m_strArrProportion[1] = m_strArrProportion[2] = m_strArrProportion[3];
			m_arrLevelMapRect[0] = m_arrLevelMapRect[1] = m_arrLevelMapRect[2] = m_arrLevelMapRect[3];
			for (int nIndex = 0; nIndex<3; ++nIndex)
			{
				m_nChannelIndex = nIndex;
				XLeftGrayToCoordinate();
				XRightGrayToCoordinate();
				ProportionToXInflection();
				YTopGrayToCoordinate();
				YBottomGrayToCoordinate();
				XLeftChangeCoordinate();
				XRightChangeCoordinate();
				XInflectionChangeCoordinate();
				YTopChangeCoordinate();
				YBottomChangeCoordinate();
			}
		}
		m_nChannelIndex = 2;
		break;
	case 2://G
		if (m_nChannelIndex == 3)
		{
			m_ftArrProportion[0] = m_ftArrProportion[1] = m_ftArrProportion[2] = m_ftArrProportion[3];
			m_strArrProportion[0] = m_strArrProportion[1] = m_strArrProportion[2] = m_strArrProportion[3];
			m_arrLevelMapRect[0] = m_arrLevelMapRect[1] = m_arrLevelMapRect[2] = m_arrLevelMapRect[3];
			for (int nIndex = 0; nIndex<3; ++nIndex)
			{
				m_nChannelIndex = nIndex;
				XLeftGrayToCoordinate();
				XRightGrayToCoordinate();
				ProportionToXInflection();
				YTopGrayToCoordinate();
				YBottomGrayToCoordinate();
				XLeftChangeCoordinate();
				XRightChangeCoordinate();
				XInflectionChangeCoordinate();
				YTopChangeCoordinate();
				YBottomChangeCoordinate();
			}
		}
		m_nChannelIndex = 1;
		break;
	case 3://B
		if (m_nChannelIndex == 3)
		{
			m_ftArrProportion[0] = m_ftArrProportion[1] = m_ftArrProportion[2] = m_ftArrProportion[3];
			m_strArrProportion[0] = m_strArrProportion[1] = m_strArrProportion[2] = m_strArrProportion[3];
			m_arrLevelMapRect[0] = m_arrLevelMapRect[1] = m_arrLevelMapRect[2] = m_arrLevelMapRect[3];
			for (int nIndex = 0; nIndex<3; ++nIndex)
			{
				m_nChannelIndex = nIndex;
				XLeftGrayToCoordinate();
				XRightGrayToCoordinate();
				ProportionToXInflection();
				YTopGrayToCoordinate();
				YBottomGrayToCoordinate();
				XLeftChangeCoordinate();
				XRightChangeCoordinate();
				XInflectionChangeCoordinate();
				YTopChangeCoordinate();
				YBottomChangeCoordinate();
			}
		}
		m_nChannelIndex = 0;
		break;
	default:
		break;
	}
	UpdateData(FALSE);
// 	CString str;
// 	str.Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
// 	SetDlgItemText(IDC_EDT_PROPORTION,str);
	XLeftChangeCoordinate();
	XRightChangeCoordinate();
	XInflectionChangeCoordinate();
	YTopChangeCoordinate();
	YBottomChangeCoordinate();
	Invalidate();
}
//����Button
void CLevelWnd::OnButtonReset()
{
	if (ImageCopy(m_CopyImage, *m_pImage))
	{
		PIGetActiveView()->Invalidate();
		InitGrayMap();
		InitMapRect(m_Items.at(m_nHistogramIndex).m_Rect);
		UpdateData(FALSE);
// 		CString str;
// 		str.Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
// 		SetDlgItemText(IDC_EDT_PROPORTION,str);
		XLeftChangeCoordinate();
		XRightChangeCoordinate();
		XInflectionChangeCoordinate();
		YTopChangeCoordinate();
		YBottomChangeCoordinate();
	}
}
//XLeft�ı��༭
void CLevelWnd::OnEditXLeft()
{
	UpdateData(TRUE);
	if (m_arrLevelMapRect[m_nChannelIndex].left<m_ftMinMatrix[m_nChannelIndex])
	{
		m_arrLevelMapRect[m_nChannelIndex].left = LONG(m_ftMinMatrix[m_nChannelIndex]);
		UpdateData(FALSE);
	}
	else if (m_arrLevelMapRect[m_nChannelIndex].left>m_arrLevelMapRect[m_nChannelIndex].right-2)
	{
		m_arrLevelMapRect[m_nChannelIndex].left = m_arrLevelMapRect[m_nChannelIndex].right-2;
		UpdateData(FALSE);
	}
	XLeftGrayToCoordinate();
	XLeftChangeCoordinate();
	ProportionToXInflection();
	XInflectionChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}
//XRight�ı��༭
void CLevelWnd::OnEditXRight()
{
	UpdateData(TRUE);
	if (m_arrLevelMapRect[m_nChannelIndex].right>m_ftMaxMatrix[m_nChannelIndex])
	{
		m_arrLevelMapRect[m_nChannelIndex].right = LONG(m_ftMaxMatrix[m_nChannelIndex]);
		UpdateData(FALSE);
	}
	else if (m_arrLevelMapRect[m_nChannelIndex].right<m_arrLevelMapRect[m_nChannelIndex].left+2)
	{
		m_arrLevelMapRect[m_nChannelIndex].right = m_arrLevelMapRect[m_nChannelIndex].left+2;
		UpdateData(FALSE);
	}
	XRightGrayToCoordinate();
	XRightChangeCoordinate();
	ProportionToXInflection();
	XInflectionChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}
//Proportion�ı��༭
void CLevelWnd::OnEditProportion()
{
	UpdateData(TRUE);
	if (CheckNum(m_strArrProportion[m_nChannelIndex]))
	{
		m_ftArrProportion[m_nChannelIndex] = (float)_tstof(m_strArrProportion[m_nChannelIndex]);
		if (m_ftArrProportion[m_nChannelIndex]<0.01f)
		{
			AfxMessageBox(_T("������0.01��99.99֮�����"));
			m_ftArrProportion[m_nChannelIndex] = 0.01f;
			m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
			UpdateData(FALSE);
		}
		else if (m_ftArrProportion[m_nChannelIndex]>99.99f)
		{
			AfxMessageBox(_T("������0.01��99.99֮�����"));
			m_ftArrProportion[m_nChannelIndex] = 99.99f;
			m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
			UpdateData(FALSE);
		}
	}
	else
	{
		AfxMessageBox(_T("������0.01��99.99֮�����"));
		m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
		UpdateData(FALSE);
	}
// 	CString str;
// 	GetDlgItem(IDC_EDT_PROPORTION)->GetWindowText(str);
// 	if (CheckNum(str))
// 	{
// 		m_ftArrProportion[m_nChannelIndex] = (float)_tstof(str);
// 		if (m_ftArrProportion[m_nChannelIndex]<0.01f)
// 		{
// 			AfxMessageBox(_T("������0.01��99.99֮�����"));
// 			m_ftArrProportion[m_nChannelIndex] = 0.01f;
// 			str.Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
// 			SetDlgItemText(IDC_EDT_PROPORTION,str);
// 			//UpdateData(FALSE);
// 		}
// 		else if (m_ftArrProportion[m_nChannelIndex]>99.99f)
// 		{
// 			AfxMessageBox(_T("������0.01��99.99֮�����"));
// 			m_ftArrProportion[m_nChannelIndex] = 99.99f;
// 			str.Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
// 			SetDlgItemText(IDC_EDT_PROPORTION,str);
// 		}
// 	}
// 	else
// 	{
// 		AfxMessageBox(_T("������0.01��99.99֮�����"));
// 		str.Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
// 		SetDlgItemText(IDC_EDT_PROPORTION,str);
// 	}
	ProportionToXInflection();
	XInflectionChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}
//YTop�ı��༭
void CLevelWnd::OnEditYTop()
{
	UpdateData(TRUE);
	if (m_arrLevelMapRect[m_nChannelIndex].top<0)
	{
		m_arrLevelMapRect[m_nChannelIndex].top = 0;
		UpdateData(FALSE);
	}
	else if (m_arrLevelMapRect[m_nChannelIndex].top>(m_arrLevelMapRect[m_nChannelIndex].bottom-2))
	{
		m_arrLevelMapRect[m_nChannelIndex].top = m_arrLevelMapRect[m_nChannelIndex].bottom-2;
		UpdateData(FALSE);
	}
	YTopGrayToCoordinate();
	YTopChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}
//YBottom�ı��༭
void CLevelWnd::OnEditYBottom()
{
	UpdateData(TRUE);
	if (m_arrLevelMapRect[m_nChannelIndex].bottom>255)
	{
		m_arrLevelMapRect[m_nChannelIndex].bottom = 255;
		UpdateData(FALSE);
	}
	else if (m_arrLevelMapRect[m_nChannelIndex].bottom<(m_arrLevelMapRect[m_nChannelIndex].top+2))
	{
		m_arrLevelMapRect[m_nChannelIndex].bottom = m_arrLevelMapRect[m_nChannelIndex].top+2;
		UpdateData(FALSE);
	}
	YBottomGrayToCoordinate();
	YBottomChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}
//��ʼ��
int CLevelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetClientRect(&m_rect);
	NewItems();
	InitGrayValues();
	UpdateData(FALSE);
// 	CString str;
// 	str.Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
// 	SetDlgItemText(IDC_EDT_PROPORTION,str);
	return 0;
}
//�����ػ�
void CLevelWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect); // �õ���ť���ڵ���Ч��������

	CDC memDC;//�����ڴ�DC
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(&rect, GetSysColor(COLOR_3DFACE));
	DrawHistogram(memDC);
	DrawBar(memDC);
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	bitmap.DeleteObject();
	memDC.DeleteDC();
}
//��������
BOOL CLevelWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//	return CWnd::OnEraseBkgnd(pDC);
	return FALSE;
}
//���ݽ���
void CLevelWnd::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �ڴ����ר�ô����/����û���
	CWnd::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_XLEFT, m_arrLevelMapRect[m_nChannelIndex].left);
	//DDV_MinMaxUInt(pDX, m_arrLevelMapRect[m_nChannelIndex].left, UINT(m_ftMinMatrix[m_nChannelIndex]), m_arrLevelMapRect[m_nChannelIndex].right-1);
	DDX_Text(pDX, IDC_EDT_XRIGHT, m_arrLevelMapRect[m_nChannelIndex].right);
	//DDV_MinMaxUInt(pDX, m_arrLevelMapRect[m_nChannelIndex].right, m_arrLevelMapRect[m_nChannelIndex].left+1, UINT(m_ftMaxMatrix[m_nChannelIndex]));
	//DDX_Text(pDX, IDC_EDT_PROPORTION, m_arrProportion[m_nChannelIndex]);
	//DDX_Text(pDX, IDC_EDT_PROPORTION, m_strArrProportion[m_nChannelIndex]);
	//DDX_CBString(pDX, IDC_EDT_PROPORTION, m_strArrProportion[m_nChannelIndex]);
	//DDX_Control(pDX, IDC_EDT_PROPORTION, m_arrProportion);
	//DDV_MinMaxFloat(pDX, m_arrProportion[m_nChannelIndex] , 0.01f, 9.99f);
	DDX_Text(pDX, IDC_EDT_PROPORTION, m_strArrProportion[m_nChannelIndex]);
	DDX_Text(pDX, IDC_EDT_YTOP, m_arrLevelMapRect[m_nChannelIndex].top);
	DDV_MinMaxUInt(pDX, m_arrLevelMapRect[m_nChannelIndex].top, 0, 255);
	DDX_Text(pDX, IDC_EDT_YBOTTOM, m_arrLevelMapRect[m_nChannelIndex].bottom);
	DDV_MinMaxUInt(pDX, m_arrLevelMapRect[m_nChannelIndex].bottom , 0, 255);
}
//��ӦDlg��WM_SIZE
void CLevelWnd::ReSize(const UINT& nType, const CRect& newRect)
{
	if(nType != SIZE_MINIMIZED)
	{
		MoveWindow(newRect);
		for(UINT nIndex=0;nIndex<m_Items.size(); ++nIndex)
		{
			if (SetItemPosition(newRect, nIndex))
			{
				switch (nIndex-m_nSliderXLeftIndex)
				{
				case 0:
					XLeftGrayToCoordinate();
					XLeftChangeCoordinate();
				case 1:
					XRightGrayToCoordinate();
					XRightChangeCoordinate();
				case 2:
					ProportionToXInflection();
					XInflectionChangeCoordinate();
				case 3:
					YTopGrayToCoordinate();
					YTopChangeCoordinate();
				case 4:
					YBottomGrayToCoordinate();
					YBottomChangeCoordinate();
				default:
					m_Items.at(nIndex).m_pItem->MoveWindow(m_Items.at(nIndex).m_Rect);
					break;
				}
			}
		}
		int nChannelIndex =m_nChannelIndex;
		for (m_nChannelIndex=0;m_nChannelIndex<4;++m_nChannelIndex)
		{
			XLeftGrayToCoordinate();
			XLeftChangeCoordinate();
			XRightGrayToCoordinate();
			XRightChangeCoordinate();
			ProportionToXInflection();
			XInflectionChangeCoordinate();
			YTopGrayToCoordinate();
			YTopChangeCoordinate();
			YBottomGrayToCoordinate();
			YBottomChangeCoordinate();
		}
		m_nChannelIndex = nChannelIndex;
		m_rect = newRect;
		m_HistogramRect = m_Items.at(m_nHistogramIndex).m_Rect;
	}
}
//�ضϻس���Ϣ
BOOL CLevelWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	// 	if(pMsg-> message==WM_KEYDOWN && pMsg-> wParam==VK_ESCAPE) 
	// 	{ 
	// 		return TRUE; 
	// 	} 

	if(pMsg-> message==WM_KEYDOWN && pMsg-> wParam==VK_RETURN) 
	{ 
		return TRUE; 
	} 
	else 
	{ 
		return CWnd::PreTranslateMessage(pMsg); 
	} 
	//return CWnd::PreTranslateMessage(pMsg);
}

/************************************************************************/
/*���ݴ����ͺ���                                                                                  */
/************************************************************************/
//��ȡͼƬ
CImage* CLevelWnd::GetImage()
{
	CDocument* pDoc = PIGetActiveDocument();
	if (pDoc == NULL)
		return NULL;
	if (!pDoc->IsKindOf(RUNTIME_CLASS(CPIDocument)))
		return NULL;

	return((CPIDocument*)pDoc)->GetImage();
}
//��ʼ���Ҷ�ֱ��ͼ
void CLevelWnd::InitGrayValues()
{
	m_pImage = GetImage();
	ImageCopy(*m_pImage, m_CopyImage);
	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();
	BYTE* pImageData = (BYTE*)m_pImage->GetBits();
	int nPitch = m_pImage->GetPitch();
	int nBitCount = m_pImage->GetBPP();
	nBitCount = nBitCount/8;
	// 	m_ftMinMatrix[0]=m_ftMaxMatrix[0]=*(pImageData);
	// 	m_ftMinMatrix[1]=m_ftMaxMatrix[1]=*(pImageData+1);
	// 	m_ftMinMatrix[2]=m_ftMaxMatrix[2]=*(pImageData+2);
	// 	m_ftMinMatrix[3]=m_ftMaxMatrix[3]=(*(pImageData))*0.3f+(*(pImageData+1))*0.59f+(*(pImageData+2))*0.11f;
	// 	for (int j=0; j<nHeight; ++j)
	// 	{
	// 		for (int i=0; i<nWidth; ++i)
	// 		{
	// 			int nPixelIndex = j * nPitch + i * nBitCount;
	// 			BYTE* pPixel = pImageData +nPixelIndex;
	// 			for (int k=0; k<3; ++k)
	// 			{
	// 				if (*(pPixel+k)>m_ftMaxMatrix[k])
	// 				{
	// 					m_ftMaxMatrix[k] = *(pPixel+k);
	// 				}
	// 				if (*(pPixel+k)<m_ftMinMatrix[k])
	// 				{
	// 					m_ftMinMatrix[k] = *(pPixel+k);
	// 				}
	// 			}
	// 			if ((*(pPixel))*0.3+(*(pPixel+1))*0.59+(*(pPixel+2))*0.11>m_ftMaxMatrix[3])
	// 			{
	// 				m_ftMaxMatrix[3] = (*(pPixel))*0.3f+(*(pPixel+1))*0.59f+(*(pPixel+2))*0.11f;
	// 			}
	// 			if ((*(pPixel))*0.3+(*(pPixel+1))*0.59+(*(pPixel+2))*0.11<m_ftMinMatrix[3])
	// 			{
	// 				m_ftMinMatrix[3] = (*(pPixel))*0.3f+(*(pPixel+1))*0.59f+(*(pPixel+2))*0.11f;
	// 			}
	// 		}
	// 	}
	for (int t=0;t<4;++t)
	{
		m_arrLevelMapRect[t].left = LONG(m_ftMinMatrix[t]);
		m_arrLevelMapRect[t].right = LONG(m_ftMaxMatrix[t]);
		if (m_ftMaxMatrix[t]!=m_ftMinMatrix[t])
		{
			m_ftTranslateProportion[t] = 256.0f/(m_ftMaxMatrix[t]-m_ftMinMatrix[t]+1);
		}
		else
		{
			m_ftTranslateProportion[t] = NULL;
		}
	}

	for (int j=0; j<nHeight; ++j)
	{
		for (int i=0; i<nWidth; ++i)
		{
			int nPixelIndex = j * nPitch + i * nBitCount;
			BYTE* pPixel = pImageData +nPixelIndex;
			for (int nIndex = 0; nIndex<4; ++nIndex)
			{
				if (nIndex!=3)
				{
					if (m_ftTranslateProportion[nIndex]!=NULL)
					{
						++m_arrGrayValues[nIndex][int(((*(pPixel+nIndex))-m_ftMinMatrix[nIndex])*m_ftTranslateProportion[nIndex])];
					}
					else
					{
						++m_arrGrayValues[nIndex][int(m_ftMinMatrix[nIndex])];
					}
				}
				else
				{
					if (m_ftTranslateProportion[nIndex]!=NULL)
					{
						++m_arrGrayValues[nIndex][int((((*(pPixel)+0.5)*0.3+(*(pPixel+1)+0.5)*0.59+(*(pPixel+2)+0.5)*0.11)-m_ftMinMatrix[nIndex])*m_ftTranslateProportion[nIndex])];
					}
					else
					{
						++m_arrGrayValues[nIndex][int(m_ftMinMatrix[nIndex])];
					}
				}
			}
		}
	}
	float ftScale = 1.0f/(nWidth*nHeight);
	float ftLog = 1/log(1.03f);
	for (int k=0; k<256; ++k)
	{
		m_arrGrayValues[0][k] =log(1+m_arrGrayValues[0][k]*ftScale)*ftLog;
		m_arrGrayValues[1][k] = log(1+m_arrGrayValues[1][k]*ftScale)*ftLog;
		m_arrGrayValues[2][k] = log(1+m_arrGrayValues[2][k]*ftScale)*ftLog;
		m_arrGrayValues[3][k] = log(1+m_arrGrayValues[3][k]*ftScale)*ftLog;
	}
}
//��ʼ���Ҷ�ӳ���
void CLevelWnd::InitGrayMap()
{
	for (int nIndex = 0; nIndex < 256; ++nIndex)
	{
		m_arrLevelMap[0][nIndex] = nIndex;
		m_arrLevelMap[1][nIndex] = nIndex;
		m_arrLevelMap[2][nIndex] = nIndex;
		m_arrLevelMap[3][nIndex] = nIndex;
	}
}
//��ʼ������&�Ҷ�ӳ�䷶Χ
void CLevelWnd::InitMapRect(const CRect& rect)
{
	for(int nIndex = 0; nIndex<4; ++nIndex)
	{
		m_arrCoordinateRact[nIndex] =CRect(rect.left, rect.left, rect.right, rect.right) ;
		m_arrXInflection[nIndex] =int(m_arrCoordinateRact[nIndex].left+0.5*m_arrCoordinateRact[nIndex].Width());
		m_ftArrProportion[nIndex] = 1.00f;
		m_strArrProportion[nIndex].Format(_T("%.2f"), m_ftArrProportion[nIndex]);
		m_ftMinMatrix[nIndex] = 0;
		m_ftMaxMatrix[nIndex]  = 255;
		m_arrLevelMapRect[nIndex] = CRect(0,0,255,255);
		m_ftTranslateProportion[nIndex]  = 1;
	}
}
//����ֱ��ͼ
void CLevelWnd::DrawHistogram(CDC& memDC)
{
	CRect drawRect = &m_Items.at(m_nHistogramIndex).m_Rect;
	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	CBrush Brush(GetSysColor(COLOR_3DFACE));
	memDC.SelectObject(pen);
	memDC.SelectObject(Brush);
	memDC.Rectangle(drawRect);
	float ftStepLength = drawRect.Width()/256.0f;
	float ftLeft = 1.0f*drawRect.left;
	int nTop = int(drawRect.bottom - m_arrGrayValues[m_nChannelIndex][0]*drawRect.Height());
	nTop = nTop > drawRect.top ? nTop : drawRect.top;
	memDC.FillSolidRect(CRect(int(ftLeft), nTop, int(ftLeft+ftStepLength), drawRect.bottom), DrawColor());
	for (int i = 1; i < 255; ++i, ftLeft+=ftStepLength)
	{
// 		if (m_arrGrayValues[m_nChannelIndex][i]==0&&m_arrGrayValues[m_nChannelIndex][i-1]!=0&&m_arrGrayValues[m_nChannelIndex][i+1]!=0)
// 		{
// 			float iGrayValue = m_arrGrayValues[m_nChannelIndex][i-1]+m_arrGrayValues[m_nChannelIndex][i+1];
// 			m_arrGrayValues[m_nChannelIndex][i-1]=0.3f*iGrayValue;
// 			nTop = int(drawRect.bottom - m_arrGrayValues[m_nChannelIndex][i-1]*drawRect.Height());
// 			nTop = nTop > drawRect.top ? nTop : drawRect.top;
// 			memDC.FillSolidRect(CRect(int(ftLeft-ftStepLength), drawRect.top, int(ftLeft), drawRect.bottom), GetSysColor(COLOR_3DFACE));
// 			memDC.FillSolidRect(CRect(int(ftLeft-ftStepLength), int(nTop), int(ftLeft), drawRect.bottom), DrawColor());
// 			m_arrGrayValues[m_nChannelIndex][i]=0.4f*iGrayValue;
// 			nTop = int(drawRect.bottom - m_arrGrayValues[m_nChannelIndex][i]*drawRect.Height());
// 			nTop = nTop > drawRect.top ? nTop : drawRect.top;
// 			memDC.FillSolidRect(CRect(int(ftLeft), int(nTop), int(ftLeft+ftStepLength), drawRect.bottom), DrawColor());
// 			m_arrGrayValues[m_nChannelIndex][i+1]=0.3f*iGrayValue;
// 			++i;
// 			ftLeft+=ftStepLength;
// 		}
		nTop = int(drawRect.bottom - m_arrGrayValues[m_nChannelIndex][i]*drawRect.Height()+0.5);
		nTop = nTop > drawRect.top ? nTop : drawRect.top;
		memDC.FillSolidRect(CRect(int(ftLeft), int(nTop), int(ftLeft+ftStepLength), drawRect.bottom), DrawColor());
	}
	nTop = int(drawRect.bottom - m_arrGrayValues[m_nChannelIndex][255]*drawRect.Height()+0.5);
	nTop = nTop > drawRect.top ? nTop : drawRect.top;
	memDC.FillSolidRect(CRect(int(ftLeft), nTop, int(ftLeft+ftStepLength), drawRect.bottom), DrawColor());
}
//���ƻҶ���
void CLevelWnd::DrawBar(CDC& MemDC)
{
	CRect barRect = m_Items.at(m_nBarIndex).m_Rect;
	float stepLength =barRect.Width()/256.0f;
	float left = 1.0f*barRect.left;
	for (int color = 0; color<256;color++,left+=stepLength)
	{
		CBrush Brush(RGB(color, color, color));
		MemDC.FillRect(CRect(int(left), barRect.top, int(left+stepLength), barRect.bottom), &Brush);
	}
}
//�趨ֱ��ͼ��ɫ
COLORREF CLevelWnd::DrawColor()
{
	switch (m_nChannelIndex)
	{
	case 0:
		return RGB(0, 0, 255);
	case 1:
		return RGB(0, 255, 0);
	case 2:
		return RGB(255, 0, 0);
	case 3:
		return RGB(0, 0, 0);
	default:
		return RGB(255, 255, 255);
	}
}
//�����ؼ�
void CLevelWnd::NewItems()
{
	CRect rect;
	GetClientRect(&rect);
	CString str;
	for(UINT nIndex = 0; nIndex < m_Items.size(); ++nIndex)
	{
		const ItemType& eType = m_Items.at(nIndex).m_eType;
		const ItemPosition& ePosition = m_Items.at(nIndex).m_ePosition;
		CRect& itemRect = m_Items.at(nIndex).m_Rect;
		const UINT& nID = m_Items.at(nIndex).m_nID;
		const UINT& nSourceID = m_Items.at(nIndex).m_nSourceID;
		const int& nSourveNum = m_Items.at(nIndex).m_nSourceNum;
		const ItemExStyle& eExStyle = m_Items.at(nIndex).m_eExStyle;
		CWnd*& pItem = m_Items.at(nIndex).m_pItem;
		switch (eType)
		{
		case LEVEL_BUTTON:
			pItem = new CButton();
			str.LoadString(nID);
			((CButton*)pItem)->Create(str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, itemRect, this, nID);
			pItem->SetFont(&m_CtrlFont);
			break;
		case LEVEL_COMBOBOX:
			pItem = new CComboBox();
			((CComboBox*)pItem)->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, itemRect, this, nID);
			for (int nCount = 0;nCount<nSourveNum; ++nCount)
			{
				str.LoadString((nSourceID+nCount));
				((CComboBox*)pItem)->InsertString(nCount, str);  
			}
			((CComboBox*)pItem)->SetCurSel(0);
			pItem->SetFont(&m_CtrlFont);
			break;
		case LEVEL_TEXT:
			pItem = new CStatic();
			str.LoadString(nID);
			((CStatic*)pItem)->Create(str, WS_CHILD | WS_VISIBLE|SS_LEFT, itemRect, this, nID);
			pItem->SetFont(&m_CtrlFont);
			break;
		case LEVEL_HISTOGRAM:
			break;
		case LEVEL_BAR:
			break;
		case LEVEL_EDIT_NUM:
			pItem = new CEdit();
			((CEdit*)pItem)->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP|ES_LEFT|ES_AUTOHSCROLL |ES_NUMBER, itemRect, this, nID);
			pItem->SetFont(&m_CtrlFont);
			break;
		case LEVEL_EDIT_CTRL:
			pItem = new CEdit();
			((CEdit*)pItem)->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP|ES_AUTOHSCROLL |ES_LEFT, itemRect, this, nID);
			pItem->SetFont(&m_CtrlFont);
			break;
		case LEVEL_SLDWND:
			{
				int& nMinID = m_Items.at(nIndex).m_nMinIndex;
				int& nMaxID = m_Items.at(nIndex).m_nMaxIndex;
				int& nMinDis = m_Items.at(nIndex).m_nMinDis;
				int& nMaxDis = m_Items.at(nIndex).m_nMaxDis;
				BOOL& bMinFlag = m_Items.at(nIndex).m_bMinFlag;
				BOOL& bMaxFlag = m_Items.at(nIndex).m_bMaxFlag;
				CRect* pMin = &m_Items.at(nMinID).m_Rect;
				CRect* pMax = &m_Items.at(nMaxID).m_Rect;
				pItem = new CLevelSlider();
				((CLevelSlider*)pItem)->NewLevelSlider(itemRect, nID,eExStyle, this, (CLevelSlider*)pItem, pMin, bMinFlag, nMinDis, pMax, bMaxFlag,  nMaxDis);
				pItem->SetFont(&m_CtrlFont);
				break;
			}
		default:
			break;
		}
	}
	// 	CString str2;
	// 	str2.Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
	// 	SetDlgItemText(IDC_EDT_PROPORTION,str2);
	UpdateData(FALSE);
}
//�Ҷ�ӳ��
void CLevelWnd::GrayMapping()
{
	const CRect& mappingSpace = m_arrLevelMapRect[m_nChannelIndex];
	const float& ftProportion = m_ftArrProportion[m_nChannelIndex];
	int nInflectionGrayY = int(mappingSpace.top+0.5*mappingSpace.Height());
	int nInflectionGrayX ; int nLeftGrayX; int nRightGrayX;
	if (m_ftTranslateProportion[m_nChannelIndex]!=NULL)
	{
		nInflectionGrayX = int((mappingSpace.Width()*ftProportion/(1.0f+ftProportion)+mappingSpace.left-m_ftMinMatrix[m_nChannelIndex])*m_ftTranslateProportion[m_nChannelIndex]);
		nLeftGrayX = int((mappingSpace.left-m_ftMinMatrix[m_nChannelIndex])*m_ftTranslateProportion[m_nChannelIndex]);
		nRightGrayX = int((mappingSpace.right-m_ftMinMatrix[m_nChannelIndex])*m_ftTranslateProportion[m_nChannelIndex]);
	}
	else
	{
		if (m_ftMinMatrix[m_nChannelIndex] < 0)
		{
			nInflectionGrayX = nLeftGrayX = nRightGrayX = 0;
		}
		else if (m_ftMinMatrix[m_nChannelIndex] > 255)
		{
			nInflectionGrayX = nLeftGrayX = nRightGrayX = 255;
		}
		else
		{
			nInflectionGrayX = nLeftGrayX = nRightGrayX = int(m_ftMinMatrix[m_nChannelIndex]);
		}
	}
	float ftStepLengthLeft = 0.5f*mappingSpace.Height()/(nInflectionGrayX-nLeftGrayX);
	float ftStepLengthRight = 0.5f*mappingSpace.Height()/(nRightGrayX-nInflectionGrayX);
	for (int nCount = 0; nCount <256; ++nCount)
	{
		if (nCount < nLeftGrayX)
		{
			m_arrLevelMap[m_nChannelIndex][nCount] = mappingSpace.top;
		}
		else if (nCount >= nLeftGrayX && nCount < nInflectionGrayX)
		{
			m_arrLevelMap[m_nChannelIndex][nCount] = int(mappingSpace.top+(nCount-nLeftGrayX)*ftStepLengthLeft);
		}
		else if (nCount >= nInflectionGrayX && nCount < nRightGrayX)
		{
			m_arrLevelMap[m_nChannelIndex][nCount] = int(nInflectionGrayY+(nCount-nInflectionGrayX)*ftStepLengthRight);
		}
		else
		{
			m_arrLevelMap[m_nChannelIndex][nCount] = mappingSpace.bottom;
		}
	}
}
//���ÿؼ�λ��
BOOL CLevelWnd::SetItemPosition(const CRect& newRect, const UINT& nIndex)
{
	const ItemPosition& ePosition = m_Items.at(nIndex).m_ePosition;
	CRect& itemRect = m_Items.at(nIndex).m_Rect;
	switch(ePosition)
	{
	case LEFT_STATIC:
		return TRUE;
	case LEFT_CHANGE:
		{
			int nRightMargin = 160;
			int nBottomMargin = 120;
			int nWidthChange = newRect.Width()-itemRect.left-nRightMargin;
			nWidthChange = nWidthChange<=0?1:nWidthChange;
			int nHeightChange = newRect.Height()-itemRect.top-nBottomMargin;
			nHeightChange = nHeightChange<=0?1:nHeightChange;
			// 			itemRect.right = itemRect.Width()<=0?(itemRect.left+1):(itemRect.left+itemRect.Width());
			// 			itemRect.bottom = itemRect.Height()<=0?(itemRect.top+1):(itemRect.top+itemRect.Height());
			// 			itemRect.right = itemRect.left+nWidthChange;
			// 			itemRect.bottom = itemRect.top+nHeightChange;
// 			int nJudgement = nWidthChange*itemRect.Height()/itemRect.Width();
// 			if (nJudgement<=nHeightChange)
// 			{
 				itemRect.right = LONG(nWidthChange)+itemRect.left;
// 				itemRect.bottom = LONG(nJudgement)+itemRect.top;
// 			}
// 			else
// 			{
 // 				itemRect.right = LONG(nHeightChange*itemRect.Width()/itemRect.Height())+itemRect.left;
				itemRect.bottom = LONG(nHeightChange)+itemRect.top;
// 			}
			return FALSE;
		}
	case RIGHT_STATIC:
		{
			int nItemWidth = itemRect.Width();
			int nRightMargin = m_rect.right-itemRect.right;
			itemRect.left = newRect.right-nRightMargin - nItemWidth;
			itemRect.right = itemRect.left+nItemWidth;
			return TRUE;
		}
	case BOTTOM_CHANGE:
		{
			int nItemHeight = itemRect.Height();
			itemRect.top = itemRect.top-m_HistogramRect.bottom+m_Items.at(m_nHistogramIndex).m_Rect.bottom;
			itemRect.bottom = itemRect.top+nItemHeight;
			itemRect.right = itemRect.left+m_Items.at(m_nHistogramIndex).m_Rect.Width();
			return FALSE;
		}
	case BOTTOM_STATIC:
		{
			int nItemHeight = itemRect.Height();
			itemRect.top = itemRect.top-m_HistogramRect.bottom+m_Items.at(m_nHistogramIndex).m_Rect.bottom;
			itemRect.bottom = itemRect.top+nItemHeight;
			return TRUE;
		}
	case BOTTOM_MEDIAN:
		{
			int nItemHeight = itemRect.Height();
			int nItemWidth = itemRect.Width();
			itemRect.top = itemRect.top-m_HistogramRect.bottom+m_Items.at(m_nHistogramIndex).m_Rect.bottom;
			itemRect.bottom = itemRect.top+nItemHeight;
			itemRect.left = LONG(0.5*(m_Items.at(m_nHistogramIndex).m_Rect.Width()-itemRect.Width())+m_Items.at(m_nHistogramIndex).m_Rect.left);
			itemRect.right = itemRect.left +nItemWidth;
			return TRUE;
		}
	case BOTTOM_RIGHT:
		{
			int nItemHeight = itemRect.Height();
			int nItemWidth = itemRect.Width();
			itemRect.top = itemRect.top-m_HistogramRect.bottom+m_Items.at(m_nHistogramIndex).m_Rect.bottom;
			itemRect.bottom = itemRect.top+nItemHeight;
			itemRect.left = itemRect.left+m_Items.at(m_nHistogramIndex).m_Rect.Width()-m_HistogramRect.Width();
			itemRect.right = itemRect.left +nItemWidth;
			return TRUE;
		}
	default:
		return FALSE;
	}
}
//����ͼƬ
BOOL CLevelWnd::UpdateImage()
{
	if (m_pImage==NULL)
	{
		return FALSE;
	}
	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();
	BYTE* pImageData = (BYTE*)m_pImage->GetBits();
	BYTE* pCopyImageData = (BYTE*)m_CopyImage.GetBits();
	int nPitch = m_pImage->GetPitch();
	int nBitCount = m_pImage->GetBPP() / 8;

	for (int j=0; j<nHeight; ++j)
	{
		for (int i=0; i<nWidth; ++i)
		{
			int nPixelIndex = j * nPitch + i * nBitCount;
			BYTE* pPixel = pImageData +nPixelIndex;
			BYTE* pCopyPixel = pCopyImageData +nPixelIndex;
			if (m_nChannelIndex == 3)
			{
				*(pPixel) = m_arrLevelMap[m_nChannelIndex][int(((*(pCopyPixel))-m_ftMinMatrix[m_nChannelIndex])*m_ftTranslateProportion[m_nChannelIndex])];
				*(pPixel+1) = m_arrLevelMap[m_nChannelIndex][int(((*(pCopyPixel+1))-m_ftMinMatrix[m_nChannelIndex])*m_ftTranslateProportion[m_nChannelIndex])];
				*(pPixel+2) = m_arrLevelMap[m_nChannelIndex][int(((*(pCopyPixel+2))-m_ftMinMatrix[m_nChannelIndex])*m_ftTranslateProportion[m_nChannelIndex])];			
			}
			else
			{
				*(pPixel+m_nChannelIndex) = m_arrLevelMap[m_nChannelIndex][int(((*(pCopyPixel+m_nChannelIndex))-m_ftMinMatrix[m_nChannelIndex])*m_ftTranslateProportion[m_nChannelIndex])];
			}
		}
	}
	PIGetActiveView()->Invalidate();
	return TRUE;
}
//����ͼƬ
BOOL CLevelWnd::ImageCopy(const CImage& m_Image, CImage& m_CopyImage)
{
	if(m_Image.IsNull())
		return FALSE;
	//Դͼ�����
	int nWidth = m_Image.GetWidth();
	int nHeight = m_Image.GetHeight();
	BYTE* pImageData = (BYTE*)m_Image.GetBits();
	int nPitch = m_Image.GetPitch();
	int nBitCount = m_Image.GetBPP();
	//����ԭ��ͼ��
	if( !m_CopyImage.IsNull())
	{
		m_CopyImage.Destroy();
	}
	//������ͼ��
	if(nBitCount == 32)   //֧��alphaͨ��
	{
		m_CopyImage.Create(nWidth, nHeight, nBitCount, 1);
	}
	else
	{
		m_CopyImage.Create(nWidth, nHeight, nBitCount, 0);
	}
	//���ص�ɫ��
	if(nBitCount<=8&&m_Image.IsIndexed())//��Ҫ��ɫ��
	{
		RGBQUAD pal[256];
		int nColors=m_Image.GetMaxColorTableEntries();
		if(nColors>0)
		{     
			m_Image.GetColorTable(0,nColors,pal);
			m_CopyImage.SetColorTable(0,nColors,pal);//���Ƶ�ɫ�����
		}   
	} 
	//Ŀ��ͼ�����
	BYTE *pCopyImageData=(BYTE*)m_CopyImage.GetBits();
	int nCopyPitch=m_CopyImage.GetPitch();
	//����ͼ������
	for(int i=0 ; i<nHeight;i++)
	{
		memcpy( pCopyImageData+i*nCopyPitch, pImageData+i*nPitch, abs(nPitch) );
	} 
	return TRUE;
}
//�ж�����
BOOL CLevelWnd::CheckNum(CString str)
{
	bool bNumFlag = true;
	bool bFloatFlag = false;
	for(int i=0;i<str.GetLength();i++)
	{
		if(((int)str.GetAt(i)>=48&&(int)str.GetAt(i)<=57))continue;	
		else if(str.GetAt(i)=='.')
		{
			if(bFloatFlag)
			{
				bNumFlag = false;
				break; 
			}
			else
			{
				bFloatFlag = true;
				continue;
			}

		}
		else
		{
			bNumFlag = false;
			break;
		}
	}
	return bNumFlag;
}

//����ת�Ҷ�
//Wnd��������ת�Ҷ�
void CLevelWnd::CoordinateToGray(UINT nID, LONG center)
{
	switch (nID)
	{
	case IDC_SLDTRN_XLEFT:
		m_arrCoordinateRact[m_nChannelIndex].left = center;
		XLeftCoordinateToGray();
		ProportionToXInflection();
		XInflectionChangeCoordinate();
		break;
	case IDC_SLDTRN_XRIGHT:
		m_arrCoordinateRact[m_nChannelIndex].right = center;
		XRightCoordinateToGray();
		ProportionToXInflection();
		XInflectionChangeCoordinate();
		break;
	case IDC_SLDTRN_YTOP:
		m_arrCoordinateRact[m_nChannelIndex].top = center;
		YTopCoordinateToGray();
		break;
	case IDC_SLDTRN_YBOTTOM:
		m_arrCoordinateRact[m_nChannelIndex].bottom = center;
		YBottomCoordinateToGray();
		break;
	case IDC_SLDTRN_XINFLECTION:
		m_arrXInflection[m_nChannelIndex] = center;
		XInflectionToProportion();
		break;
	default:
		break;
	}
// 	CString str;
// 	str.Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
// 	SetDlgItemText(IDC_EDT_PROPORTION,str);
	UpdateData(FALSE);
}
//xLeft����תxLeft�Ҷ�ֵ
void CLevelWnd::XLeftCoordinateToGray()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nXLeftCoordinate = m_arrCoordinateRact[m_nChannelIndex].left;
	float ftScale = 255.0f/wholeRect.Width();
	m_arrLevelMapRect[m_nChannelIndex].left = int((nXLeftCoordinate-wholeRect.left)*ftScale);
	UpdateData(FALSE);
}
//xRight����תxRight�Ҷ�ֵ
void CLevelWnd::XRightCoordinateToGray()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nXRightCoordinate = m_arrCoordinateRact[m_nChannelIndex].right;
	float ftScale = 255.0f/wholeRect.Width();
	m_arrLevelMapRect[m_nChannelIndex].right = int((nXRightCoordinate-wholeRect.left)*ftScale);
	UpdateData(FALSE);
}
//�յ�x����ת�յ����
void CLevelWnd::XInflectionToProportion()
{
	const CRect& coordinateRact = m_arrCoordinateRact[m_nChannelIndex];
	int& nInflection_x = m_arrXInflection[m_nChannelIndex];
	//nInflection_x = nInflection_x<=coordinateRact.left?(coordinateRact.left+2):nInflection_x;
	//nInflection_x = nInflection_x>=coordinateRact.right?(coordinateRact.right-2):nInflection_x;
	m_ftArrProportion[m_nChannelIndex] =1.0f* (nInflection_x-coordinateRact.left)/(coordinateRact.right-nInflection_x);
	m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_ftArrProportion[m_nChannelIndex]);
}
//yTop����תyTop�Ҷ�ֵ
void CLevelWnd:: YTopCoordinateToGray()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nYTopCoordinate = m_arrCoordinateRact[m_nChannelIndex].top;
	float ftScale = 255.0f/wholeRect.Width();
	m_arrLevelMapRect[m_nChannelIndex].top = int((nYTopCoordinate-wholeRect.left)*ftScale);
}
//yBottom����תyBottom�Ҷ�ֵ
void CLevelWnd::YBottomCoordinateToGray()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nYBottomCoordinate = m_arrCoordinateRact[m_nChannelIndex].bottom;
	float ftScale = 255.0f/wholeRect.Width();
	m_arrLevelMapRect[m_nChannelIndex].bottom = int((nYBottomCoordinate-wholeRect.left)*ftScale);
}

//�Ҷ�ת����
//xLeft�Ҷ�ֵתxLeft����
void CLevelWnd::XLeftGrayToCoordinate()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nXLeftGray = m_arrLevelMapRect[m_nChannelIndex].left;
	if (m_ftTranslateProportion[m_nChannelIndex]!=NULL)
	{
		float ftScale = wholeRect.Width()/(m_ftMaxMatrix[m_nChannelIndex]-m_ftMinMatrix[m_nChannelIndex]+1);
		m_arrCoordinateRact[m_nChannelIndex].left =int((nXLeftGray-m_ftMinMatrix[m_nChannelIndex])*ftScale+wholeRect.left+0.5*ftScale);
	}
	else
	{
		m_arrCoordinateRact[m_nChannelIndex].left = wholeRect.left;
	}
}
//xRight�Ҷ�ֵתxRight����
void CLevelWnd::XRightGrayToCoordinate()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nXRightGray = m_arrLevelMapRect[m_nChannelIndex].right;
	if (m_ftTranslateProportion[m_nChannelIndex]!=NULL)
	{
		float ftScale = wholeRect.Width()/(m_ftMaxMatrix[m_nChannelIndex]-m_ftMinMatrix[m_nChannelIndex]+1);
		m_arrCoordinateRact[m_nChannelIndex].right =int((nXRightGray-m_ftMinMatrix[m_nChannelIndex])*ftScale+wholeRect.left+0.5*ftScale);
	}
	else
	{
		m_arrCoordinateRact[m_nChannelIndex].right = wholeRect.right;
	}
}
//�յ����ת�յ�x����
void CLevelWnd::ProportionToXInflection()
{
	const CRect& coordinateRact = m_arrCoordinateRact[m_nChannelIndex];
	float& ftProportion = m_ftArrProportion[m_nChannelIndex];
	//ftProportion = ftProportion<=0?0.01f:ftProportion;
	//ftProportion = ftProportion>=256? 256.0f:ftProportion;
	m_arrXInflection[m_nChannelIndex] = 	int(coordinateRact.Width()*ftProportion/(1.0f+ftProportion)+coordinateRact.left);
}
//yTop�Ҷ�ֵתyTop����
void CLevelWnd::YTopGrayToCoordinate()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nYTopGray = m_arrLevelMapRect[m_nChannelIndex].top;
	float ftScale = wholeRect.Width()/256.0f;
	m_arrCoordinateRact[m_nChannelIndex].top =int(nYTopGray*ftScale+wholeRect.left+0.5*ftScale);
}
//yBottom�Ҷ�ֵתyBottom����
void CLevelWnd::YBottomGrayToCoordinate()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nYBottomGray = m_arrLevelMapRect[m_nChannelIndex].bottom;
	float ftScale = wholeRect.Width()/256.0f;
	m_arrCoordinateRact[m_nChannelIndex].bottom =int(nYBottomGray*ftScale+wholeRect.left+0.5*ftScale);
}

//��������
//����xLeft����
void CLevelWnd::XLeftChangeCoordinate()
{
	int nNewCenter = m_arrCoordinateRact[m_nChannelIndex].left;
	CRect& XLeftRect = m_Items.at(m_nSliderXLeftIndex).m_Rect;
	int nXLeftWidth = XLeftRect.Width();
	XLeftRect.left = LONG(nNewCenter-0.5*nXLeftWidth);
	XLeftRect.right = LONG(nNewCenter+0.5*nXLeftWidth);
	CWnd*& pXLeft = m_Items.at(m_nSliderXLeftIndex).m_pItem;
	pXLeft->MoveWindow(XLeftRect);
}
//����xRight����
void CLevelWnd::XRightChangeCoordinate()
{
	int nNewCenter = m_arrCoordinateRact[m_nChannelIndex].right;
	CRect& XRightRect = m_Items.at(m_nSliderXRightIndex).m_Rect;
	int nXRightWidth = XRightRect.Width();
	XRightRect.left = LONG(nNewCenter-0.5*nXRightWidth);
	XRightRect.right = LONG(nNewCenter+0.5*nXRightWidth);
	CWnd*& pXLeft = m_Items.at(m_nSliderXRightIndex).m_pItem;
	pXLeft->MoveWindow(XRightRect);
}
//���¹յ�����
void CLevelWnd::XInflectionChangeCoordinate()
{
	int nNewCenter = m_arrXInflection[m_nChannelIndex];
	CRect& XInflectionRect = m_Items.at(m_nSliderXInflectionIndex).m_Rect;
	int nXInflectionWidth = XInflectionRect.Width();
	XInflectionRect.left = LONG(nNewCenter-0.5*nXInflectionWidth);
	XInflectionRect.right = LONG(nNewCenter+0.5*nXInflectionWidth);
	CWnd*& pXInflection = m_Items.at(m_nSliderXInflectionIndex).m_pItem;
	pXInflection->MoveWindow(XInflectionRect);
}
//����yTop����
void CLevelWnd::YTopChangeCoordinate()
{
	int nNewCenter = m_arrCoordinateRact[m_nChannelIndex].top;
	CRect& YTopRect = m_Items.at(m_nSliderYTopIndex).m_Rect;
	int nYTopWidth = YTopRect.Width();
	YTopRect.left = LONG(nNewCenter-0.5*nYTopWidth);
	YTopRect.right = LONG(nNewCenter+0.5*nYTopWidth);
	CWnd*& pXLeft = m_Items.at(m_nSliderYTopIndex).m_pItem;
	pXLeft->MoveWindow(YTopRect);
}
//����yBottom����
void CLevelWnd::YBottomChangeCoordinate()
{
	int nNewCenter = m_arrCoordinateRact[m_nChannelIndex].bottom;
	CRect& YBottomRect = m_Items.at(m_nSliderYBottomIndex).m_Rect;
	int nYBottomWidth = YBottomRect.Width();
	YBottomRect.left = LONG(nNewCenter-0.5*nYBottomWidth);
	YBottomRect.right = LONG(nNewCenter+0.5*nYBottomWidth);
	CWnd*& pXLeft = m_Items.at(m_nSliderYBottomIndex).m_pItem;
	pXLeft->MoveWindow(YBottomRect);
}