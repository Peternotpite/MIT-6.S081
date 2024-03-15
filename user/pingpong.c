#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2]; 
    int pid;
    pipe(p); //p[0]输出，p[1]输入

    if (fork() == 0) { 
        pid = getpid();
        //得到pid后，pid会一直存放在管道的输出缓冲区，
        //"1"只是为了说明进程能够正确读取管道的信息
        char buf[2];
        //printf("sizeof(pid)=：%d",sizeof(pid));
        if(read(p[0],buf,1) != 1){
            fprintf(2,"Error,child don't get pid\n");
            exit(1);
        }
        close(p[0]);
        printf("%d: recived ping\n",pid);
        if(write(p[1],buf,1) != 1){
            fprintf(2,"Error,child don't send pid\n");
            exit(1);
        }
        close(p[1]);
        exit(0);
    }else{
        pid=getpid(); 
        //info同理
        char buf[2];
        char info[2]="a"; 
        buf[1]=0;

        if(write(p[1],info,1) != 1){
            fprintf(2,"Error,parent don't send pid\n");
            exit(1);
        }
        close(p[1]);
        wait(0);
        if(read(p[0],buf,1) != 1){
            fprintf(2,"Error,parent don't get pid\n");
            exit(1);
        }
        close(p[0]);
        printf("%d: recived pong\n",pid);
        exit(0);
    }
}



