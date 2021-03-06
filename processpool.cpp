#include"processpool.h"
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<sys/wait.h>
#include<iostream>
#include<dlfcn.h>
#include<cstring>
#include<cstdlib>
#include"module.h"
#include"response.h"
#include<cstdio>
using namespace std;
//externals
extern module**modules;
extern int module_num;

processpool::processpool(int listenfd,int processnum):listenfd(listenfd),processnum(processnum),idx(-1)
{
	subprocess=new process[processnum];
	for(int i=0;i<processnum;i++){
		if(pipe(subprocess[i].pipefd)<0){cout<<"pipe num "<<"i"<<" error"<<endl;}
		subprocess[i].mypid=fork();
		if(subprocess[i].mypid<0){cout<<"fork num "<<"i"<<" error"<<endl;}
		if(subprocess[i].mypid>0){close(subprocess[i].pipefd[0]);continue;}
		else{close(subprocess[i].pipefd[1]);idx=i;break;}
	}
}
processpool*processpool::get(int listenfd){
	if(pool==NULL)
	{
		pool=new processpool(listenfd);
	}
	return pool;
}
processpool::~processpool(){
	delete[]subprocess;
}
void processpool::run(){
	if(idx!=-1){runchild();return;}
	runparent();
}
void processpool::runchild(){
	while(1)
	{
	//most important part
		int b;
		int*buf=&b;
		read(subprocess[idx].pipefd[0],buf,sizeof(int));
		if(*buf==idx)
		{
			struct sockaddr_in clientaddr;
			unsigned int len=sizeof(clientaddr);
			int newsock=accept(listenfd,reinterpret_cast<sockaddr*>(&clientaddr),&len);
			if(newsock<0)
			{
				cout<<"[processpool]accept error"<<endl;
			}
			else
			{
				cout<<"[processpool]Child process No."<<idx<<" is working."<<endl;
			}
			char head[1024];
			for(int i=0;i<1024;i++)
				head[i]='\0';
			//cope with head

			read(newsock,head,1024);

			char method[100];
			for(int i=0;i<100;i++)
				method[i]='\0';

			char url[100];
			for(int i=0;i<100;i++)
				url[i]='\0';
			//GET Method and url from head
			sscanf(head,"%s %s",method,url);
			//Get rid of "/" before url
			strcpy(url,url+1);

			response*res=new response();
			bool flag=false;//modules
			for(int i=0;i<module_num;i++)
			{
				if(strcmp(modules[i]->name,url)==0)//same
				{
					modules[i]->command(res);
					flag=true;
				}
			}
			if(!flag)//other pages
			{
				FILE*fd;
				if(strcmp("index.html",url)==0)
				{
					fd=fopen("index.html","r");
				}
				else
				{
					fd=fopen("error.html","r");
				}
				fread(res->body,1,4096,fd);
				fclose(fd);
			}
			//HTML 
			char sen[4096];
			sprintf(sen,"HTTP/1.1 200 OK\r\n");
			sprintf(sen,"%sServer:myserver\r\n",sen);
			sprintf(sen,"%sContent-length:%lu\r\n",sen,strlen(res->body));
			sprintf(sen,"%sContent-type:text/html\r\n\r\n",sen);
			sprintf(sen,"%s%s",sen,res->body);
			send(newsock,sen,strlen(sen),0);
			close(newsock);
		}
	}
}
void processpool::runparent(){
	int epollfd=epoll_create(5);
	epoll_event event;
	event.events=EPOLLIN|EPOLLET;
	event.data.fd=listenfd;
	if(epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&event)<0){cout<<"epoll_ctl error"<<endl;exit(1);}
	epoll_event events[5];
	for(int j=0;j<10000;j++){
		int i=j%4;
		if(epoll_wait(epollfd,events,5,-1)<0){cout<<"epoll_wait error"<<endl;continue;}
		if(events[0].data.fd==listenfd){
			int b=i;
			int*buf=&b;
			write(subprocess[i].pipefd[1],buf,sizeof(int));
		}
	}
	close(epollfd);
}
processpool*processpool::pool=NULL;
