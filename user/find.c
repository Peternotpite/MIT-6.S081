#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

void find(char* file_name,char* file_path);

int main(int argc, char *argv[])
{
    char* file_path=argv[1];
    char* file_name=argv[2];
    if(argc != 3)
    {
        fprintf(2, "Error,unless three argument! \n");
        exit(1);
    }

    find(file_name,file_path);
    exit(0);
}

void find(char* name,char* path)
{
    struct stat st;
    struct dirent de;
    int fd;
    char buf[512],*p;

    //文件是否打开成功
    if((fd = open(path,0)) < 0){
        fprintf(2,"Error,can't open file %s\n",path);
        return;
    }
    //fstat是否打开成功
    if(fstat(fd,&st) < 0){
        fprintf(2,"Error,can't stat %s\n",path);
        close(fd);
        return;
    }

    while(read(fd,&de,sizeof(de)) == sizeof(de) )
    {
        strcpy(buf,path);
        p=buf+strlen(buf);
        *p++='/'; //表示当前文件名结束
        if(de.inum == 0)
        {
            continue; //当前目录不存在或被删除
        }
        memmove(p,de.name,DIRSIZ);
        p[DIRSIZ] = 0;//在*p后面加'\0',说明字符串结束
        if(stat(buf,&st) < 0) //文件打开失败或者文件不存在
        {
            fprintf(2,"Error,can't stat %s",buf);
        }

        switch(st.type)
        {
            case T_FILE:
                if(strcmp(name,de.name) == 0)
                    printf("%s\n",buf); //buf是指针数组，文件名信息存在buf数组中，指针p只是指向buf的末尾
                break;
            
            case T_DIR:
                if(strcmp(de.name,".") != 0 && strcmp(de.name,"..") != 0) 
                //只要当前name(de.name)不等于"."和".."就可以递归
                    find(name,buf);
                break;
        }
    }
    close(fd);
    return;
}