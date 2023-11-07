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
// #define STTY_DEV "/dev/bus/usb/003/009"
#define BUFF_SIZE 512

int SetOption(int fd)
{
    struct termios opt;
    //取得当前串口配置
    tcgetattr(fd,&opt);
    tcflush(fd,TCIOFLUSH);

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
    if(0!=tcsetattr(fd,TCSANOW,&opt)){
        perror("set baudrate");
        return -1;
    }
    tcflush(fd,TCIOFLUSH);
    return 0;
}
int main()
{
    int stty_fd,n;
    int cd;
    char buffer[BUFF_SIZE];
    char phone[20]="+8619812345678";
    char sms_number[20]="+8613010701500";//定义短消息中心号码
    char sms_gb2312[140]="工作愉快！";
    char sms_utf8[140];
    char* sms_in=sms_gb2312;
    char* sms_out=sms_utf8;
    int str_len,i,tmp;
    size_t gb2312_len,utf8_len;

    //打开串口设备
    stty_fd=open(STTY_DEV,O_RDWR);
    if(-1==stty_fd){
        perror("open device");
        return 0;
    }
    printf("Open device success!\n");

    //设置串口参数
    if(0!=SetOption(stty_fd)){
        close(stty_fd);
        return 0;
    }

    //转换电话号
    if(phone[0]=='+'){
        for(i=0;i<strlen(phone)-1;i++)
            phone[i]=phone[i+1];
    }
    phone[i]='\0';
    str_len=strlen(phone);
    if((strlen(phone)%2)!=0){ //若号码长度为奇数，则末尾加F
        phone[str_len]='F';
        phone[str_len+1]='\0';
    }
    for(i=0;i<strlen(phone);i+=2){ //把号码的奇偶位调换
        tmp=phone[i];
        phone[i]=phone[i+1];
        phone[i+1]=tmp;
    }

    //转换短消息中心号码到符合的格式
    if(sms_number[0]=='+'){//去掉号码开头的+
        for(i=0;i<strlen(sms_number)-1;i++)
            sms_number[i]=sms_number[i+1];
    }
    sms_number[i]='\0';

    str_len=strlen(sms_number);
    if((strlen(sms_number)%2)!=0){//若长度是奇数，则后面加F
        sms_number[str_len]='F';
        sms_number[str_len+1]='\0';
    }
    for(i=0;i<strlen(sms_number);i+=2){//把号码的奇偶调换
        tmp=sms_number[i];
        sms_number[i]=sms_number[i+1];
        sms_number[i+1]=tmp;
    }
    str_len=strlen(sms_number);
    for(i=strlen(sms_number)+2;i!=0;i--){//所有的字符向后移动两个字节
        sms_number[i]=sms_number[i-2];
    }
    sms_number[str_len+3]='\0';
    strncpy(sms_number,"91",2);//开头写入字符串91
    tmp=strlen(sms_number)/2;//计算字符串长度
    str_len=strlen(sms_number);
    for(i=strlen(sms_number)+2;i!=0;i--){
        sms_number[i]=sms_number[i-2];
    }
    sms_number[str_len+3]='\0';
    sms_number[0]=(char)(tmp/10)+0x30;//将字符串西写入短信字符串的开头部分
    sms_number[1]=(char)(tmp%10)+0x30;
    //转换短消息内容
    cd=iconv_open("utf-8","gb2312");//gb2312->utf-8
    if(0==cd){
        perror("create iconv handle!");
        close(stty_fd);
        return 0;
    }
    gb2312_len=strlen(sms_gb2312);
    utf8_len=140;
    if(-1==iconv(cd,&sms_in,&gb2312_len,&sms_out,&utf8_len)){
        //转换字符位Unicode编码
        perror("convert code");
        close(stty_fd);
        return 0;
    }
    iconv_close(cd);
    //设置使用PDU模式
    strcpy(buffer,"AT+CMGF=0\n");
    write(stty_fd,buffer,strlen(buffer));
    n=read(stty_fd,buffer,BUFF_SIZE);
    if(n<=0){
        perror("set pdu mode");
        close(stty_fd);
        return 0;
    }
    if(0!=strncmp(buffer,"OK",2)){//判断命令是否执行成功
        perror("set pdu mode");
        close(stty_fd);
        return 0;
    }
    //发送消息
    sprintf(buffer,"AT+CMGS=%d\n",utf8_len);
    write(stty_fd,buffer,strlen(buffer));
    write(stty_fd,sms_utf8,utf8_len);
    printf("Send Message OK!\n");
    close(stty_fd);
    return 0;

}