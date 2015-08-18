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
	infile.open("colorpoints0523-14.txt"); //ע���ļ���·��
	if(!infile)
	{
		cerr<<"open error!"<<endl;
        exit(1);
    }
	while(1)
	{
		infile>>send_v.x>>send_v.y>>send_v.th;                   //ÿһ֡�����еĳ���λ����Ϣ
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
		//memset(snd_buf,0,BUF_SIZE);//��շ��ͻ��棬����յĻ����ܵ��½���ʱ��������
		memcpy(snd_buf,"1",sizeof("1")); //�ṹ��ת�����ַ���
		retVal = send(s,snd_buf,sizeof(snd_buf),0);
		j++;
		if(infile.eof()) 
		{
			memset(snd_buf,0,BUF_SIZE);//��շ��ͻ��棬����յĻ����ܵ��½���ʱ��������
			memcpy(snd_buf,"2",sizeof("2")); //�ṹ��ת�����ַ���
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
	// ��ʼ��Socket��̬��
	if(WSAStartup(MAKEWORD(2,2),&wsd) != 0)
	{
		printf("WSAStartup failed !\n");
		return 1;
	}
	//����Socket
	sServer = socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET == sServer)
	{
		printf("Socket failed !\n");
		WSACleanup();
		return -1;
	}
	//���÷�������ַ
	printf("Please input the server's IP: ");
    std::string str;
	//�������������
	std::getline(std::cin,str);
	//���û���������ݸ��Ƶ�buf��
	ZeroMemory(buf,BUF_SIZE);
	strcpy(buf,str.c_str());

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr(buf);
	servAddr.sin_port = htons(9990);
	int sServerAddlen = sizeof(servAddr);
	//���ӷ�����
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
	//ѭ��������������ַ���������ʾ������Ϣ
	//while(true)
	//{
	//	printf("Please input a string to send: ");
	//    std::string str;
	//	//�������������
	//	std::getline(std::cin,str);
	//	//���û���������ݸ��Ƶ�buf��
	//	ZeroMemory(buf,BUF_SIZE);
	//	strcpy(buf,str.c_str());
	//	//���������������
	//	retVal = send(sHost,buf,strlen(buf),0);
	//	if(SOCKET_ERROR ==retVal)
	//	{
	//		printf("Send failed !\n");
	//		closesocket(sHost);
	//		WSACleanup();
	//		return -1;
	//	}
	//	//���ܷ������ش�������
	//	retVal = recv(sHost,buf,sizeof(buf)+1,0);
	//	printf("Recv From Server: %s\n",buf);
	//}
	//�ͷ�Socket
	prcess_info(sServer);
	GetLocalTime(&st1);
	printf("%4d-%2d-%2d %2d:%2d:%2d\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	printf("%4d-%2d-%2d %2d:%2d:%2d\n",st1.wYear,st1.wMonth,st1.wDay,st1.wHour,st1.wMinute,st1.wSecond);
	closesocket(sServer);
	WSACleanup();
	//��ͣ����������˳�
	system("pause");
	return 0;
}

