
#include <iostream>
#include <string>
#include <CryptoAes.h>
#include "md5.h"
#include "sha.h"
extern "C" {
#include <stdio.h>
#include <string.h>
#include "aes.h"
}

using namespace std;

CryptoAes::CryptoAes()
{

}

CryptoAes::~CryptoAes()
{

}

int CryptoAes::encryptStream(const string& origFile,const string& destFile,const unsigned char* key16)
{
	FILE *pOrigStream = NULL;
	FILE *pDestStream = NULL;
	size_t origFileLength = 0;
	size_t destFileLength = 0;
	char *pOrigBuffer = NULL;
	char *pDestBuffer = NULL;
	aes_data_head_t *pAesExternHeader = NULL;
	char *pFileStart = NULL;
	AES_KEY aesKs;
	int ret = 0;
	int i = 0;

	pOrigStream = fopen(origFile.c_str(),"r");
	if (NULL == pOrigStream) {
		cout<<"open file="<<origFile<<"failed\n"<<endl;
		ret = -1;
		goto err;
	}
	
	/* get file lenght */
	fseek(pOrigStream, 0 , SEEK_END);
	origFileLength = ftell(pOrigStream);
	rewind(pOrigStream);
	
	/* malloc a buffer to storage origFile data */
	pOrigBuffer = new char[origFileLength + sizeof(aes_data_head_t)];
	if(NULL== pOrigBuffer) {
		ret = -2;
		cout<<"out of memory"<<endl;
		goto err;
	}
	memset(pOrigBuffer, 0 ,sizeof(pOrigBuffer));

	pAesExternHeader = reinterpret_cast<aes_data_head_t *>(pOrigBuffer);
	pFileStart = pOrigBuffer +  sizeof(aes_data_head_t);
	/* storage file lenght to data header */
	pAesExternHeader->date_length = origFileLength;

	/* read file data */
	ret = fread(pFileStart, 1, origFileLength, pOrigStream);
	if(ret != origFileLength) {
		ret = -3;
		cout << "read file cout failed! origFileLenght="<<origFileLength<<"readFileLenght="<<ret<<endl;
		goto err;
	}
	/* calculate checksum */
	md5_buffer(pFileStart, pAesExternHeader->date_length, pAesExternHeader->checksum);
#if 0
	for(i=0;i<4;i++) {
		printf("%08x",pAesExternHeader->checksum[i]);
	}
	printf("\n");
#endif 
	// printf("md5 = %s\n",md5_stream(pOrigStream, pAesExternHeader->checksum));
	 /* let's 16-byte alignment */
	destFileLength = origFileLength + sizeof(aes_data_head_t);
	destFileLength = (destFileLength & 0xF) ? ((destFileLength+0x10) & (~0xF)):destFileLength;
	
	/* malloc buffer storage enc data */
	pDestBuffer = new char[destFileLength];
	if(NULL == pDestBuffer) {
		ret = -4;	
		cout << "out of memory"<<endl;
		goto err;
	}
	
	/* make keys */
	private_AES_set_encrypt_key(key16, 128, &aesKs);
	/* encry files */
	memset(iv,0,sizeof(iv));
	AES_cbc_encrypt((const unsigned char*) pOrigBuffer, (unsigned char *) pDestBuffer, (origFileLength + sizeof(aes_data_head_t)), &aesKs, iv, 1);
	
	/* open or create new file to storage encry data */
	pDestStream = fopen(destFile.c_str(), "w+");
	if (NULL == pDestStream) {
		cout<<"open file="<<destFile<<"failed\n"<<endl;
		ret = -5;
		goto err;
	}
	/* write data to the opend file */
	fwrite(pDestBuffer, destFileLength, 1, pDestStream);
	cout<<"encry file ok"<<endl;
		
err:
	if(pOrigStream) {
		fclose(pOrigStream);	
	}
	if(pDestStream) {
		fclose(pDestStream);
	}
	if(pOrigBuffer){
		delete(pOrigBuffer);
	}
	if(pDestBuffer) {
		delete(pDestBuffer);
	}
}

