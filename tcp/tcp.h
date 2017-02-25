sockaddr_in getSockaddr(DWORD ip,DWORD port){//填充Socket地址
    sockaddr_in sockaddr;
    memset(&sockaddr,0,sizeof(sockaddr_in));//将sin_zero填充字段设0
    sockaddr.sin_family = AF_INET;//用ipv4通信
    sockaddr.sin_addr.s_addr = ip;
    sockaddr.sin_port=port;
    return sockaddr;
}

char buf_send[100];
char buf_recv[100];
DWORD WINAPI send(LPVOID p){
    scanf("%s",buf_send);
    int len=strlen(buf_send);
    int len_send=send(*(SOCKET*)p,buf_send,len,0);
    if(len_send!=len)puts("send error");
    return 0;
}

DWORD WINAPI recv(LPVOID p){
    int len_recv=recv(*(SOCKET*)p,buf_recv,sizeof buf_recv,0);
    if(len_recv==-1)puts("recv error");
    else {
        buf_recv[len_recv]=0;
        puts(buf_recv);
    }
    return 0;
}


void trans(SOCKET sock){
    while(true){
        WaitForSingleObject(CreateThread(NULL, 0, send, &sock, 0, NULL), 0);
        WaitForSingleObject(CreateThread(NULL, 0, recv, &sock, 0, NULL), 0);
    }
}
