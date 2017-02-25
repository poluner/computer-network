#include<bits/stdc++.h>
#include<winsock2.h>
#include"tcp.h"
using namespace std;

int main(){
    WSAStartup(MAKEWORD(2, 2), new WSADATA());///注意，这里也要初始化winsock2环境

    sockaddr_in sockaddr_serv=getSockaddr(htonl(INADDR_ANY),htons(5000));
    int len_sockaddr_serv=sizeof(sockaddr_serv);
    SOCKET sock_stream=socket(AF_INET,SOCK_STREAM,0);

    if(bind(sock_stream,(sockaddr*)&sockaddr_serv,len_sockaddr_serv)==-1){
        puts("bind error");
        return 0;
    }
    puts("server build success");
    listen(sock_stream,5);

    puts("wait connect");
    SOCKET sock_msg=accept(sock_stream,(sockaddr*)&sockaddr_serv,&len_sockaddr_serv);//等待连接
    if(sock_msg==-1){
        puts("accept fail");
        return 0;
    }
    puts("accept success");
    trans(sock_msg);
    system("pause");
}
