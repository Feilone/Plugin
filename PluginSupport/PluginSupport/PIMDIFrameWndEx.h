#pragma once

#include "PluginDefine.h"

// CPIMDIFrameWndEx ���

class PLUGIN_EXT_CLASS CPIMDIFrameWndEx : public CMDIFrameWndEx
{
	DECLARE_DYNCREATE(CPIMDIFrameWndEx)
protected:
	CPIMDIFrameWndEx();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPIMDIFrameWndEx();

public:
	virtual void ProgressInit(BOOL bDlgOrBar, LPCTSTR lpszText, CDialog** pDialog);
	virtual BOOL ProgressPercent(int nPercent);
	virtual void ProgressDone();

	BOOL GetProgressType()						{ return m_nProgressType; }

	void SetProgressThreadRunning(BOOL bFlag)	{ m_bProgressThreadRunning = bFlag; }

	BOOL GetProgressThreadRunning()				{ return m_bProgressThreadRunning; }

private:
	BOOL m_nProgressType;

	// Progress thread running flag
	BOOL m_bProgressThreadRunning;

protected:
	afx_msg LRESULT OnUIEnableEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUICheckEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPluginCommand(UINT nID);
	afx_msg void OnUpdatePluginCommand(CCmdUI* pCmdUI);
	afx_msg LRESULT OnGetActiveDocument(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetActiveView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressInit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressPercent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressDone(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


