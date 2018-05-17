#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

/*
 *	TCP:
 *		int socket(int domain,int type,int protocol):创建socket
 *		int bind(int sockfd,struct sockaddr* addr,int addrlen):绑定socket
 *		int listen(int socketfd,int backlog):使socket处于监听状态，并初始化一个大小为
 *	backlog的请求列队
 *		int accept(int sockfd,void* addr,int* addrlen):阻塞处理客户端的请求消息
 *		int connect(int sockfd,struct sockaddr* ser_addr,int addrlen):连接服务器
 *		int send(int sockfd,void* msg,int len,int flags):发送数据
 *		int recv(int sockfd,void* msg,int len,int flags):接受数据
 *		int close(int sockfd):销毁socket
 *		uint16_t htons(uint16_t port):将本机字节序列(地址的低位存储值的低位)转换成网
 *	络字节序列(地址的低位存储值的高位)
 *		uint16_t ntohs(uint16_t port):将网络字节转换本机字节序列
 *		void bzero(void *s, int n):置字节字符串s的前n个字节为零
 *		in_addr inet_addr(char* cp):将字符串转换为in_addr类型
 *		char *inet_ntoa(struct in_addr addr):将in_addr类型转换为字符串
 * */

void tcpServer(){
	int sockfd,client_fd;
	struct sockaddr_in my_addr,client_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(10000);
	my_addr.sin_addr.s_addr=INADDR_ANY; //表示任何地址0.0.0.0
	bzero(&(my_addr.sin_zero),8);
	bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr));
	listen(sockfd,128);
	unsigned int sin_size =sizeof(struct sockaddr_in);
	while(1){
		client_fd=accept(sockfd,(struct sockaddr*)&client_addr,&sin_size);
		printf("收到一个请求: %s \n",inet_ntoa(client_addr.sin_addr));
		send(client_fd,"Hello I am Server\n",20,0);
		close(client_fd);
	}
}

void tcpClient(){
	int sockfd,revsize;
	char buf[20];
	struct sockaddr_in server_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(10000);
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1"); //表示任何地址0.0.0.0
	bzero(&(server_addr.sin_zero),8);
	connect(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
	revsize=recv(sockfd,buf,20,0);
	close(sockfd);
	printf("%d :%s",revsize,buf);
}

/*
 * UDP:
 *	 int sendto(int sockfd,void* buf,int len,unsigned flags,struct socketaddr* to,int tolen):
 *	发送数据
 *	 int recvfrom(int sockfd,void* buf,int len,unsigned flags,struct socketaddr* from,int* fromlen)
 *	接受数据
 * */

void udpServer(){
	int sockfd,revsize;
	char buf[20];
	struct sockaddr_in my_addr,client_addr;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(10000);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr));
	unsigned int sin_size =sizeof(struct sockaddr_in);
	while(1){
		revsize=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&client_addr,&sin_size);
		printf("%d :%s",revsize,buf);
	}
	close(sockfd);
}

void udpClient(){
	int sockfd;
	struct sockaddr_in server_addr;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(10000);
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	bzero(&(server_addr.sin_zero),8);
	sendto(sockfd,"Hello I am Client\n",20,0,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
	close(sockfd);
}

int main(){
	tcpServer();
	tcpClient();
	udpServer();
	udpClient();
	return 0;
}

/*
 * domain:
 * 		AF_INET:IPv4	AF_INET6:IPv6
 * type:
 * 		SOCK_STREAM:TCP	SOCK_DGRAM:UDP
 * protocol:
 * 		0:自动选择
 * */

/*
 * 1.socketaddr_in结构:
 * 		struct socketaddr_in{
 * 			unsigned short int sin_family; //通信地址类型
 * 			unit16_t sin_port;	//端口号
 * 			struct in_addr sin_addr; //IP地址
 * 			unsigned char sin_zero[8]; //填充为0,为了与socketaddr结构体兼容
 * 		};
 * */
