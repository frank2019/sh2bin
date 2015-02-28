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






void usage(void)
{
	printf("jnit_sh2bin \n\n\n");
	D("sh2bin  version 0.1   by frank2019@yeah.net \n");
}



int  main(int argc,char  **argv)
{
	if(argc !=1)
	{
		usage();
		return 1;
	}

	char * common="common.h";
	char * scripts="main_scripts.c";
	char * makefile="Makefile";
	char * Android_mk="Android.mk";

	char * commonData=NULL;
	char * scriptsData=NULL;
	char * makefileData=NULL;
	char * Android_mkData=NULL;

	char * commonArrayName="common";
	char * scriptsArrayName="main_script";
	char * makefileArrayName="Makefile";
	char * Android_mkArrayName="Android_mk";

	
	if(access(common,F_OK)!=0)
	{
		printf("Error: %s is  not exists!\n",common);
		return -2;
	}
	if(access(scripts,F_OK)!=0)
	{
		printf("Error: %s is  not exists!\n",scripts);
		return -3;
	}
	if(access(makefile,F_OK)!=0)
	{
		printf("Error: %s is  not exists!\n",makefile);
		return -4;
	}
	if(access(Android_mk,F_OK)!=0)
	{
		printf("Error: %s is  not exists!\n",Android_mk);
		return -4;
	}
	

	commonData=read_script(common);
	if(commonData==NULL)
	{
		printf("Error:read_script  %s !\n",common);
		return -5;
	}
	scriptsData=read_script(scripts);
	if(scriptsData==NULL)
	{
		printf("Error:read_script  %s !\n",scripts);
		return -6;
	}
	makefileData=read_script(makefile);
	if(makefileData==NULL)
	{
		printf("Error:read_script  %s !\n",makefile);
		return -7;
	}

	Android_mkData=read_script(Android_mk);
	if(Android_mkData==NULL)
	{
		printf("Error:read_script  %s !\n",Android_mk);
		return -7;
	}



	creatHeadFile(commonData, commonArrayName, strlen(commonData), "data.h", 0);
	creatHeadFile(scriptsData, scriptsArrayName, strlen(scriptsData), "data.h", 1);
	creatHeadFile(makefileData, makefileArrayName, strlen(makefileData), "data.h", 1);
	creatHeadFile(Android_mkData, Android_mkArrayName, strlen(Android_mkData), "data.h", 1);


	return 0;
}
