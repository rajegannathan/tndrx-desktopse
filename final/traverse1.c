#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<wait.h>
#include<unistd.h>
#include<utmp.h>

#define SIZE 1024

int main()
{
	char *file_nam,*path_nam,*temp,user[50],file[100];
	int pid,array[2],filecount = 0,i,flen,plen,watch;
	struct utmp *u;
	u = (struct utmp*)malloc(sizeof(struct utmp));
	FILE *f;
	f = fopen("/var/run/utmp","r");
	while(fread(u,sizeof(struct utmp),1,f))
	{
		if(!strcmp(u->ut_line,"tty7"))
		{
			strcpy(user,u->ut_user);
		}
	}
	if(!strcmp(user,"root"))
		strcpy(file,"/");
	else
		strcpy(file,"/home/");
	strcat(file,user);
	strcat(file,"/Documents/linear/db/filelist");
	temp = (char*)malloc(SIZE);
	path_nam = (char*)malloc(10);
	file_nam = (char*)malloc(SIZE);
	pipe(array);
	pid = fork();
	if(pid == 0)
	{
		close(array[0]);
		FILE *f1;
		f1 = fopen(file,"r");
		strcpy(path_nam,"\n");
		while(!feof(f1))
		{
			fread(&watch,sizeof(int),1,f1);
			fgets(temp,520,f1);
			file_nam = (temp+1);
			if(temp[0] == '\0')
				break;
			file_nam[strlen(file_nam) - 1] = '\0';
			if((temp[0] & 0XF0))
			{
				if((temp[0] & 0X0F) == 4)
				{
					strcat(path_nam,"/");
					strcat(path_nam,file_nam);
				}
				else if((temp[0] & 0X0F) == 15)
				{
					i = strlen(path_nam) - 2;
					while(path_nam[i] != '/')
						i--;
					path_nam[i] = '\0';
					continue;
				}
				if(strstr(file_nam,"er") != NULL)
				{
					filecount++;
					flen = strlen(file_nam);
	//				file_nam[flen] = '\n';	file_nam[flen+1] = '\0';
					write(array[1],file_nam,strlen(file_nam));
	//printf("%s",file_nam);
					plen = strlen(path_nam);
					path_nam[plen] = '\n';	path_nam[plen+1] = '\0';
					write(array[1],path_nam,strlen(path_nam));
	//printf("%s",path_nam);
					file_nam[flen] = '\0';	path_nam[plen] = '\0';
				}
			}
		}
		printf("%u",filecount);
	}
	else
	{
		printf("Child's pid %u\n",pid);
		int k;
		close(array[1]);
		close(0);
		while(read(array[0],temp,1024))
		{
		}
	}
}
