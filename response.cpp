#include<cstring>
#include"response.h"
response::response(){
	body=new char[4096];
	for(int i=0;i<4096;i++)
	{
		body[i]='\0';
	}
}
response::~response(){
	delete[]body;
}
