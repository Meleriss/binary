#ifndef CODER_H
#define CODER_H

#include <stdlib.h>
#include <string.h>

enum {
    MaxCodeLength = 4
};

typedef struct {
    uint8_t code[MaxCodeLength];
    size_t lenght;
} CodeUnits;

int encode(uint32_t code_point, CodeUnits *code_units); 

uint32_t decode(const CodeUnits *code_unit); 

int read_next_code_unit(FILE *in, CodeUnits *code_units); 

int write_code_unit(FILE *out, const CodeUnits *code_unit); 

#endif