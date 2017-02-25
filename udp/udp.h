sockaddr_in getSockaddr(DWORD ip,DWORD port){//填充Socket地址
    sockaddr_in sockaddr;
    memset(&sockaddr,0,sizeof(sockaddr_in));//将sin_zero填充字段设0
    sockaddr.sin_family = AF_INET;//用ipv4通信
    sockaddr.sin_addr.s_addr = ip;
    sockaddr.sin_port=port;
    return sockaddr;
}
