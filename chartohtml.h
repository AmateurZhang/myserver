#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
int ChartoHTML(char* c,char* text,int filelength)
{	
	
	strcat(text,"<html>\r\n<body>\r\n");	
	strcat(text,"<table><tr><td>");
	for(int i=0;i<filelength-1;i++)
	{
		if(c[i]==32)
		{
			strcat(text,"</td><td>");
			while(c[i]==32&&c[i+1]==32)
				{
					i++;
				}
		}
		else if(c[i]=='\n'&&c[i+1]!='\0')
		{
			strcat(text,"</td></tr><tr><td>");
		}
		else if(c[i]=='\n'&&c[i+1]=='\0')
		{
			strcat(text,"</td></tr></table>");
		}
		else
		{
			int j=0;
			while(text[j]!='\0')
			{j++;}
			text[j]=c[i];
		}
	}
	strcat(text,"</body>\r\n</html>\r\n");
	return 0;
}
