#include <stdio.h>  
#include <stdlib.h>    
#include <string.h>         
#include <unistd.h>         
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <netdb.h>

#define true 1
#define false 0
#define MAXSIZE 2048
#define NAMELENTH 20
#define PORT 2999
#define BACKLOG 8
#define MAXTHREAD 8

struct info
{
    pthread_t thread_id;//当前线程id
    int index;//当前clients数组下表
    int connectfd; //当前连接套接字
};


unsigned int thread_num=0;//用于记录线程数量,当thread_num==MAXTHREAD就应该让新接入的客户端等待了。
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER; //线程互斥锁初始化，不确定可用。
struct info  clients[MAXTHREAD]={0};//保存客户端信息的数组，线程函数中会传入下标，在线程函数中应该使用传入的cur_index而不是thread_num
int useable[MAXTHREAD];//记录可用的clients下标，创建线程之后将相应下标置0，在线程执行完毕之后应将相应的下标置1；


void* client_in(void* cur_index);  //线程函数，参数为当前的线程下标，用于不同线程间的区分。
void take_order(char* cmd,int cur_index);//执行具体命令，为了方便操作，且因为线程间问题，在执行文件传输中直接对cliens下标进行操作
void send_msg(const char* msg,int index);//发送消息
void send_dir(int index);//发送当前目录内容
void recive_msg(int index);//接收消息
void recive_cmd(char* cmd_buf,int index);//接收命令
int recive_choice(int index);//接收选择
void send_file(char* filename,int index);//发送文件
void recive_file(char* filename,int index);//接收文件


int main()
{
    memset(useable,1,sizeof(useable));
    struct sockaddr_in saddr;
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0)
    {
        perror("socket");
        exit(-1);
    }
    int opt=SO_REUSEADDR;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_addr.s_addr=htons(INADDR_ANY);
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(bind(listenfd,(struct sockaddr*)&saddr,sizeof(struct sockaddr_in))<0)
    {
        perror("bind");
        exit(-1);
    }
    if((listen(listenfd,BACKLOG))<0)
    {
        perror("listen");
        exit(-1);
    }
    printf("------------------------------------------------\n");
    printf(" |server started, waiting for clients conncet|\n");
    printf("------------------------------------------------\n");
     
    while(1)
    {
        struct sockaddr_in caddr;
        memset(&caddr,0,sizeof(caddr));
        int n= sizeof(caddr);
        int fd=accept(listenfd,(struct sockaddr*)&caddr,&n);
        if(fd<0)
            {perror("connect");exit(-1);}
        printf("client has connected:%s",inet_ntoa(caddr.sin_addr));
        if(thread_num<=MAXTHREAD)
        {
            int index;
            for(index=0;index<MAXTHREAD;index++)
            {
                if(useable[index])
                    break;
            }
            send_msg("\0",index);
            int num=pthread_create(&(clients[index].thread_id),NULL,&client_in,&index);
            if(num<0)
            {
                printf("thread create fail!");
                exit(-1);
            }
            clients[index].connectfd=fd;
            clients[index].index=index;
            useable[index]=0;
            thread_num++;
        }
        else
        {
            printf("there is one client be hold!");
            //send(fd,"server is busy now ,retry later",MAXSIZE,0);//线程队列满，向客户端发送消息。
        }
    }
    close(listenfd);
    return 0;
}

void* client_in(void* cur_index)
{
    int is_continue=1;
    int index=*((int*)cur_index);
    char * cmd=(char*)malloc(MAXSIZE);
    send_msg("Hello!you has been connected with our server\nNow you can input your order:\n",index);
    while(is_continue)
    {
        recive_cmd(cmd,index);
        send_msg("recive your command, try to analyze....\n",index);
        take_order(cmd,index);
        send_msg("Wanna take another order?[Y/N]?\n",index);
        memset(cmd,0,MAXSIZE);
        is_continue=recive_choice(index);
        if(is_continue<0)
        {
            send_msg("wrong choice",index);
            break;
        }
    }
    close(clients[index].connectfd);
    useable[clients[index].index]=1;
    return ;
}

void take_order(char* cmd,int index)
{
    if(strncmp(cmd,"exit",4)==0)
	{
		pthread_cancel(clients[index].thread_id);
	}
	else if(strncmp(cmd,"ls",2)==0)
	{
        send_dir(index);
	}
	else if(strncmp(cmd,"get",3)==0)
	{
        char filename[MAXSIZE];
        memset(filename,0,MAXSIZE);
        strcat(filename,cmd+4);
        send_file(filename,index);
	}
	else if(strncmp(cmd,"put",4)==0)
	{
        char filename[MAXSIZE];
        memset(filename,0,MAXSIZE);
        strcat(filename,cmd+4);
        recive_file(filename,index);
	}
    else
    {
        send_msg("missing order!\n",index);
    }
}


void send_msg(const char* msg,int index)
{
    if(send(clients[index].connectfd,msg,MAXSIZE,0)<0)
       perror("send");
}

void send_dir(int index)
{
    int fd;
    int bakfd=dup(1);
    close(1);
    dup(fd);
    system("ls .");
    char buf[MAXSIZE];
    memset(buf,0,MAXSIZE);
    do
    {
    if(read(fd,buf,MAXSIZE)<0) perror("read");
	send_msg(buf,index);
    }
    while(buf[MAXSIZE-1]=='\0');
    close(fd);
    dup(bakfd);
}

void recive_msg(int index)
{
    char buf[MAXSIZE];
    do
    {
    memset(buf,0,MAXSIZE);
    if(recv(clients[index].connectfd,buf,MAXSIZE,0)<0)
          perror("recv");
    printf("%s",buf);
    }
    while(buf[MAXSIZE]=='\0');
}

void recive_cmd(char* cmd_buf,int index)
{
    memset(cmd_buf,0,MAXSIZE);
    if(recv(clients[index].connectfd,cmd_buf,MAXSIZE,0)<0)
           perror("recv");
    printf("recive commands:[%s]",cmd_buf);
}
int recive_choice(int index)
{
    char buf[MAXSIZE];
    memset(buf,0,MAXSIZE);
    if(recv(clients[index].connectfd,buf,MAXSIZE,0)<0)
           perror("recv");
    if(!strncmp("Y",buf,1)||!strncmp("y",buf,1))
       return true;
    else if(!strncmp("N",buf,1)||!strncmp("n",buf,1))
       return false;
    else   
       return -1;
}

void send_file(char* filename,int index)
{
    char buf[MAXSIZE];
    int fd= open(filename,O_RDONLY);
    if(fd<0)
        perror("open");
    do
    {
        memset(buf,0,MAXSIZE);
        if(read(fd,buf,MAXSIZE)<0) perror("read");
        if(send(clients[index].connectfd,buf,MAXSIZE,0)<0) perror("send");
    }
    while(buf[MAXSIZE-1]=='\0');
}

void recive_file(char*filename,int index)
{
    printf("restore file :%s",filename);
    char buf[MAXSIZE];
    int fd=open(filename,O_RDWR|O_CREAT);
    if(fd<0)  perror("open");
    do
    {
        memset(buf,0,MAXSIZE);
        if(recv(clients[index].connectfd,buf,MAXSIZE,0)<0) perror("recv");
        if(write(fd,buf,MAXSIZE)<0) perror("write");
        lseek(fd,MAXSIZE,SEEK_SET);
    }
    while(buf[MAXSIZE-1]=='\0');
}


