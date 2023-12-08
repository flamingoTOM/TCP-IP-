
// NetCommuClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "NetCommuClient.h"
#include "NetCommuClientDlg.h"
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


// CNetCommuClientDlg 对话框



CNetCommuClientDlg::CNetCommuClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NETCOMMUCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetCommuClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITREVBOX, m_EditRevBox);
}

BEGIN_MESSAGE_MAP(CNetCommuClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONCONNECT, &CNetCommuClientDlg::OnBnClickedButtonconnect)
	ON_BN_CLICKED(IDC_BUTTONSEND, &CNetCommuClientDlg::OnBnClickedButtonsend)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &CNetCommuClientDlg::OnBnClickedButtonstop)
	ON_BN_CLICKED(IDC_BUTTONQUIT, &CNetCommuClientDlg::OnBnClickedButtonquit)
END_MESSAGE_MAP()


// CNetCommuClientDlg 消息处理程序

BOOL CNetCommuClientDlg::OnInitDialog()
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

void CNetCommuClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNetCommuClientDlg::OnPaint()
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
HCURSOR CNetCommuClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNetCommuClientDlg::OnBnClickedButtonconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(0, 0, ConnectNetworkThread, this, 0, NULL);
}
DWORD WINAPI ConnectNetworkThread(LPVOID lpParameter)
{
	CNetCommuClientDlg* pClient =(CNetCommuClientDlg*)lpParameter;
	if (pClient->ConnectSocket(pClient))
	{

	}
	return 0;
}
#define MAX_BUFF 256
BOOL CNetCommuClientDlg::ConnectSocket(CNetCommuClientDlg* pClient)
{
	// TODO: 在此处添加实现代码.
	m_ClientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (NULL == m_ClientSock)
	{
		MessageBox(_T("创建socket失败"));
		return FALSE;
	}
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	CString strIp;
	DWORD dPort = GetDlgItemInt(IDC_EDITPORT);
	GetDlgItemText(IDC_IPADDRESS, strIp);
	if (strIp == _T("0.0.0.0") || (dPort >= 65535 && dPort < 1024) || dPort == 0)
	{
		MessageBox(_T("请输入正确IP地址或端口"));
		return FALSE;
	}
	sa.sin_port = htons(dPort);
	char szIpAdd[32];
	USES_CONVERSION; //定义后才能使用T2A
	sprintf_s(szIpAdd, 32, "%s", T2A(strIp));
	//sa.sin_addr.S_un.S_addr = inet_addr(szIpAdd);
	inet_pton(AF_INET, szIpAdd, &sa.sin_addr.S_un.S_addr);
	if (SOCKET_ERROR == connect(m_ClientSock, (sockaddr*)&sa, sizeof(sa)))
	{
		MessageBox(_T("连接客户端错误,请检查你填写的IP和端口是否错误"));
		return FALSE;
	}

	pClient->SetRevBoxText(_T("连接服务器成功"));
	isServerOn = TRUE;

	CString strMsg;
	while (TRUE)
	{
		if (socket_Select(m_ClientSock, 100, TRUE))
		{
			char szMsg[MAX_BUFF] = { 0 };
			int iRead = recv(m_ClientSock, szMsg, MAX_BUFF, 0);
			if (iRead > 0)
			{
				strMsg = szMsg;
				pClient->SetRevBoxText(strIp + _T(">>") + strMsg);
			}
			else
			{
				pClient->SetRevBoxText(_T("已断线，请重新连接"));
				isServerOn = FALSE;
				return TRUE;
			}
		}
	}
	return TRUE;
	
}


void CNetCommuClientDlg::SetRevBoxText(CString strMsg)
{
	// TODO: 在此处添加实现代码.
	m_EditRevBox.SetSel(-1, -1);
	m_EditRevBox.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
}
CString GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d %02d:%02d"), time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute);
	return strTime;
}

bool CNetCommuClientDlg::socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead)
{
	// TODO: 在此处添加实现代码.
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


void CNetCommuClientDlg::OnBnClickedButtonsend()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	char szBuf[256] = { 0 };
	CString strGetMsg;
	int iWrite;
	GetDlgItemText(IDC_EDITSENDBOX, strGetMsg);
	strcpy_s(szBuf, T2A(strGetMsg));
	iWrite = send(m_ClientSock, szBuf, 256, 0);
	if (SOCKET_ERROR == iWrite) {
		SetRevBoxText(_T("发送错误"));
	}
	SetRevBoxText(_T("我自己 >>") + strGetMsg);
	SetDlgItemText(IDC_EDITSENDBOX, _T(""));
	return;
}


void CNetCommuClientDlg::OnBnClickedButtonstop()
{
	// TODO: 在此添加控件通知处理程序代码
	closesocket(m_ClientSock);
	isServerOn = FALSE;
}


void CNetCommuClientDlg::OnBnClickedButtonquit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonstop();
	SendMessage(WM_CLOSE);
}
