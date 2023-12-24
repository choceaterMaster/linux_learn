#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
 
int main(int argc, char* argv[])
{
	char buf[30];
	int size=0;
    int mysize=0;
	//int line=0;
 
	if(argc==1){
		printf("wrong number of arguments.\n");
		return -1;
	}
	
	int fd=open(argv[1], O_RDONLY);
	if(fd==-1){
		printf("open file failed.\n");
		return -1;
	}
	
 
	while(size=read(fd, buf, 29)){
		buf[size]='\0';
		fputs(buf, stderr);
        mysize++;
	}
    printf("\n size is %d",mysize);
	return 0;
}