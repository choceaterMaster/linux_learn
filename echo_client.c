#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define EHCO_PORT 8080
#define MAX_COMMAND 5

int main()
{
    int sock_fd;
    struct sockaddr_in serv_addr;
    char* buff[MAX_COMMAND]={"abc","def","test","hello","quit"};
    char tmp_buf[100];
    int n,i;

    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd==-1){
        perror("create socket error!");
        return 0;
    } else {
        printf("success to create socket %d\n",sock_fd);
    }

    //设置server地址结构
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(EHCO_PORT);
    serv_addr.sin_addr.s_addr=htons(INADDR_ANY);//设置服务器地址
    bzero(&(serv_addr.sin_zero),8);

    //连接到服务器
    if(-1==connect(sock_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))){
        perror("connect() error!\n");
        close(sock_fd);
        return 0;
    } else printf("success connect to server!\n");

    //发送并接受缓冲的数据
    for(i=0;i<MAX_COMMAND;i++){
        send(sock_fd,buff[i],100,0);
        n=recv(sock_fd,tmp_buf,100,0);
        tmp_buf[n]='\0';
        printf("data send: %s receive: %s\n",buff[i],tmp_buf);
        if(0==strncmp(tmp_buf,"quit",4))
            break;
    }
    close(sock_fd);
    return 0;
}