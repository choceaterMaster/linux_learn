#include <stdio.h>
#include <malloc.h>
 
void print(char *, int);
int g1=12;
long g2;
int main(){
  char *s1 = "abcde";//位于text段
  char *s2 = "abcde";//位于text段，同s1
  char s3[] = "abcd";
  long int *s4[100];
  char *s5 = "abcde";//常量字符串"abcde"在常量区，但是s1,s2,s5本身在stack上，但它们用有相同的地址
  int a = 5;
  int b = 6; //a和b在stack上，所以&a>&b
  const int c =10;
  printf("变量地址\n&s1=%p\n&s2=%p\n&s3=%p\n&s4=%p\n&s5=%p\ns1=%p\ns2=%p\ns3=%p\ns4=%p\ns5=%p\na=%p\nb=%p\n",&s1,&s2,&s3,&s4,&s5,s1,s2,s3,s4,s5,&a,&b);
  printf("&g1=%p\n &g2=%p\n&c=%p\n",&g1,&g2,&c);
  printf("变量地址在进程调用中");
  print("ddddddd",5);
  printf("main=%p, print=%p\n",main,print);
  return 0;
}
void print(char *str, int p)
{
  char *s1 = "abcde";
  char *s2 = "abcde";
  char s3[] = "abcd";
  long int *s4[100];
  char *s5 = "abcde";//常量字符串"abcde"在常量区，但是s1,s2,s5本身在stack上，但它们用有相同的地址
  int a = 5;
  int b = 6; //a和b在stack上，所以&a>&b
  int c;
  int d;
  char *q = str;
  int m =p;
  char *r=(char*)malloc(1);
  char *w = (char*)malloc(1);
   printf("变量地址\n&s1=%p\n&s2=%p\n&s3=%p\n&s4=%p\n&s5=%p\ns1=%p\ns2=%p\ns3=%p\ns4=%p\ns5=%p\na=%p\nb=%p\n",&s1,&s2,&s3,&s4,&s5,s1,s2,s3,s4,s5,&a,&b);
   printf("str=%p\nq=%p\n&q=%p\n&p=%p\n&m=%p\nr=%p\nw=%p\n&r=%p\n&w=%p\n",&str,q,&q,&p,&m,r,w,&r,&w);
}

//输出 

/*
变量地址
&s1=0x7ffc2e9dd868                  
&s2=0x7ffc2e9dd870
&s3=0x7ffc2e9ddba0
&s4=0x7ffc2e9dd880
&s5=0x7ffc2e9dd878
s1=0x400948
s2=0x400948
s3=0x7ffc2e9ddba0
s4=0x7ffc2e9dd880
s5=0x400948
a=0x7ffc2e9dd85c
b=0x7ffc2e9dd860
&g1=0x601048
 &g2=0x601058
&c=0x7ffc2e9dd864
变量地址在进程调用中变量地址
&s1=0x7ffc2e9dd4e0
&s2=0x7ffc2e9dd4e8
&s3=0x7ffc2e9dd830
&s4=0x7ffc2e9dd510
&s5=0x7ffc2e9dd4f0
s1=0x400948
s2=0x400948
s3=0x7ffc2e9dd830
s4=0x7ffc2e9dd510
s5=0x400948
a=0x7ffc2e9dd4d4
b=0x7ffc2e9dd4d8
str=0x7ffc2e9dd4c8
q=0x4009df
&q=0x7ffc2e9dd4f8
&p=0x7ffc2e9dd4c4
&m=0x7ffc2e9dd4dc
r=0x873420
w=0x873440
&r=0x7ffc2e9dd500
&w=0x7ffc2e9dd508
main=0x4005d6, print=0x400729
*/