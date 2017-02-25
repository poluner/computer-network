//IP数据报头
//8位BYTE char
//16位WORD short
//32位DWORD long

const DWORD TIMEOUT	= 3000;	//默认超时时间，单位ms
const int ICMP_DATA_SIZE	= 32;	//默认ICMP数据部分长度
const int IP_SIZE	= 1024;	//最大IP数据报的大小
const int MAX_HOP=30;

struct IpHeader {//p127
    //按字节传输，字节中从高位到低位一次传输比特流
    BYTE	len_hdr	: 4;    /// 头部长度单位是4字节
    BYTE	version	: 4;	// version of IP
    BYTE	tos;			// type of service
    WORD	len_total;		// total length of the packet

    WORD	identifier;		// unique identifier
    WORD	frag_and_flags;	//片偏移13位和片标志3位
    BYTE	ttl;			/// time to live
    BYTE	protocol;		// protocol (TCP, UDP etc)
    WORD    cksum;		    // IP checksum

    DWORD	ip_source;		// source IP address
    DWORD   ip_dest;		// destination IP address
};

//ICMP数据报头
struct IcmpHeader {
    BYTE	type;		//8位类型
    BYTE	code;		//8位代码
    WORD	cksum;		//16位校验和

    WORD	id;			///16位标识符，icmp没有这个呀？
    WORD	seq;		//16位序列号
};
