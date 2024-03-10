#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        printf("Error,sleep is fail.\n");
        exit(1);
    }
    printf("sleep\n");
    sleep(atoi(argv[1]));
    exit(0);
}