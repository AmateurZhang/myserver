#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include"module.h"
#include"response.h"
#include"chartohtml.h"
using namespace std;
module*process_module;
extern "C" void getprocess(response*res)
{	const int filelength=4096;
	FILE*fd=popen("ps -u","r");
	if(fd==NULL)
		{
		cout<<"[getprocess]Can not build new file"<<endl;
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
	process_module=new module("process",getprocess);
	return process_module;
}
