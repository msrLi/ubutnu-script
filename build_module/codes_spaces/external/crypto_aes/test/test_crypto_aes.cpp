#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <CryptoAes.h>

using namespace std;

const struct option long_opts[] =
{
	{"encfile", 1, 0, 'e'},
	{"decfile", 1, 0, 'd'},
	{"help", 0, 0,'h'},
	{},
};

int Usarge(char *argv0)
{
	cout << "Usage: " << argv0 << "origfile encryfile  [-e/-d]" << endl;
	cout << "Supported options:" <<endl;
	cout << "\t"<< "-e encrypt file"<<endl;
	cout << "\t" << "-d decrypt file"<<endl;
	return 0;
}
 
int main(int argc,char **argv)
{
	int ret = 0;
	unsigned int origChecsum[4];
	unsigned int destChecsum[4];
	unsigned int flageEncrypt = 0;
	unsigned int flageDecrypt = 0;
	int i = 0;
	CryptoAes aes;
	string origFile;
	string destFile;
	
	static const unsigned char key16[16] = {
                0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
                0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12
        };

	unsigned int c = 0;
	while((c = getopt_long(argc, argv, "edh", long_opts, NULL)) != -1) 	
	{
		switch(c){
			case 'e':
				flageEncrypt = 1;
				break;
			case 'd':
				flageDecrypt = 1;
				break;
			case 'h':
			default:
				Usarge(argv[0]);
				return -1;
		}
	}

	if (optind >= argc || (flageEncrypt == flageDecrypt)) {
		Usarge(argv[0]);
		return -1;
	}

	if (argv[optind] == NULL || NULL == argv[optind+1] ){
		Usarge(argv[0]);
                return -1;	
	}
	origFile = argv[optind];
	destFile = argv[optind+1];
	if(flageEncrypt) {
		aes.encryptStream(origFile, destFile, key16);
	}
	// aes.decryptStream("EncryFile", "DecryFile", key16);
	if(flageDecrypt) {
		aes.decryptStream(origFile, destFile, key16, origChecsum, destChecsum);
		printf("File orig      checsum=");		
		for(i=0;i<4;i++) {
			printf("%08x",origChecsum[i]);
		}
		printf("\n");

		printf("File calculate checsum=");		
		for(i=0;i<4;i++) {
			printf("%08x",destChecsum[i]);
		}
		printf("\n");
	}
	return 0;
}
