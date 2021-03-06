
// winClientMFCDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "winClientMFC.h"
#include "winClientMFCDlg.h"
#include "afxdialogex.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <time.h>
#pragma comment(lib,"Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUF_SIZE 1024
#define DEFAULT_PORT "27015"
#define FILE_NAME_MAX_SIZE 512



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


// CwinClientMFCDlg 对话框



CwinClientMFCDlg::CwinClientMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_WINCLIENTMFC_DIALOG, pParent)
	, serverIp(_T(""))
	, serverPort(_T(""))
	, fileName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwinClientMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, serverIp);
	DDX_Text(pDX, IDC_EDIT2, serverPort);
	DDX_Control(pDX, IDC_LIST1, news);
	DDX_Text(pDX, IDC_EDIT3, fileName);
}

BEGIN_MESSAGE_MAP(CwinClientMFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CwinClientMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CwinClientMFCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CwinClientMFCDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CwinClientMFCDlg 消息处理程序

BOOL CwinClientMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	
	//初始化Socket
	CwinClientMFCApp* App = (CwinClientMFCApp*)AfxGetApp();
	//printf("%d", sizeof(fileS));
	//初始化WSA
	App->iResult = WSAStartup(MAKEWORD(2, 2), &(App->wsaData));
	if (App->iResult) {
		MessageBox(L"WSAStartup failed:" + App->iResult);
		//printf("WSAStartup failed:%d\n", App->iResult);
		//return 1;
		PostMessage(WM_QUIT, 0, 0);
	}
	//协议信息
	ZeroMemory(&(App->hints), sizeof(App->hints));
	//ipv4
	App->hints.ai_family = AF_INET;
	//TCP
	App->hints.ai_socktype = SOCK_STREAM;
	App->hints.ai_protocol = IPPROTO_TCP;
	serverPort = DEFAULT_PORT;
	serverIp = "127.0.0.1";
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CwinClientMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CwinClientMFCDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CwinClientMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CwinClientMFCDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//连接服务器
	UpdateData(TRUE);
	CwinClientMFCApp* App = (CwinClientMFCApp*)AfxGetApp();
	CString err;
	news.AddString(L"serverIp:"+serverIp);
	if (serverIp.GetLength() == 0) {
		news.AddString(L"IP地址不能为空");
		return;
	}
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, serverIp, -1, NULL, NULL, 0, NULL);
	char *ip = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, serverIp, -1, ip, dwNum, 0, NULL);
	if (serverPort.GetLength() == 0) {
		news.AddString(L"端口号不能为空");
		return;
	}
	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, serverPort, -1, NULL, NULL, 0, NULL);
	char *port = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, serverPort, -1, port, dwNum, 0, NULL);
	App->iResult = getaddrinfo(ip, port, &(App->hints), (&App->result));
	if (App->iResult) {
		err.Format(_T("%d"), App->iResult);
		news.AddString(L"getaddrinfo failed:" + err);
		//WSACleanup();
		return;
	}
	App->ptr = App->result;
	InitConnect();
}



void CwinClientMFCDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//下载
	UpdateData(TRUE);
	CwinClientMFCApp* App = (CwinClientMFCApp*)AfxGetApp();
	CString err;
	ZeroMemory((char*)&(App->oldFile), sizeof(App->oldFile));
	WideCharToMultiByte(CP_OEMCP, NULL, fileName, -1, (App->oldFile.name), FILE_NAME_MAX_SIZE, 0, NULL);
	App->iResult = send(App->ConnectSocket, App->oldFile.name, strlen(App->oldFile.name) + 1, 0);
	if (App->iResult == SOCKET_ERROR) {
		//printf("send failed: %d\n", WSAGetLastError());
		err.Format(_T("%d"), WSAGetLastError());
		news.AddString(L"文件名发送失败：" + err);
		closesocket(App->ConnectSocket);
		return;
	}
	ZeroMemory(App->recvbuf, sizeof(App->recvbuf));
	//接受文件名和大小
	App->iResult = recv(App->ConnectSocket, (char*)&(App->fileS), sizeof(App->fileS), 0);
	if (App->iResult > 0) {

		//int pos = 0;
		//char size[BUF_SIZE];
		//pos = strpos(recvbuf, ';');

		strcpy_s(App->oldFile.name, App->fileS.name);
		App->oldFile.size = App->fileS.size;
		//发送文件断点id
		App->iResult = send(App->ConnectSocket, (char*)&(App->oldFile), sizeof(App->oldFile), 0);
		if (App->iResult == SOCKET_ERROR) {
			//printf("send failed: %d\n", WSAGetLastError());
			err.Format(_T("%d"), WSAGetLastError());
			news.AddString(L"send failed:" + err);
			closesocket(App->ConnectSocket);
			return;
		}
		//打开文件
		FILE *fp;
		//重传以追加方式打开
		//if (!oldFile.end&&oldFile.id > 0) {
		//	fopen_s(&fp, oldFile.name, "ab+");
		//}
		//else
		App->oldT = time(NULL);
		fopen_s(&fp, App->oldFile.name, "wb");
		/*if (!oldFile.end&&oldFile.id > 0) {
			fseek(fp, BUF_SIZE*oldFile.id, SEEK_CUR);
		}*/
		RecvFIle(fp);
	}
	else if (App->iResult == 0)
		news.AddString(L"已关闭连接");
	//printf("Connection closed\n");
	else {
		err.Format(_T("%d"), WSAGetLastError());
		news.AddString(L"recv failed:" + err);
	}
		//printf("recv failed: %d\n", WSAGetLastError());
	printf("connect close\n");
	closesocket(App->ConnectSocket);
	news.AddString(L"连接已关闭");
}

//重传文件
void CwinClientMFCDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	InitConnect();
	CwinClientMFCApp* App = (CwinClientMFCApp*)AfxGetApp();
	CString err;
	App->iResult = send(App->ConnectSocket, App->oldFile.name, strlen(App->oldFile.name) + 1, 0);
	if (App->iResult == SOCKET_ERROR) {
		//printf("send failed: %d\n", WSAGetLastError());
		err.Format(_T("%d"), WSAGetLastError());
		news.AddString(L"文件名发送失败：" + err);
		closesocket(App->ConnectSocket);
		return;
	}
	ZeroMemory(App->recvbuf, sizeof(App->recvbuf));
	//接受文件名和大小
	App->iResult = recv(App->ConnectSocket, (char*)&(App->fileS), sizeof(App->fileS), 0);
	if (App->iResult > 0) {

		//int pos = 0;
		//char size[BUF_SIZE];
		//pos = strpos(recvbuf, ';');

		//strcpy_s(App->oldFile.name, App->fileS.name);
		//App->oldFile.size = App->fileS.size;
		//发送文件断点id
		App->iResult = send(App->ConnectSocket, (char*)&(App->oldFile), sizeof(App->oldFile), 0);
		if (App->iResult == SOCKET_ERROR) {
			//printf("send failed: %d\n", WSAGetLastError());
			err.Format(_T("%d"), WSAGetLastError());
			news.AddString(L"send failed:" + err);
			closesocket(App->ConnectSocket);
			return;
		}
		//打开文件
		FILE *fp;
		//重传以追加方式打开
		//if (!oldFile.end&&oldFile.id > 0) {
		fopen_s(&fp, App->oldFile.name, "ab+");
		//}
		//else
		//App->oldT = time(NULL);
		//fopen_s(&fp, App->oldFile.name, "wb");
		//文件流起点重定位
		fseek(fp, BUF_SIZE*(App->oldFile.id), SEEK_CUR);
		/*if (!oldFile.end&&oldFile.id > 0) {
		fseek(fp, BUF_SIZE*oldFile.id, SEEK_CUR);
		}*/
		RecvFIle(fp);
	}
	else if (App->iResult == 0)
		news.AddString(L"已关闭连接");
	//printf("Connection closed\n");
	else {
		err.Format(_T("%d"), WSAGetLastError());
		news.AddString(L"recv failed:" + err);

	}
	//printf("recv failed: %d\n", WSAGetLastError());
	//printf("connect close\n");
	closesocket(App->ConnectSocket);
	news.AddString(L"连接已关闭");
}
