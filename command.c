#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include "command.h"
#include "coder.h"

int encode_file(const char *in_file_name, const char *out_file_name){
	CodeUnits unit;
    CodeUnits *unitpoint = &unit;
    uint32_t val;

	FILE *in_file;
	if((in_file = fopen(in_file_name, "r")) == NULL)
		return -1;
	
	FILE *out_file;
	if((out_file = fopen(out_file_name, "wb")) == NULL)
		return -1;

	while(fscanf(in_file, "%" SCNx32, &val) == 1){
        encode(val, unitpoint);
        write_code_unit(out_file, unitpoint);
    }
    
    fclose(in_file);
    fclose(out_file);
	return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name){
    CodeUnits unit;
    CodeUnits *unitpoint = &unit;

    FILE *in_file = fopen(in_file_name, "rb");
    if(in_file == NULL)
        return -1;

    FILE *out_file = fopen(out_file_name, "w");
    if(out_file == NULL){
        return -1;
    }

    while(1){ 
        read_next_code_unit(in_file, unitpoint);
        if(feof(in_file))
           break;
        fprintf(out_file, "%" PRIx32"\n", decode(unitpoint));
    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}