#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int c2i(char ch)
{
    if(isdigit(ch))
        return ch-48;
    if(ch<'A' || (ch >'F' && ch<'a')|| ch > 'z')
        return -1;
    if(isalpha(ch))
        return isupper(ch)? ch-55:ch-87;
    return -1;
}
int hex2dec(char* hex)
{
    int len;
    int num=0;
    int temp;
    int bits;
    int i;
    char str[64]={0};
    if(NULL==hex)
    {
        printf("input para error!\n");
        return 0;
    }
    if(('0'==hex[0])&&(('X'==hex[1])||('x'==hex[1])))
    {
        strcpy(str,&hex[2]);
    }else{
        strcpy(str,hex);
    }
    printf("input num=%s \n",str);

    len=strlen(str);
    for(i=0,temp=0;i<len;i++,temp=0)
    {
        temp=c2i(*(str+i));
        bits=(len -i -1)*4;
        temp=temp<<bits;
        num=num|temp;
    }
    return num;

}
int main(int argc,char ** argv)
{
    int l_s32Ret=0;
    if(2!=argc)
    {
        printf("error!\n");
        printf("usage: %s Num \n",argv[0]);
        printf("eg 1: %s 0x400\n",argv[0]);
        return 0;
    }
    l_s32Ret=hex2dec(argv[1]);
    printf("argv[0]= %s, argv[1]=%s\n",argv[0],argv[1]);
    printf("value hex = 0x%x \n",l_s32Ret);
    printf("value dec = %d \n",l_s32Ret);
    return 0;
}