#pragma once

class CLog
{
public:
	CLog();
	virtual ~CLog();

public:
	//-��־�ļ�-
	//���û��ָ������Ϊexe����·���µ�log.log�ļ�
	static CString GetLogFile();

	static short SetLogFile(LPCTSTR strPath);

	static short ViewLogFile();

	// ǰ׺
	// ������������ͬһ���ļ������־������Ϊÿ����������һ��ǰ׺
	// ǰ׺����������ʱ��֮ǰ
	static short SetPrefix(LPCTSTR strPrefix);

	//-��־��Ϣ-
	//-��ȡ��־�ַ���,��������-
	static CString sOutV(LPCTSTR strType, LPCTSTR strFormat = NULL, va_list valist = NULL);

	static CString sOut0(LPCTSTR strType, LPCTSTR strFormat = NULL,...);
	
	static CString sOut ( LPCTSTR strFormat = NULL,...);

	//����־��Ϣ������ļ�
	static short OutV(LPCTSTR strType, LPCTSTR strFormat = NULL, va_list valist = NULL);

	static short Out0(LPCTSTR strType, LPCTSTR strFormat = NULL, ...);

	static short Out(LPCTSTR strFormat = NULL, ...);

protected:
	static CString m_strLogFile;

	static CString m_strLogPrefix;

	static HANDLE m_hWriteEvent;
};