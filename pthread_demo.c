#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void* mid_thread(void* arg);//mid线程声明
void* term_thread(void* arg);//term线程声明
int main()
{
    pthread_t mid_tid,term_tid;//线程ID
    if(pthread_create(&mid_tid,NULL,mid_thread,NULL)){//创建线程
        perror("Create mid thread error!");
        return 0;
    }
    if(pthread_create(&term_tid,NULL,term_thread,&mid_tid)){
        perror("Create term thread fail!\n");
        return 0;
    }
    if(pthread_join(mid_tid,NULL)){//等待mid线程结束
        perror("wait mid thread error!");
    }
    if(pthread_join(term_tid,NULL)){
        perror("wait term thread error!");
        return 0;
    }
}

void* mid_thread(void* arg)
{
    int times=0;
    printf("mid thread create!\n");
    while(1){
        printf("waitting term thread %d times!\n",times);
        sleep(1);
        times++;
    }
}
void* term_thread(void* arg)
{
    pthread_t* tid;
    printf("term thread created!\n");
    sleep(2);
    if(NULL!=arg){
        tid=arg;
        pthread_cancel(*tid);//如果线程ID合法，则结束线程
    }
}