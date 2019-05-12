#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>     
#include <fcntl.h>     
#include <netdb.h>

#define MAXSIZE 2048
#define PORT 2999

int sockfd=-1;

void recive_msg()//接受消息
{
    char buf[MAXSIZE];
    do
    {
    memset(buf,0,MAXSIZE);
    if(recv(sockfd,buf,MAXSIZE,0)<0)
       perror("recv");
    printf("%s",buf);
    } while (buf[MAXSIZE-1]=='\0');

};

void recive_file(char *filename)//接受文件
{
    
    printf("restore file with name : %s",filename);
    char buf[MAXSIZE];
    int fd = open(filename,O_RDWR|O_CREAT);
    if(fd<0)
       perror("open");
    do
    {
    memset(buf,0,MAXSIZE);
    if(recv(sockfd,buf,MAXSIZE,0)<0)
       perror("recv");
    if(write(fd,buf,MAXSIZE)<0)
       perror("write");
    lseek(fd,MAXSIZE,SEEK_SET);
    }
    while(buf[MAXSIZE-1]=='\0');

}

void send_msg(const char* msg)//发送消息
{
    if(send(sockfd,msg,MAXSIZE,0)<0)
       perror("send");
}

void send_file(char* filename)//发送文件
{
    printf("send %s to server",filename);
    int fd= open(filename,O_RDONLY);
    if(fd<0)
     {perror("open");exit(-1);}
    char buf[MAXSIZE];
    do
    {
        memset(buf,0,MAXSIZE);
        if(read(fd,buf,MAXSIZE)<0)
           perror("read");
        if(send(sockfd,buf,MAXSIZE,0)<0)
           perror("send");
    }
    while(buf[MAXSIZE-1]=='\0');
    
}

void send_order(char * cmd)
{
    if(strncmp(cmd,"exit",4)==0)
	{
		send_msg(cmd);
	}
	else if(strncmp(cmd,"ls",2)==0)
	{
        send_msg(cmd);
	}
	else if(strncmp(cmd,"get",3)==0)
	{
        char filename[MAXSIZE];
        memset(filename,0,MAXSIZE);
        strcat(filename,cmd+4);
        recive_file(filename);
	}
	else if(strncmp(cmd,"put",4)==0)
	{
        char filename[MAXSIZE];
        memset(filename,0,MAXSIZE);
        strcat(filename,cmd+4);
        send_file(filename);
	}
}



int main()
{
    fflush(stdin);
    char * buf=(char*)malloc(MAXSIZE);
    memset(buf,0,MAXSIZE);
    printf("Try to login server,please input the address of server:");
    scanf("%s",buf);

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){perror("socket");exit(-1);}
    struct sockaddr_in saddr;
    struct hostent * host;

    if((host=gethostbyname(buf))==NULL)
    {
        printf("name solution fail\n");
        exit(-1);
    }

    memset(&saddr,0,sizeof(saddr));
    saddr.sin_addr=*((struct in_addr *)host->h_addr_list[0]);
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
if(connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr))<0)
   {
       perror("connect");exit(-1);
   }
    while(1)
   {
       memset(buf,0,MAXSIZE);
       recv(sockfd,buf,MAXSIZE,0);
       if(buf[0]!='\0')
       {
           printf("%s",buf);
       }
       else break;
   }
   memset(buf,0,MAXSIZE);
    while(1)
   {
       recive_msg();
       memset(buf,0,MAXSIZE);
       fflush(stdin);
       fgets(buf,MAXSIZE,stdin);
       buf[strlen(buf)-1]='\0';
       send_order(buf);
   }
}

