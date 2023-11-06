#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>//UNIX标准函数定义
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //文件控制定义
#include <termios.h> //PPSIX终端控制定义
#include <errno.h> //错误号定义
#include <string.h>

#define STTY_DEV "/dev/ttyS0"
#define BUFF_SIZE 512

int main()
{
    int stty_fd,n;
    char buffer[BUFF_SIZE];
    struct termios opt;

    //打开串口设备
    stty_fd=open(STTY_DEV,O_RDWR);
    if(-1==stty_fd){
        perror("open device failed");
        return 0;
    }
    printf("open device success, waiting user input ... \n");

    //取得当前串口配置
    tcgetattr(stty_fd,&opt);
    tcflush(stty_fd,TCIOFLUSH);

    //设置波特率为19200bit/s
    cfsetispeed(&opt,B19200);
    cfsetospeed(&opt,B19200);

    //设置数据位 8位数据位
    opt.c_cflag &=~CSIZE;
    opt.c_cflag |=CS8;

    //设置奇偶位 无奇偶校验
    opt.c_cflag &=~PARENB;
    opt.c_iflag &= ~INPCK;

    //设置停止位 1位
    opt.c_cflag &=~CSTOPB;

    //设置超时事件位15秒
    opt.c_cc[VTIME] = 150;
    opt.c_cc[VMIN]=0;

    //设置写入设备
    if(0!=tcsetattr(stty_fd,TCSANOW,&opt)){
        perror("set baudrate");
        return 0;
    }
    tcflush(stty_fd,TCIOFLUSH);

    //读取数据，直到接受"quit"
    while(1)
    {
        n=read(stty_fd,buffer,BUFF_SIZE);
        if(n<=0){
            perror("read data");
            return 0;
        }
        buffer[n]='\0';
        printf("%s",buffer);
        if(0==strncmp(buffer,"quit",4)){
            printf("user send quit\n");
            break;
        }
    }
    printf("exit!\n");
    close(stty_fd);
    return 0;
}