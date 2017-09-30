#include<cstdio>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include"module.h"
#include"response.h"
#include"chartohtml.h"
using namespace std;
module*time_module;
extern "C" void gettime(response*res)
{	const int filelength=200;
	FILE*fd=popen("uname -v","r");
	if(fd==NULL)
		{
		cout<<"[getdisk]Can not build new file"<<endl;
		exit(1);
		}

	char c[filelength];
	for(int i=0;i<filelength;i++)
	{
		c[i]='\0';
	}
	
	fread(c,filelength,1,fd);

	char text[4096];
	for(int i=0;i<4096;i++)
	     text[i]='\0';
	
	ChartoHTML(c,text,filelength);
	
	strcat(res->body,text);
	pclose(fd);
}
extern "C" module*hook(){
	time_module=new module("time",gettime);
	return time_module;
}
