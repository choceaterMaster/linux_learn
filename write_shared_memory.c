#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int shmid;//共享内存ID
    char* ptr;
    char* shm_str="string in a share memory";
    shmid=shmget(0x90,1024,SHM_W|SHM_R|IPC_CREAT|IPC_EXCL);
    if(-1==shmid)
        perror("create share memory");
    ptr=(char*)shmat(shmid,0,0);
    if((void*)-1==ptr)
        perror("get share memory");
    strcpy(ptr,shm_str);
    shmdt(ptr);
    return 0;
}