
 /* Copyright (c) 2014-2018 Frank Xia  
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.*/

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
#include "data.h"



int writeFile(char *filename,char *buf,int  len)
{
	FILE* fpnew = fopen (filename, "wb");
	int res=-1;

	res= fwrite(buf, len, 1, fpnew);
	if(res<0)
	{
		perror("write \n");
		return -2;
	}

	fclose(fpnew);
}



void usage(void)
{
	printf("sh2bin  [ -p  android/linux ] -i  inputSh   [ -o outputBin]\n\n\n");
	printf("sh2bin  version 0.1\n");
	D("by frank2019@yeah.net \n\n");
}



enum PlatformType
{
	LinuxOS=1,
	AndroidOS=2
};


int  makebin(char * platform,char *name)
{
	
	D("Begin @makebin\n");
	if(strcmp(platform,"linux")==0)
	{
		char *cmd="make";
		if(chdir(".sh2bin")!=0)
		{
			perror("chdir\n");
			system("rm -rf .sh2bin");
			return  -2;
		}
		system(cmd);
		if(access("edit",F_OK)==0)
		{
			if(chdir("..")!=0)
			{
				perror("chdir\n");
				system("rm -rf *");
				return  -2;
			}
			
			if(rename(".sh2bin/edit",name)==0)
			{
				system("rm -rf .sh2bin");
				return 0;
			}
			else
			{
				perror("Error rename");
				system("rm -rf .sh2bin");
				return  -1;
			}
		}
		else 
			return  1;
	}
	else if(strcmp(platform,"android")==0)
	{
		
		char *cmd="ndk-build";
		if(chdir(".sh2bin")!=0)
		{
			perror("chdir\n");
			system("rm -rf .sh2bin");
			return  -2;
		}
		system("mkdir -p jni");
		system("mv  *.h  *.c   jni");
		system("mv  *.mk   jni");
		
		
		system(cmd);
		if(chdir("..")!=0)
		{
			perror("chdir\n");
			system("rm -rf *");
			return  -2;
		}
		if(access(".sh2bin/libs/armeabi/edit",F_OK)==0)
		{
			
			if(rename(".sh2bin/libs/armeabi/edit",name)==0)
			{
				system("rm -rf .sh2bin");
				return 0;
			}
			else
			{
				perror("rename\n");
				system("rm -rf .sh2bin");
				return  -1;
			}
		}
		else 
		{
			printf("Error compile  \n");
			system("rm -rf .sh2bin");
			return  1;
		}
	}
	D("Do not support  @makebin\n");
	return  -2;
	
}

 
 int main(int argc,char **argv)
 {
 	int oc;                     /*Ñ¡Ïî×Ö·û */
    int ret=-1;

	char *platform=NULL;
	char *inputSh=NULL;
	char *outputBin=NULL;

	char *defaultOS="linux";
	char *defaultOutputBin="a.out";

	char * shText=NULL;

	

	

    while((oc = getopt(argc, argv, "p:i:o:")) != -1)
    {
        switch(oc)
        {
            case 'p':
                platform=optarg;
                break;
            case 'i':
                inputSh=optarg;
                break;
            case 'o':
                outputBin = optarg;
                break;
			default :
				usage();
				break;
        }
    }

	if(inputSh ==NULL)
	{
		usage();
		return  -1;
	}
	if(platform ==NULL)
	{
		platform=defaultOS;
	}
	else if(strncmp(platform,"linux",sizeof("linux")) !=0 
		&& strncmp(platform,"android",sizeof("android"))!=0)
	{
		printf("Error: do not support the  expect platform:%s\n",platform);
		return -2;
	}
	
	if(outputBin ==NULL)
	{
		outputBin=defaultOutputBin;
	}

	D("platform=%s,inputSh=%s,outputBin=%s\n",platform,inputSh,outputBin);
	//return  0;
	

	shText=read_script(inputSh);
	if(shText ==NULL)
	{
		printf("Error read_script!\n");
		return  2;
	}
	system("mkdir -p .sh2bin");

	// encrypt  sh
	encryptData(shText,strlen(shText));
	creatHeadFile(shText,"text", strlen(shText),".sh2bin/text.h",0);
	free(shText);

	if(access(".sh2bin/text.h",F_OK)!=0)
	{
		printf("Error create .sh2bin/text.h\n");
		return  -1;
	}
	

	
	ret=writeFile(".sh2bin/common.h",common,sizeof(common));
	if(ret<0)
	{
		printf("Error: writeFile .sh2bin/common.h\n");
		return -3;
	}

	ret=writeFile(".sh2bin/main_scripts.c",main_script,sizeof(main_script));
	if(ret<0)
	{
		printf("Error: writeFile .sh2bin/main_scripts.c\n");
		return -3;
	}

	ret=writeFile(".sh2bin/Makefile",Makefile,sizeof(Makefile));
	if(ret<0)
	{
		printf("Error: writeFile .sh2bin/Makefile\n");
		return -3;
	}

	ret=writeFile(".sh2bin/Android.mk",Android_mk,sizeof(Android_mk));
	if(ret<0)
	{
		printf("Error: writeFile .sh2bin/Android_mk\n");
		return -3;
	}
	
	
	return makebin(platform,outputBin);
	

	
   
 }

