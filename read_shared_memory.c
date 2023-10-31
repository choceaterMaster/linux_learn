#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
    int shmid;
    char* ptr;
    shmid=shmget(0x90,1024,SHM_W|SHM_R|IPC_EXCL);
    if(-1==shmid)
        perror("create share memory");
    ptr=shmat(shmid,0,0);
    if((void*)-1==ptr)
        perror("get share memory");
    printf("string in share memory: %s\n",ptr);
    shmdt(ptr);
    return 0;
}