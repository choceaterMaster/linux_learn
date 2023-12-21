#include <stdio.h>
int main(int argc,char** argv)
{
    int c;
    while(--argc>0 && (*++argv)[0]=='-')
    {
        while(c=*++argv[0]){
            switch(c){
                case 'x':
                    printf("op is 'x'\n");
                    break;
                case 'a':
                    printf("op is 'a'\n");
                    break;
                default:
                    printf("illegal opyion %c\n",c);
                    break;
            }
        }
    }
}