
// NetCommuServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "NetCommuServer.h"
#include "NetCommuServerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetCommuServerDlg 对话框



CNetCommuServerDlg::CNetCommuServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NETCOMMUSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetCommuServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITGETBOX, m_EditRevBox);
}

BEGIN_MESSAGE_MAP(CNetCommuServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONSTART, &CNetCommuServerDlg::OnBnClickedButtonstart)
	ON_BN_CLICKED(IDC_BUTTONSEND, &CNetCommuServerDlg::OnBnClickedButtonsend)
	ON_BN_CLICKED(IDC_BUTTONCLOSE, &CNetCommuServerDlg::OnBnClickedButtonclose)
	ON_BN_CLICKED(IDC_BUTTONHIDE, &CNetCommuServerDlg::OnBnClickedButtonhide)
	ON_BN_CLICKED(IDC_BUTTONQUIT, &CNetCommuServerDlg::OnBnClickedButtonquit)
END_MESSAGE_MAP()


// CNetCommuServerDlg 消息处理程序

BOOL CNetCommuServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNetCommuServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNetCommuServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNetCommuServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNetCommuServerDlg::OnBnClickedButtonstart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ServicePort = GetDlgItemInt(IDC_EDITPORT); //获取端口号的值
	if (m_ServicePort <= 1024 || m_ServicePort > 65535)
	{
		AfxMessageBox(_T("请输入合适的端口"));
		SetDlgItemText(IDC_EDITPORT,_T(""));
		GetDlgItem(IDC_EDITPORT)->SetFocus();
		return;
	}
	StartServer();
	m_isServerOpen = TRUE;


}


bool CNetCommuServerDlg::StartServer()
{
	// TODO: 在此处添加实现代码.
	m_hListenThread = CreateThread(NULL, 0, ListenThreadFunc, this, 0, 0);
	if (m_hListenThread == NULL)
		return FALSE;
	else
		return TRUE;
}

DWORD WINAPI  ListenThreadFunc(LPVOID Lparam)
{
	CNetCommuServerDlg* pServer = (CNetCommuServerDlg*)Lparam;
	if (INVALID_SOCKET == (pServer->m_SockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		AfxMessageBox(_T("SOCKET 创建失败！"));
		return 0;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(pServer->m_ServicePort);
	if (0 != bind(pServer->m_SockListen, (sockaddr*)&service, sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("绑定端口失败！"));
		return 0;
	}
	if (0 != listen(pServer->m_SockListen, 5))
	{
		AfxMessageBox(_T("监听端口失败！"));
		return 0;
	}
	AfxMessageBox(_T("服务器启动成功，开始监听端口"));

	//*****************************************//
	while (TRUE)
	{

		if (socket_Select(pServer->m_SockListen, 100, TRUE))
		{
			sockaddr_in clientAddr;
			int iLen = sizeof(sockaddr_in);
			SOCKET accSock = accept(pServer->m_SockListen, (struct sockaddr*)&clientAddr, &iLen);
			if (accSock == INVALID_SOCKET)
			{
				continue;
			}
			//将节点加入链中
			CClientItem tItem;
			tItem.m_ClientSocket = accSock;
			char sendBuf[20] = { '\0' };
			inet_ntop(AF_INET, &clientAddr.sin_addr.S_un.S_addr, sendBuf, 100);
			tItem.m_strIp = sendBuf;
			tItem.m_pMainWnd = pServer;
			int idx = pServer->m_ClientArray.Add(tItem); //idx是第x个连接的客户端
			tItem.m_hThread = CreateThread(NULL, 0, ClientThreadProc, //创建一个线程并挂起:CREATE_SUSPENDED
				&(pServer->m_ClientArray.GetAt(idx)), CREATE_SUSPENDED, NULL);
			pServer->m_ClientArray.GetAt(idx).m_hThread = tItem.m_hThread;
			//等把hThread加入了节点，才开始执行线程，如下
			ResumeThread(tItem.m_hThread);
			pServer->SetDlgItemText(IDC_EDITGETBOX, tItem.m_strIp + _T("上线") + "\r\n");
			Sleep(100);
		}

	}
}


#define MAX_BUFF 256
	DWORD WINAPI ClientThreadProc(LPVOID Lparam) { //利用异步IO模型循环读取socket内的信息，并发送给各个用户
		USES_CONVERSION;
		CString strMsg;
		CClientItem ClientItem = *(CClientItem*)Lparam;
		while (TRUE)
		{
			if (socket_Select(ClientItem.m_ClientSocket, 100, TRUE))
			{
				char szRev[MAX_BUFF] = { 0 };
				int iRet = recv(ClientItem.m_ClientSocket, szRev, sizeof(szRev), 0);
				if (iRet > 0)
				{
					strMsg = A2T(szRev); //中文出现乱码，英文正常
	//				strMsg.Format(_T("%s"),szRev); //这么写连英文都不对了
					ClientItem.m_pMainWnd->SetRevBoxText(ClientItem.m_strIp + _T(">>") + strMsg);
					ClientItem.m_pMainWnd->SendClientMsg(strMsg, &ClientItem);
				}
				else {
					strMsg = ClientItem.m_strIp + _T(" 已离开");
					ClientItem.m_pMainWnd->RemoveClientFromArray(ClientItem);
					ClientItem.m_pMainWnd->SetRevBoxText(strMsg);
					break;
				}
			}
		}
		return 0;
	}

BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead) {
	FD_SET fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hSocket, &fdset);
	nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
	tv.tv_sec = 0;
	tv.tv_usec = nTimeOut;
	int iRet = 0;
	if (bRead)
	{
		iRet = select(0, &fdset, NULL, NULL, &tv);
	}
	else
	{
		iRet = select(0, NULL, &fdset, NULL, &tv);
	}
	if (iRet <= 0)
	{
		return FALSE;
	}
	else if (FD_ISSET(hSocket, &fdset))
	{
		return TRUE;
	}
	return FALSE;
}

