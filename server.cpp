#include"processpool.h"
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<cstring>
#include<cstdlib>
#include"module.h"
using namespace std;
int seekports(int listenfd);
int main(int argc,char*argv[])
{
	cout<<"Welcome to the server!"<<endl;
	cout<<"In this server, you can get some information about the device and the OS"<<endl;
	//modules
	load_modules();

	//socket
	int listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd<0)
	{
		cout<<"socket error"<<endl;
		exit(1);
	}

	//binding 
	struct sockaddr_in address;
	const int PORT=2133;
	memset(&address,0,sizeof(address));
	address.sin_family=AF_INET;
	address.sin_port=htons(PORT);
	address.sin_addr.s_addr=htonl(INADDR_ANY);

	//scan the ports if 2133 was used
	if(bind(listenfd,reinterpret_cast<struct sockaddr*>(&address),sizeof(address))<0)
	{
		cout<<"[Binding]bind error the first time"<<endl;
		int result = seekports(listenfd);
		if(result>0)
			{
			address.sin_port=htons(result);
			cout<<"[Binding]New PORT "<<result<<endl;
			cout<<"[Binding]MainPage 0.0.0.0:"<<result<<"/index.html"<<endl;
			}
		else
			{
			cout<<"[Binding]Bind Failed!"<<endl;
			exit(1);
			}
		
	}
	else
	{
	cout<<"[Binding]bind success, MainPage 0.0.0.0:2133/index.html"<<endl;
	}
	//listen
	if(listen(listenfd,5)<0)//max queue length 5
	{
		cout<<"listen error"<<endl;
		exit(1);
	}
	//processpool
	processpool*pool=processpool::get(listenfd);
	if(pool){
		pool->run();
		delete pool;
	}
	close(listenfd);
	return 0;
}

int seekports(int listenfd)
{	
	struct sockaddr_in address;
	int PORT=1000;
	memset(&address,0,sizeof(address));
	address.sin_family=AF_INET;
	address.sin_port=htons(PORT);
	address.sin_addr.s_addr=htonl(INADDR_ANY);
	do
	{
	PORT++;
	cout<<"[Binding]Trying PORT "<<PORT<<endl;
	if(PORT>1999)
		break;
		
	memset(&address,0,sizeof(address));
	address.sin_family=AF_INET;
	address.sin_port=htons(PORT);
	address.sin_addr.s_addr=htonl(INADDR_ANY);	
	}
	while(bind(listenfd,reinterpret_cast<struct sockaddr*>(&address),sizeof(address))<0);
	if(PORT>1999)
		return -1;
	else
		return PORT;

}

