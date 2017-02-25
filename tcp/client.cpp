#include<bits/stdc++.h>
#include<winsock2.h>
#include"tcp.h"
using namespace std;

int main(){
    WSAStartup(MAKEWORD(2, 2), new WSADATA());///注意，这里也要初始化winsock2环境

    puts("input server's ip");
    char ip_serv[30];
    scanf("%s",ip_serv);
    sockaddr_in sockaddr_clie=getSockaddr(inet_addr(ip_serv),htons(5000));
    SOCKET sock_msg=socket(AF_INET,SOCK_STREAM,0);

    while(connect(sock_msg,(sockaddr*)&sockaddr_clie,sizeof(sockaddr_clie))){
        puts("wait 1s");
        Sleep(1000);
    }
    puts("accept success");
    trans(sock_msg);
    system("pause");
}
