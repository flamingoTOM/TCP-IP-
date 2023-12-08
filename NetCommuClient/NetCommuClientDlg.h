
// NetCommuClientDlg.h: 头文件
//

#pragma once
DWORD WINAPI ConnectNetworkThread(LPVOID lpParameter);

// CNetCommuClientDlg 对话框
class CNetCommuClientDlg : public CDialogEx
{
// 构造
public:
	CNetCommuClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETCOMMUCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditRevBox;
	afx_msg void OnBnClickedButtonconnect();
	BOOL ConnectSocket(CNetCommuClientDlg* pClient);
	SOCKET m_ClientSock;
	BOOL isServerOn;
	void SetRevBoxText(CString strMsg);
	bool socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);
	afx_msg void OnBnClickedButtonsend();
	afx_msg void OnBnClickedButtonstop();
	afx_msg void OnBnClickedButtonquit();
};
CString GetTime();