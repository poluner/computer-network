#include <bits/stdc++.h>
#include <winsock2.h>
#include "ping.h"
using namespace std;

SOCKET getSocket(int timeout){//使用ICMP协议创建套接字
    SOCKET socket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);//设置套接字
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));//设置接收时延
    setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));//设置发送时延
    return socket;
}

sockaddr_in getSockaddr(DWORD ip){//用ip填充Socket地址
    sockaddr_in sockaddr;
    memset(&sockaddr,0,sizeof(sockaddr_in));//将sin_zero填充字段设0，没有设置sin_port？
    sockaddr.sin_family = AF_INET;//用ipv4通信
    sockaddr.sin_addr.s_addr = ip;
    return sockaddr;
}

DWORD getIp(char *name){
    DWORD ip = inet_addr(name);//将命令行参数转换为IP地址
    if(ip!=INADDR_NONE)return ip;
    hostent* ph = gethostbyname(name);
    if(ph==0)return INADDR_NONE;//解析主机名失败
    return (*(in_addr*)ph->h_addr).s_addr;
}

WORD GenerateChecksum(WORD* buf, int Size) {//产生校验和
    DWORD cksum = 0;
    while (Size>1) {
        cksum += *buf++;
        Size -= sizeof(WORD);
    }
    if (Size)cksum += *(UCHAR*)buf;

    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (WORD)(~cksum);
}

int main(int argc,char *argv[]) {
    WSAStartup(MAKEWORD(2, 2), new WSADATA());///初始化winsock2环境

    puts("input host name or ip");
    char name[30];
    scanf("%s",name);
    DWORD ip=getIp(name);
    if(ip==INADDR_NONE){
        puts("Could not resolve the host name");
        WSACleanup();
        return -1;
    }
    char *ip_in=inet_ntoa(*(in_addr*)(&ip));//ip点分十进制
    printf("\nPinging to %s [%s] with %d bytes of data:\n\n",name,ip_in,ICMP_DATA_SIZE);

    sockaddr_in sockaddr_dest=getSockaddr(ip);
    SOCKET socket =getSocket(TIMEOUT);

    const int ICMP_SIZE=sizeof(IcmpHeader)+ICMP_DATA_SIZE;
    char icmpHeaderBuf_send[ICMP_SIZE];
    char ipBuf_recv[IP_SIZE];

    //填充icmp数据包各字段
    IcmpHeader *icmpHeader_send = (IcmpHeader *)icmpHeaderBuf_send;
    icmpHeader_send->type = 8;//回显请求8
    icmpHeader_send->code = 0;//回显请求(ping请求)
    icmpHeader_send->id = GetCurrentProcessId();

    for(WORD seq = 0; seq <4; seq++) {//发送4个icmp请求回显报文
        icmpHeader_send->cksum = 0;///注意：用整个icmp计算cksum，icmp包含了chsum，所以要将chsum先设成0再参与计算cksum
        icmpHeader_send->cksum = GenerateChecksum((WORD*)icmpHeaderBuf_send,ICMP_SIZE);

        DWORD time_send = GetTickCount();//发送时刻
        //发送
        sendto(socket, icmpHeaderBuf_send, sizeof(icmpHeaderBuf_send),0,(sockaddr*)&sockaddr_dest, sizeof(sockaddr_dest));

        //接收
        sockaddr_in from;
        int len_from = sizeof(from);
        if(recvfrom(socket,ipBuf_recv,IP_SIZE,0,(sockaddr *)&from,&len_from)==-1){//收到ip数据包
            puts("Request timed out.");
            continue;
        };
        DWORD time_recv=GetTickCount();//立刻保存接收时刻

        IpHeader *ipHeader = (IpHeader *)ipBuf_recv;
        char *ip_source=inet_ntoa(*(in_addr*)(&ipHeader->ip_source));//从返回的ip报文中获取ip
        printf("Reply from %s : bytes=%d RTT=%dms, TTL=%c.\n",ip_source,ICMP_DATA_SIZE,time_recv-time_send,ipHeader->ttl);
    }
    puts("\nPing complete.\n");
    closesocket(socket);
    WSACleanup();
    system("pause");
}
