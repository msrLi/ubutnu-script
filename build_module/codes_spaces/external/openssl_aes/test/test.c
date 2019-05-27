#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include "aes.h"
#include "aes_extern_data.h"


int main(int argc, char * argv[])
{
	FILE *pOrig = fopen("Makefile","r");
	long oringFileSize = 0; 
	long encfileSize = 0;
	char *bufferOring = NULL;
	FILE *pEncFile = fopen("EntryFile","w+");
	char *bufferEnc = NULL;
	FILE *pDecFIle = fopen("Decfile","w+");
    aes_data_head_t *pAesExtern = NULL;
	unsigned char iv[AES_BLOCK_SIZE];
	size_t ret =0;
	static const unsigned char key16[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12
	};
	AES_KEY aesKs;
    aes_data_head_t *pAesExternData = NULL;
	if(pOrig == NULL || NULL == pEncFile || NULL == pDecFIle) {
		printf("File open failed %p %p %p \n", pOrig, pEncFile, pDecFIle);
		goto err;
	}
    printf("LHB %s %d\n",__func__,__LINE__);

	memset(iv, 0, sizeof(iv));

	/* read file lenght */
	fseek (pOrig, 0 , SEEK_END);  
	oringFileSize = ftell(pOrig);  
	rewind (pOrig);
    printf("LHB %s %d\n",__func__,__LINE__);
	/* malloc buffer of oring */
	bufferOring = (char *) malloc(sizeof(char) * oringFileSize + sizeof(aes_data_head_t) );
	if(NULL == bufferOring) {
		printf("out of memory\n");
		goto err;
	}
    printf("LHB %s %d\n",__func__,__LINE__);
    pAesExternData = (aes_data_head_t *) bufferOring;
    printf("LHB %s %d\n",__func__,__LINE__);
    pAesExternData->date_length = oringFileSize;
    printf("LHB %s %d\n",__func__,__LINE__);
    oringFileSize += sizeof(aes_data_head_t);

    printf("LHB %s %d\n",__func__,__LINE__);
	ret = fread(bufferOring+sizeof(aes_data_head_t), 1, pAesExternData->date_length, pOrig);
	if(ret != pAesExternData->date_length) {
		printf("read file %d oring file size %d\n",ret, pAesExtern->date_length);
		goto err;
	}
    printf("LHB %s %d\n",__func__,__LINE__);
	/* let's 16-byte alignment */
	encfileSize = oringFileSize;
	encfileSize = (encfileSize & 0xF) ? ((encfileSize+0x10) & (~0xF)):encfileSize;

	bufferEnc = (char *) malloc(sizeof(char) * encfileSize );
	if(NULL == bufferEnc) {
		printf("malloc out of memory\n");
		goto err;
	}	
    printf("LHB %s %d\n",__func__,__LINE__);

	/* Make keys */
	private_AES_set_encrypt_key(key16, 128, &aesKs);
	/* encry file */
	AES_cbc_encrypt(bufferOring, bufferEnc, oringFileSize, &aesKs, iv,1);
	fwrite(bufferEnc, oringFileSize, 1, pEncFile);

    printf("LHB %s %d\n",__func__,__LINE__);
	free(bufferOring);
	oringFileSize = encfileSize;
	bufferOring = (char *) malloc(sizeof(char) * oringFileSize );
	memset(bufferOring, 0, sizeof(bufferOring));
	memset(iv, 0,sizeof(iv));
	memset(&aesKs, 0, sizeof(aesKs));
	private_AES_set_decrypt_key(key16, 128, &aesKs);
	AES_cbc_encrypt(bufferEnc, bufferOring, oringFileSize, &aesKs, iv,0);
    pAesExternData = (aes_data_head_t *) bufferOring;

    printf("LHB %s %d\n",__func__,__LINE__);
	fwrite(bufferOring + sizeof(aes_data_head_t), pAesExternData->date_length, 1, pDecFIle);

	printf("runing  ok\n");
	ret = 0;
err:
	if (pOrig) {
		fclose(pOrig);
	}
	
	if(bufferOring) {
		free(bufferOring);
	}

	if(pEncFile) {
		fclose(pEncFile);
	}

	if(pDecFIle) {
		fclose(pDecFIle);
	}

	if(bufferEnc) {
		free(bufferEnc);
	}

	return ret;
}

