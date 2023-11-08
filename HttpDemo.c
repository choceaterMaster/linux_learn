//使用HTTP从网站获取一个页面
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

void sig_int(int sig);

int GetHttpHeader(char* buff,char* header);

#define PRINTERROR(s) \
fprintf(stderr,"\nError at %s, errno = %d\n",s,errno)

int main()
{
    int bytes_all=0;
    char* host_name="www.4399.com";
    int nRet;
    int sock_fd;
    struct sockaddr_in server_addr;
    struct hostent* host_entry;
    char strBuffer[2048]={0};
    char strHeader[1024]={0};//存放HTTP请求报文头
    //安装SIGINT信号响应函数
    signal(SIGINT,sig_int);
    sock_fd=socket(PF_INET,SOCK_STREAM,0);
    if(sock_fd==1){
        PRINTERROR("socket()");
        return -1;
    }
    host_entry=gethostbyname(host_name);//获取域名对应的IP地址
    server_addr.sin_port=htons(80);
    server_addr.sin_family=PF_INET;
    server_addr.sin_addr=(*(struct in_addr*)*(host_entry->h_addr_list));
    nRet=connect(sock_fd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
    if(nRet==-1){
        PRINTERROR("connect()");
        close(sock_fd);
        return -1;
    }
    //构造HTTP请求报文头
    sprintf(strBuffer,"GET / HTTP1.1\r\n");
    strcat(strBuffer,"Accept */*\r\n");
    strcat(strBuffer,"Connection:Keep-Alive\r\n");

    //发送HTTP请求
    nRet=send(sock_fd,strBuffer,strlen(strBuffer),0);
    if(nRet==-1){
        PRINTERROR("send()");
        close(sock_fd);
        return -1;
    }
    //获取服务器返回的页面内容
    while(1)
    {
        nRet=recv(sock_fd,strBuffer,sizeof(strBuffer),0);
        if(nRet==-1){
            PRINTERROR("recv()");
            return -1;    
        }
        bytes_all+=nRet;//累加服务器返回页面内容的字节数
        if(0==GetHttpHeader(strBuffer,strHeader)){
            printf("%s",strHeader);
        }
        //检查服务器是否关闭连接
        if(nRet==0){
            fprintf(stderr,"\n %d bytes received.\n",bytes_all);
            break;
        }
        //打印服务器返回的内容
        printf("%s",strBuffer);
    }
    //关闭连接
    close(sock_fd);
    return 0;
}

void sig_int(int sig)//中断信号响应函数
{
    printf("Ha ha , we get SIGINT!\n");
}
//获取HTTP协议头
int GetHttpHeader(char* buff,char* header)
{
    char* p,*q;
    int i=0;
    p=buff;//缓冲区头
    q=header;//协议头
    if(NULL==p) return -1;
    if(NULL==q) return -1;
    while('\0'==(*p)){
        q[i]=p[i];
        if((p[i]==0x0d)&&(p[i+1]==0x0a)&&
        (p[i+2]==0x0d)&&(p[i+3]==0x0a)){//判断是否句子结尾
            q[i+1]=p[i+1];
            q[i+2]=p[i+2];
            q[i+3]=p[i+3];
            q[i+4]=0;
            return 0;
        }
        i++;
    }
    return -1;
}