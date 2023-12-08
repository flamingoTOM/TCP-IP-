// NetServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <WINSOCK2.H>
#include <iostream>
#pragma  comment(lib,"ws2_32.lib")   //����ws2_32.lib��
using namespace std;

void main(int argc, char* argv[])
{
	////SOCKETǰ��һЩ��飬���Э���İ汾��Ϊ�˱����İ汾��socket������ͨ��  
    //WSAStartup������Ӧ�İ汾��WSAStartup�Ĳ���һ���ǰ汾��Ϣ��һ����һЩ��ϸ��ϸ�ڣ�ע��ߵ�λ  
   //WSAStartup��WSACleanup��Ӧ  

	
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 1, 1 );
	
	err = WSAStartup( wVersionRequested, &wsaData );//�첽�׽�������
	if ( err != 0 ) {
		return;
	}
	
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
        HIBYTE( wsaData.wVersion ) != 1 ) {
		WSACleanup( );
		return;
	}
	//--------------------------------------------------------------------------------------------------
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);//ipv4 or ipv6? ��or���ݱ���
	//typeΪ����protocolΪ0����Ĭ��TCP�׽��֣�typeΪ���ݱ���protocolΪ0��Ĭ��ΪUDP��
	
	SOCKADDR_IN addrSrv;// socket����У�������һ���ṹ��SOCKADDR_IN����������һЩ��Ϣ
	addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");//IP��ַ
    //	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//IP��ַ
	addrSrv.sin_family=AF_INET;//Э���壬��socket��ֻ����AF_INET
	addrSrv.sin_port=htons(6000);//�˿ں�
	
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));// �󶨶˿�
	
	listen(sockSrv,5);//�����յ�������Ϊ5
	
	//------------------------------------------------------------------------------------------------
	SOCKADDR_IN addrClient;// �����ϵĿͻ���ip��ַ
	int len=sizeof(SOCKADDR);
	cout<<"waiting for connected..."<<endl;


	while(1)
	{
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);// ���ܿͻ�������,��ȡ�ͻ��˵�ip��ַ
		char sendBuf[50];
		sprintf(sendBuf,"from server:Welcome %s to here!",inet_ntoa(addrClient.sin_addr));// �����Ϣ���ͳ�ȥ
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);// ������Ϣ���ͻ���
		char recvBuf[50];
		recv(sockConn,recvBuf,50,0);// ���ܿͻ�����Ϣ
		printf("%s\n",recvBuf);
		//closesocket(sockConn);//�Ͽ�����
	}
	
}

