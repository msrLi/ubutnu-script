#include <stdio.h>
#include <string.h>

int  main (int argc,char argv[])
{
	char buffer[4 * 8];
	int i = 0;
	FILE *pFile = fopen("Makefile", "r");

	memset(buffer, 0 ,sizeof(buffer));
	
	md5_stream(pFile, buffer);
	printf("md5summ ="); 
	for(i=0;i<4 * 8;i++) {
		printf("%x",buffer[i]);
	}
	printf("\n");
	printf("%s %d\n",__func__,__LINE__);
	return 0;
}
