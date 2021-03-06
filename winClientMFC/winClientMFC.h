
// winClientMFC.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#define BUF_SIZE 1024
#define DEFAULT_PORT "27015"
#define FILE_NAME_MAX_SIZE 512
struct FILESEND {
	long id = 0;									//文件包编号
	float size = 0;									//文件大小
	int end = 0;									//文件结束标志
	char name[FILE_NAME_MAX_SIZE];					//文件名
	char content[BUF_SIZE];							//文件包缓冲区
};

// CwinClientMFCApp:
// 有关此类的实现，请参阅 winClientMFC.cpp
//

class CwinClientMFCApp : public CWinApp
{
//自定义全局
public:
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	SOCKET ConnectSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	FILESEND fileS, oldFile;
	//char sendbuf[BUF_SIZE];
	char recvbuf[BUF_SIZE];
	//char fileName[File_Name_Max_Size+1];
	int iResult, iSendResult;
	int recvbuflen = BUF_SIZE;
	time_t oldT, nowT;

	int ConnectServer() {
		//初始化连接socket
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			//printf("Error at socket():%d\n", WSAGetLastError());
			//freeaddrinfo(result);
			//WSACleanup();
			return 1;
		}
		//连接服务器
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			//printf("Unable to connect to server!\n");
			closesocket(ConnectSocket);
			//continue;
			return 2;
		}
		//freeaddrinfo(result);
		//ZeroMemory(sendbuf, sizeof(recvbuf));
		ZeroMemory(recvbuf, sizeof(recvbuf));
		ZeroMemory((char*)&fileS, sizeof(fileS));
		ZeroMemory((char*)&oldFile, sizeof(oldFile));
		return 0;
	}



public:
	CwinClientMFCApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CwinClientMFCApp theApp;
