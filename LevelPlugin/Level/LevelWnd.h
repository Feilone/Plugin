#pragma once
#include "ItemInfo.h"
#include "LevelSlider.h"
#include <vector>
using std::vector;

// CLevelWnd

class CLevelWnd : public CWnd
{
	DECLARE_DYNAMIC(CLevelWnd)

public:
	//���캯��
	CLevelWnd();
	//��������
	virtual ~CLevelWnd();
protected:
	DECLARE_MESSAGE_MAP()
public:
	/************************************************************************/
	/* ��Ϣ��Ӧ�ͺ���                                                                                 */
	/************************************************************************/
	//ͨ��ѡ��Combobox
	afx_msg void OnSelchangeComboChannel();
	//����Button
	afx_msg void OnButtonReset();
	//XLeft�ı��༭
	afx_msg void OnEditXLeft();
	//XRight�ı��༭
	afx_msg void OnEditXRight();
	//Proportion�ı��༭
	afx_msg void OnEditProportion();
	//YTop�ı��༭
	afx_msg void OnEditYTop();
	//YBottom�ı��༭
	afx_msg void OnEditYBottom();
	//��ʼ��
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����ػ�
	afx_msg void OnPaint();
	//��������
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//���ݽ���
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ӦDlg��WM_SIZE
	void ReSize(const UINT& nType, const CRect& Rect);
	//�ضϻس���Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	/************************************************************************/
	/*���ݴ����ͺ���                                                                                  */
	/************************************************************************/
	//��ȡͼƬ
	CImage* GetImage();
	//��ʼ���Ҷ�ֱ��ͼ
	void InitGrayValues();
	//��ʼ���Ҷ�ӳ���
	void InitGrayMap();
	//��ʼ������&�Ҷ�ӳ�䷶Χ
	void InitMapRect(const CRect& rect);
	//����ֱ��ͼ
	void DrawHistogram(CDC& memDC);
	//���ƻҶ���
	void DrawBar(CDC& memDC);
	//�趨ֱ��ͼ��ɫ
	COLORREF DrawColor();
	//�����ؼ�
	void NewItems();
	//�Ҷ�ӳ��
	void GrayMapping();
	//���ÿؼ�λ��
	BOOL SetItemPosition(const CRect& newRect, const UINT& nIndex);
	//����ͼƬ
	BOOL UpdateImage();
	//����ͼƬ
	BOOL ImageCopy(const CImage& m_Image, CImage& m_CopyImage);
	//�ж�����
	BOOL CheckNum(CString );

	//����ת�Ҷ�
	//Wnd��������ת�Ҷ�
	void CoordinateToGray(UINT nID, LONG center);
	//xLeft����תxLeft�Ҷ�ֵ
	void XLeftCoordinateToGray();
	//xRight����תxRight�Ҷ�ֵ
	void XRightCoordinateToGray();
	//�յ�x����ת�յ����
	void XInflectionToProportion();
	//yTop����תyTop�Ҷ�ֵ
	void YTopCoordinateToGray();
	//yBottom����תyBottom�Ҷ�ֵ
	void YBottomCoordinateToGray();

	//�Ҷ�ת����
	//xLeft�Ҷ�ֵתxLeft����
	void XLeftGrayToCoordinate();
	//xRight�Ҷ�ֵתxRight����
	void XRightGrayToCoordinate();
	//�յ����ת�յ�x����
	void ProportionToXInflection();
	//yTop�Ҷ�ֵתyTop����
	void YTopGrayToCoordinate();
	//yBottom�Ҷ�ֵתyBottom����
	void YBottomGrayToCoordinate();

	//��������
	//����xLeft����
	void XLeftChangeCoordinate();
	//����xRight����
	void XRightChangeCoordinate();
	//���¹յ�����
	void XInflectionChangeCoordinate();
	//����yTop����
	void YTopChangeCoordinate();
	//����yBottom����
	void YBottomChangeCoordinate();
	
	public:
		/************************************************************************/
		/* �洢������                                                                                        */
		/************************************************************************/
	//�������ֵ
	float m_ftMaxMatrix[4];
	//������Сֵ
	float m_ftMinMatrix[4];
	//ת������(ft)
	float m_ftTranslateProportion[4];
	//Wnd����
	CRect m_rect;
	//ֱ��ͼ����
	CRect m_HistogramRect;
	//�Ҷ�ֱ��ͼ���Ʋ���
	float m_arrGrayValues[4][256];
	//�Ҷ�ӳ������
	int m_arrLevelMap[4][256];
	//ͼ��ָ��
	CImage* m_pImage;
	//ԭͼ����
	CImage m_CopyImage;
	//�ؼ�����
	CFont m_CtrlFont;
	//Ԫ�ؼ���
	vector<CItemInfo> m_Items;
	//����ӳ����������
	CRect m_arrCoordinateRact[4];
	//ɫ��ӳ������
	CRect m_arrLevelMapRect[4];
	//����X�յ�����
	int m_arrXInflection[4];
	//ɫ��ӳ�����
	float m_ftArrProportion[4];
	//ת������(str)
	CString m_strArrProportion[4];
	/************************************************************************/
	/* ����������                                                                                        */
	/************************************************************************/
	//ͨ��ѡ���־
	int m_nChannelIndex;
	//ֱ��ͼ������ַ
	int m_nHistogramIndex;
	//�Ҷ���������ַ
	int m_nBarIndex;
	//SliderXLeft������ַ
	int m_nSliderXLeftIndex;
	//SliderXRight������ַ
	int m_nSliderXRightIndex;
	//SliderXInflection������ַ
	int m_nSliderXInflectionIndex;
	//SliderYTop������ַ
	int m_nSliderYTopIndex;
	//SliderYBottom������ַ
	int m_nSliderYBottomIndex;

};


