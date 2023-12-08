
// NetCommuServerDlg.h: 头文件
//

#pragma once
#include "CClientItem.h"
#include "afxwin.h"
//#include<WS2tcpip.h>
#include "Winsock2.h"
DWORD WINAPI  ListenThreadFunc(LPVOID Lparam);
BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);
DWORD WINAPI ClientThreadProc(LPVOID Lparam);
CString GetTime();


// CNetCommuServerDlg 对话框
class CNetCommuServerDlg : public CDialogEx
{
// 构造
public:
	CNetCommuServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETCOMMUSERVER_DIALOG };
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
	afx_msg void OnBnClickedButtonstart();
	UINT m_ServicePort;
	bool StartServer();
	bool m_isServerOpen;
	HANDLE m_hListenThread;
	SOCKET m_SockListen;
	CEdit m_EditRevBox;
	CArray<CClientItem> m_ClientArray;
	// 设置文本格式
	void SetRevBoxText(CString strMsg);
	// 客户端下线，从链表移除该节点
	void RemoveClientFromArray(CClientItem in_item);
	void SendClientMsg(CString strMsg, CClientItem* pWhoseItem);
	BOOL equal(const CClientItem* p1, const CClientItem* p2);
	afx_msg void OnBnClickedButtonsend();
	afx_msg void OnBnClickedButtonclose();
	afx_msg void OnBnClickedButtonhide();
	afx_msg void OnBnClickedButtonquit();
};
