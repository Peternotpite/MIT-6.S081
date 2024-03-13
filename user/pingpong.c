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
        //得到pid后，pid会自动存入数组元素buf[0]中，pid通过ASCII码以字符存入buf
        //（目前不知道怎么会自动存进去）
        char buf[10];
        //printf("sizeof(pid)=：%d",sizeof(pid));
        if(read(p[0],buf,8) != 8){
            fprintf(2,"Error,child don't get pid\n");
            exit(1);
        }
        close(p[0]);
        printf("%d: recived ping\n",pid);
        if(write(p[1],buf,8) != 8){
            fprintf(2,"Error,child don't send pid\n");
            exit(1);
        }
        close(p[1]);
        exit(0);
    }else{
        pid=getpid(); 
        //info同理
        char buf[10];
        char info[10]="a"; 
        buf[9]=0;

        if(write(p[1],info,8)!=8){
            fprintf(2,"Error,parent don't send pid\n");
            exit(1);
        }
        close(p[1]);
        wait(0);
        if(read(p[0],buf,8)!=8){
            fprintf(2,"Error,parent don't get pid\n");
            exit(1);
        }
        close(p[0]);
        printf("%d: recived pong\n",pid);
    }
    return 0;
}