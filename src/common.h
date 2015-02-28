#ifndef  _COMMON_H_
#define  _COMMON_H_

#define  DEBUG    0

#if  DEBUG
	#define D(...)  printf(__VA_ARGS__)
#else
	#define D(...)   ;
#endif


char teakey_backup_data[16]={
        0xdd,0xa3,0x3d,0x52,0x9c,0x30,0x35,0x19,
    	0x52,0x25,0xaa,0x63,0x82,0x3d,0xe1,0x28
};






void tea_encrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
    uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i < 32; i++) {                       /* basic cycle start */
        sum += delta;
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;
}
 
void tea_decrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i;  /* set up */
    uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i<32; i++) {                         /* basic cycle start */
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        sum -= delta;
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;
}


void encryptData(char *p,int len)
{
	
	int  i;
	
    for (i = 0; i < len/8; i ++) {
        tea_encrypt ((uint32_t *)p, (uint32_t *)teakey_backup_data);
        p += 8;
    }
}

void decryptData(char *p,int len)
{
	
	int  i;
	
    for (i = 0; i < len/8; i ++) {
        tea_decrypt ((uint32_t *)p, (uint32_t *)teakey_backup_data);
        p += 8;
    }
}



#define SIZE 4096
char * read_script2(char * file)
{
	FILE * i;
	char * text;
	int cnt, l;

	text = malloc(SIZE);
	if (!text)
		return NULL;
	i = fopen(file, "rb");
	if (!i)
		return NULL;
	for (l = 0;;) {
		text = realloc(text, l + SIZE);
		if (!text)
			return NULL;
		cnt = fread(&text[l], 1, SIZE, i);
		if (!cnt)
			break;
		l += cnt;
	}
	fclose(i);
	text = realloc(text, l + 1);
	if (!text)
		return NULL;
	text[l] = '\0';

	/* Check current System ARG_MAX limit. */
	if (l > 0.80 * (cnt = sysconf(_SC_ARG_MAX))) {
		fprintf(stderr, " WARNING!!\n"
"   Scripts of length near to (or higher than) the current System limit on\n"
"   \"maximum size of arguments to EXEC\", could comprise its binary execution.\n"
"   In the current System the call sysconf(_SC_ARG_MAX) returns %d bytes\n"
"   and your script \"%s\" is %d bytes length.\n",
		 cnt, file, l);
	}
	
	return text;
}


char * read_script(char * file)
{
	FILE * i;
	char * text;
	int cnt, l;
	int  fileSize=-1;

	
	i = fopen(file, "rb");
	if (!i)
		return NULL;
	if(fseek(i, 0, SEEK_END))
		return NULL;

	fileSize=ftell(i);
	if(fileSize<0)
	{
		perror("ftell");
		return NULL;
	}

	if(fseek(i, 0, SEEK_SET))
		return NULL;
	
	text=malloc(fileSize+1);
	if(text==NULL)
	{
		return NULL;
	}

	memset(text,0,fileSize+1);
	
	for (l = 0;;) {
		cnt = fread(&text[l], 1, SIZE, i);
		if (!cnt)
			break;
		l += cnt;
	}
	fclose(i);
	

	/* Check current System ARG_MAX limit. */
	if (l > 0.80 * (cnt = sysconf(_SC_ARG_MAX))) {
		fprintf(stderr, " WARNING!!\n"
"   Scripts of length near to (or higher than) the current System limit on\n"
"   \"maximum size of arguments to EXEC\", could comprise its binary execution.\n"
"   In the current System the call sysconf(_SC_ARG_MAX) returns %d bytes\n"
"   and your script \"%s\" is %d bytes length.\n",
		 cnt, file, l);
	}
	
	return text;
}





int  creatHeadFile(char *p,char *arrayname,int len,char *file,int addFlag)
{
	
	int ret;
	int  i;

	
	

	char * openFlag=NULL;
	if(addFlag)
		openFlag="a+";
	else
		openFlag="wb";
		

	FILE* fpnew = fopen (file, openFlag);
	fprintf(fpnew,"char  %s[%d]=\n",arrayname,len);
	fprintf(fpnew,"{\n\t");
	for(i=0;i<len-1;i++)
	{
		fprintf(fpnew,"0x%02x,",(unsigned char)p[i]);
		if((i+1)%16 ==0)
		{
			fprintf(fpnew,"\n\t");
		}
	}
	fprintf(fpnew,"0x%02x\n",(unsigned char)p[i]);
	fprintf(fpnew,"};\n");
	fclose(fpnew);
	return  0;
}


#endif
