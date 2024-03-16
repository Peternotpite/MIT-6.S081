#include "kernel/types.h"
#include "user/user.h"
#define PRIME_NUM 35
#define READEND 0
#define WRITEEND 1
void prime(int *p);

int main(int argc, char *argv[])
{
    int p_left[2];
    pipe(p_left);

    if(fork()==0)
    {
        close(p_left[WRITEEND]);
        prime(p_left);
    }else{
        //father process
        close(p_left[READEND]);
        for(int i=2;i<=PRIME_NUM;i++)
        {
           if(write(p_left[WRITEEND],&i,4) != 4)
           {
                exit(0);
           }
        }
        wait(0);
    }
    exit(0);
}

void prime(int *p)
{
    int p_child[2];
    int *buf=(int*) malloc(sizeof(int));
    close(p[WRITEEND]);
   
    if(read(p[READEND],buf,4) != 4)
    {
        exit(0);
    }
    pipe(p_child);

    if(fork()==0)
    {
        prime(p_child);

    }else{
        //child's father process
        close(p_child[READEND]);
        printf("prime %d\n",*buf);
        int prime = *buf;

        while(read(p[READEND],buf,4) != 0)
        {
            if(*buf%prime != 0)
            {
                write(p_child[WRITEEND],buf,4);
            }
        }
        close(p_child[WRITEEND]);
        wait(0);
        exit(0);
    }
        
}