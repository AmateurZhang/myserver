#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include"module.h"
#include"response.h"
#include"chartohtml.h"
using namespace std;
module*disk_module;
extern "C" void getdisk(response*res)
{
	const int filelength=600;
	FILE*fd=popen("df -lh","r");
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
	disk_module=new module("disk",getdisk);
	return disk_module;
}
