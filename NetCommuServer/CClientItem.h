#pragma once

class CNetCommuServerDlg;
class CClientItem
{
public:
	CString m_strIp;
	SOCKET m_ClientSocket;
	HANDLE m_hThread;
	CNetCommuServerDlg* m_pMainWnd;
	CClientItem() {
		m_ClientSocket = INVALID_SOCKET;
		m_hThread = NULL;
		m_pMainWnd = NULL;
	}
};

