#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
int main(int argc,char** argv)
{
    int opt;
    char* string="a::b:c:d";
    while((opt=getopt(argc,argv,string))!=-1)
    {
        printf("opt = %c\t\t",opt);
        printf("optarg=%s\t\t",optarg);
        printf("optind=%d\t\t",optind);
        printf("argv[optind]=%s\n",argv[optind]);
    }
}