int CryptoAes::decryptStream(const string& origFile,const string& destFile,const unsigned char* key16)
{
	FILE *pOrigStream = NULL;
	FILE *pDestStream = NULL;
	size_t origFileLength = 0;
	size_t destFileLength = 0;
	char *pOrigBuffer = NULL;
	char *pDestBuffer = NULL;
	aes_data_head_t *pAesExternHeader = NULL;
	char *pFileStart = NULL;
	AES_KEY aesKs;
	int ret = 0;
	int i = 0;
	pOrigStream = fopen(origFile.c_str(),"r");
	if (NULL == pOrigStream) {
		cout<<"open file="<<origFile<<"failed\n"<<endl;
		ret = -1;
		goto err;
	}
	
	/* get file lenght */
	fseek(pOrigStream, 0 , SEEK_END);
	origFileLength = ftell(pOrigStream);
	rewind(pOrigStream);
	
	/* malloc a buffer to storage origFile data */
	pOrigBuffer = new char[origFileLength];
	if(NULL== pOrigBuffer) {
		ret = -2;
		cout<<"out of memory"<<endl;
		goto err;
	}
	memset(pOrigBuffer, 0 ,sizeof(pOrigBuffer));

	/* read file data */
	ret = fread(pOrigBuffer, 1, origFileLength, pOrigStream);
	if(ret != origFileLength) {
		ret = -3;
		cout << "read file cout failed! origFileLenght="<<origFileLength<<"readFileLenght="<<ret<<endl;
		goto err;
	}
	
	/* malloc buffer storage enc data */
	pDestBuffer = new char[origFileLength];
	if(NULL == pDestBuffer) {
		ret = -4;	
		cout << "out of memory"<<endl;
		goto err;
	}
	
	/* make keys */
	private_AES_set_decrypt_key(key16, 128, &aesKs);
	/* encry files */
	memset(iv,0,sizeof(iv));
	AES_cbc_encrypt((const unsigned char *)pOrigBuffer,(unsigned char *) pDestBuffer, origFileLength, &aesKs, iv, 0);

	pAesExternHeader = reinterpret_cast<aes_data_head_t *>(pDestBuffer);
	destFileLength = pAesExternHeader->date_length;	
	pFileStart = pDestBuffer + sizeof(aes_data_head_t);
	
	/* storage orig checksum */
	memcpy(mOrigChecsum, pAesExternHeader->checksum, sizeof(mOrigChecsum));

	/* calculate checksum */
	memset(mCalculateChecksum, 0 ,sizeof(mCalculateChecksum));
	md5_buffer(pFileStart, destFileLength, mCalculateChecksum);
	/* open or create new file to storage encry data */
	pDestStream = fopen(destFile.c_str(), "w+");
	if (NULL == pDestStream) {
		cout<<"open file="<<destFile<<"failed\n"<<endl;
		ret = -5;
		goto err;
	}
	/* write data to the opend file */
	fwrite(pFileStart, destFileLength, 1, pDestStream);
	cout<<"decry file ok"<<endl;
	ret = 0;
		
err:
	if(pOrigStream) {
		fclose(pOrigStream);	
	}
	if(pDestStream) {
		fclose(pDestStream);
	}
	if(pOrigBuffer){
		delete(pOrigBuffer);
	}
	if(pDestBuffer) {
		delete(pDestBuffer);
	}
	return ret;
}

int CryptoAes::decryptStream(const std::string& origFile,const std::string& destFile,const unsigned char* key16, 
                          unsigned int (&origChecksum)[4], unsigned int (&calculateChecksum)[4] )
{
	decryptStream(origFile, destFile, key16);
	memcpy(origChecksum, mOrigChecsum, sizeof(mOrigChecsum));	
	memcpy(calculateChecksum, mCalculateChecksum, sizeof(mCalculateChecksum));
	return 0;
}
