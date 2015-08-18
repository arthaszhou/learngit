#include "stdafx.h"
#include<WINSOCK2.H>
#include<string>
#include<iostream>
#include <iomanip>
#include <fstream>
#pragma comment(lib,"WS2_32.lib")
#define BUF_SIZE 64

using namespace std;

typedef struct VehilePos
{
	double x;
	double y;
	double th;
}VehilePos;
typedef struct PictureData
{
	int d;
	unsigned char r;
	unsigned char g;
	unsigned char b;
}PictureData;
	
void prcess_info(SOCKET s)
{
	int retVal,j=1;
	char snd_buf[BUF_SIZE];
	double temp[5];
	VehilePos send_v;
	PictureData send_p;
	ifstream infile;
	infile.open("colorpoints0523-14.txt"); //注意文件的路径
	if(!infile)
	{
		cerr<<"open error!"<<endl;
        exit(1);
    }
	while(1)
	{
		infile>>send_v.x>>send_v.y>>send_v.th;                   //每一帧数据中的车体位姿信息
		retVal = send(s,(char *)&send_v,sizeof(send_v),0);
		//printf("%lf,%lf,%lf\n",send_v.x,send_v.y,send_v.th);
		for(int i=0;i<761;i++)
		{
			infile>>temp[0]>>temp[1]>>temp[2]>>temp[3]>>temp[4];
			send_p.d = temp[0];
			send_p.b = temp[2] * 255;
			send_p.g = temp[3] * 255;
			send_p.r = temp[4] * 255;
			//printf("%d,%d,%d,%d\n",send_p.d,send_p.b,send_p.g,send_p.r);
			retVal = send(s,(char *)&send_p,sizeof(send_p),0);
		}
		//memset(snd_buf,0,BUF_SIZE);//清空发送缓存，不清空的话可能导致接收时产生乱码
		memcpy(snd_buf,"1",sizeof("1")); //结构体转换成字符串
		retVal = send(s,snd_buf,sizeof(snd_buf),0);
		j++;
		if(infile.eof()) 
		{
			memset(snd_buf,0,BUF_SIZE);//清空发送缓存，不清空的话可能导致接收时产生乱码
			memcpy(snd_buf,"2",sizeof("2")); //结构体转换成字符串
			retVal = send(s,snd_buf,sizeof(snd_buf),0);
			break;
		}
	}
	cout<<j<<endl;
	infile.close();
}

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsd;
	SOCKET sServer;
	SOCKADDR_IN servAddr;
	int retVal;
	char buf[BUF_SIZE];
	// 初始化Socket动态库
	if(WSAStartup(MAKEWORD(2,2),&wsd) != 0)
	{
		printf("WSAStartup failed !\n");
		return 1;
	}
	//创建Socket
	sServer = socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET == sServer)
	{
		printf("Socket failed !\n");
		WSACleanup();
		return -1;
	}
	//设置服务器地址
	printf("Please input the server's IP: ");
    std::string str;
	//接受输入的数据
	std::getline(std::cin,str);
	//将用户输入的数据复制到buf中
	ZeroMemory(buf,BUF_SIZE);
	strcpy(buf,str.c_str());

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr(buf);
	servAddr.sin_port = htons(9990);
	int sServerAddlen = sizeof(servAddr);
	//连接服务器
	retVal = connect(sServer,(LPSOCKADDR)&servAddr,sizeof(servAddr));
	if(SOCKET_ERROR == retVal)
	{
		printf("Connect failed !\n");
		closesocket(sServer);
		WSACleanup();
		return -1;
	}
	SYSTEMTIME st,st1;
	GetLocalTime(&st);
	//循环向服务器发送字符串，并显示反馈信息
	//while(true)
	//{
	//	printf("Please input a string to send: ");
	//    std::string str;
	//	//接受输入的数据
	//	std::getline(std::cin,str);
	//	//将用户输入的数据复制到buf中
	//	ZeroMemory(buf,BUF_SIZE);
	//	strcpy(buf,str.c_str());
	//	//向服务器发送数据
	//	retVal = send(sHost,buf,strlen(buf),0);
	//	if(SOCKET_ERROR ==retVal)
	//	{
	//		printf("Send failed !\n");
	//		closesocket(sHost);
	//		WSACleanup();
	//		return -1;
	//	}
	//	//接受服务器回传的数据
	//	retVal = recv(sHost,buf,sizeof(buf)+1,0);
	//	printf("Recv From Server: %s\n",buf);
	//}
	//释放Socket
	prcess_info(sServer);
	GetLocalTime(&st1);
	printf("%4d-%2d-%2d %2d:%2d:%2d\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	printf("%4d-%2d-%2d %2d:%2d:%2d\n",st1.wYear,st1.wMonth,st1.wDay,st1.wHour,st1.wMinute,st1.wSecond);
	closesocket(sServer);
	WSACleanup();
	//暂停，按任意键退出
	system("pause");
	return 0;
}

