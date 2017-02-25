#include<bits/stdc++.h>
#include<winsock2.h>
#include"udp.h"
using namespace std;

char buf_send[4096];
char buf_recv[4096];
DWORD WINAPI recv(LPVOID p){//服务器接收客户端消息
    sockaddr_in sockaddr_clie;//从recvfrom中获取客户地址等信息
    int len_sockaddr_clie=sizeof sockaddr_clie;
    int len_recv=recvfrom(*(SOCKET*)p,buf_recv,sizeof(buf_recv),0,(sockaddr*)&sockaddr_clie,(int*)&len_sockaddr_clie);
    if(len_recv==-1)puts("recv error");
    else {
        buf_recv[len_recv]=0;
        char *ip_clie=inet_ntoa(*(in_addr*)(&sockaddr_clie.sin_addr));
        printf("message from %s: %s\n",ip_clie,buf_recv);
    }
    return 0;
}

DWORD WINAPI send(LPVOID p){//客户端向服务器发送
    char ip_serv[30];
    scanf("%s %s",ip_serv,buf_send);//每次发送消息都要先发ip
    sockaddr_in sockaddr_serv=getSockaddr(inet_addr(ip_serv),htons(200));
    int len=strlen(buf_send);
    int len_send=sendto(*(SOCKET*)p,buf_send,len,0,(sockaddr*)&sockaddr_serv,sizeof sockaddr_serv);
    if(len_send!=len)puts("send error");
    return 0;
}

int main(){
    WSAStartup(MAKEWORD(2, 2), new WSADATA());

    SOCKET sock_serv=socket(AF_INET,SOCK_DGRAM,0);
    sockaddr_in sockaddr_serv=getSockaddr(htonl(INADDR_ANY),htons(200));//服务器绑定任意地址以接收客户端消息
    if(bind(sock_serv,(sockaddr*)&sockaddr_serv,sizeof sockaddr_serv)==-1){
        puts("bind error");
        return 0;
    }
    puts("bind success");

    SOCKET sock_clie=socket(AF_INET,SOCK_DGRAM,0);//客户端socket不必绑定
    while(true){
        WaitForSingleObject(CreateThread(NULL, 0, send, &sock_clie, 0, NULL), 0);//这里用sock_serv也可以，不过用sock_clie说明问题更加清楚
        WaitForSingleObject(CreateThread(NULL, 0, recv, &sock_serv, 0, NULL), 0);
    }
}
