#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "command.h"
//#include "coder.h"

int main(int argc, char *argv[])
{
	if(argc != 4){
		printf("\tUsage:\n\tcoder encoder <in-file-name> <out-file-name>\n\tcoder decoder <in-file-name> <out-file-name>\n");
		return 1;
	}
	if(strcmp(argv[1], "decode") == 0){
		printf("\tДекодирование\n");
		decode_file(argv[3], argv[2]); 
	}else{
		if(strcmp(argv[1], "encode") == 0) {
			printf("\tКодирование\n");
			encode_file(argv[2], argv[3]); 
		}else{
			printf("\tUsage:\n\tcoder encoder <in-file-name> <out-file-name>\n\tcoder decoder <in-file-name> <out-file-name>\n");
			return 1;
		}
	}
    return 0;
}