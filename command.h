#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "coder.h"


int encode_file(const char *in_file_name, const char *out_file_name); 

int decode_file(const char *in_file_name, const char *out_file_name); 

#endif