// 设置文本格式
void CNetCommuServerDlg::SetRevBoxText(CString strMsg)
{
	// TODO: 在此处添加实现代码.
	m_EditRevBox.SetSel(-1, -1);
	m_EditRevBox.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));

}

CString GetTime() {
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d %02d:%02d"), time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute);
	return strTime;
}

// 客户端下线，从链表移除该节点
void CNetCommuServerDlg::RemoveClientFromArray(CClientItem in_item)
{
	// TODO: 在此处添加实现代码.
	for (int idx = 0; idx < m_ClientArray.GetCount(); idx++)
	{
		if (in_item.m_ClientSocket == m_ClientArray[idx].m_ClientSocket &&
			in_item.m_hThread == m_ClientArray[idx].m_hThread &&
			in_item.m_strIp == m_ClientArray[idx].m_strIp &&
			in_item.m_pMainWnd == m_ClientArray[idx].m_pMainWnd)
		{
			m_ClientArray.RemoveAt(idx);
		}
	}
	return;
}


void CNetCommuServerDlg::SendClientMsg(CString strMsg, CClientItem* pWhoseItem)
{
	// TODO: 在此处添加实现代码.
	USES_CONVERSION;
	char szBuf[256] = { 0 };
	strcpy_s(szBuf, 256, T2A(strMsg));
	for (int i = 0; i < m_ClientArray.GetCount(); i++)
	{
		if (!pWhoseItem || !equal(pWhoseItem, &(m_ClientArray.GetAt(i))))
		{
			send(m_ClientArray.GetAt(i).m_ClientSocket, szBuf, 256, 0);
		}
	}
}


BOOL CNetCommuServerDlg::equal(const CClientItem* p1, const CClientItem* p2)
{
	// TODO: 在此处添加实现代码.
	if (p1->m_ClientSocket == p2->m_ClientSocket && p1->m_hThread == p2->m_hThread && p1->m_strIp == p2->m_strIp)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void CNetCommuServerDlg::OnBnClickedButtonsend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;
	GetDlgItemText(IDC_EDITSENDBOX, strMsg);
	SendClientMsg(strMsg, NULL);
	SetRevBoxText(_T("服务器>>") + strMsg);
	SetDlgItemText(IDC_EDITSENDBOX, _T(""));
}


void CNetCommuServerDlg::OnBnClickedButtonclose()
{
	// TODO: 在此添加控件通知处理程序代码
	int AllClient = m_ClientArray.GetCount();
	for (int idx = 0; idx < AllClient; idx++)
	{
		closesocket(m_ClientArray.GetAt(idx).m_ClientSocket);
	}
	m_ClientArray.RemoveAll();
	closesocket(m_SockListen);
	SetRevBoxText(_T("停止监听端口"));
	m_isServerOpen = FALSE;
}


void CNetCommuServerDlg::OnBnClickedButtonhide()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
	Sleep(200);
}


void CNetCommuServerDlg::OnBnClickedButtonquit()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_isServerOpen)
	{
		int AllClient = m_ClientArray.GetCount();
		for (int idx = 0; idx < AllClient; idx++)
		{
			closesocket(m_ClientArray.GetAt(idx).m_ClientSocket);
		}
		m_ClientArray.RemoveAll();
		closesocket(m_SockListen);
		SetRevBoxText(_T("停止监听端口"));
		m_isServerOpen = FALSE;
	}
	SendMessage(WM_CLOSE);
}
