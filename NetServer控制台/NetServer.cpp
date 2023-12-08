// NetServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <WINSOCK2.H>
#include <iostream>
#pragma  comment(lib,"ws2_32.lib")   //链接ws2_32.lib库
using namespace std;

void main(int argc, char* argv[])
{
	////SOCKET前的一些检查，检查协议库的版本，为了避免别的版本的socket，并且通过  
    //WSAStartup启动对应的版本，WSAStartup的参数一个是版本信息，一个是一些详细的细节，注意高低位  
   //WSAStartup与WSACleanup对应  

	
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 1, 1 );
	
	err = WSAStartup( wVersionRequested, &wsaData );//异步套接字启动
	if ( err != 0 ) {
		return;
	}
	
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
        HIBYTE( wsaData.wVersion ) != 1 ) {
		WSACleanup( );
		return;
	}
	//--------------------------------------------------------------------------------------------------
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);//ipv4 or ipv6? 流or数据报？
	//type为流，protocol为0，则默认TCP套接字；type为数据报，protocol为0，默认为UDP。
	
	SOCKADDR_IN addrSrv;// socket编程中，定义了一个结构体SOCKADDR_IN来存计算机的一些信息
	addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");//IP地址
    //	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//IP地址
	addrSrv.sin_family=AF_INET;//协议族，在socket中只能是AF_INET
	addrSrv.sin_port=htons(6000);//端口号
	
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));// 绑定端口
	
	listen(sockSrv,5);//最多接收的链接数为5
	
	//------------------------------------------------------------------------------------------------
	SOCKADDR_IN addrClient;// 连接上的客户端ip地址
	int len=sizeof(SOCKADDR);
	cout<<"waiting for connected..."<<endl;


	while(1)
	{
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);// 接受客户端连接,获取客户端的ip地址
		char sendBuf[50];
		sprintf(sendBuf,"from server:Welcome %s to here!",inet_ntoa(addrClient.sin_addr));// 组合消息发送出去
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);// 发送消息到客户端
		char recvBuf[50];
		recv(sockConn,recvBuf,50,0);// 接受客户端消息
		printf("%s\n",recvBuf);
		//closesocket(sockConn);//断开连接
	}
	
}

