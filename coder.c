#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "coder.h"


int encode(uint32_t code_point, CodeUnits *code_units){
    uint32_t val = code_point;
    for(int i = 0; i < 4; i++)                          //Обнуляем массив code
        code_units->code[i] = 0;

    if(val >= 0 && val < (1<<7)){             // 1 Байт
        code_units->lenght = 1;
        code_units->code[0] = val;

    }else if(val >= (1<<7) && val < (1<<11)){ // 2 Байта
        code_units->lenght = 2;
        code_units->code[1] = (val & ~(1<<6))|(1<<7);
        code_units->code[0] = ((val >> 6) & ~(1<<5))|(3<<6);// 3 = 11

    }else if(val >= (1<<11) && val < (1<<16)){//3 Байта
        code_units->lenght = 3;
        code_units->code[2] = (val & ~(1<<6))|(1<<7);
        code_units->code[1] = ((val >> 6) & ~(1<<6))|(1<<7);
        code_units->code[0] = ((val >> 6) & ~(1<<4))|(7<<5);

    }else if(val >= (1<<16) && val < (1<<21)){                //4 Байта
        code_units->lenght = 4; 
        code_units->code[3] = (val & ~(1<<6))|(1<<7);
        code_units->code[2] = ((val >> 6) & ~(1<<6))|(1<<7);
        code_units->code[1] = ((val >> 6) & ~(1<<6))|(1<<7);
        code_units->code[0] = ((val >> 6) & ~(1<<3))|(15<<4);
    }else
        return -1;
    return 0;
}

uint32_t decode(const CodeUnits *code_unit){
    uint32_t decode_number = 0;
    if(code_unit->lenght == 1){
        decode_number = code_unit->code[0];
    }else if(code_unit->lenght == 2){
        decode_number = (code_unit->code[0] & 31) << 6;            // 31 = 00011111
        decode_number = decode_number | (code_unit->code[1] & 63); // 63 = 00111111

    }else if(code_unit->lenght == 3){
        decode_number = (code_unit->code[0] & 15) << 6;             // 15 = 00001111
        decode_number = (decode_number | (code_unit->code[1] & 63)) << 6;
        decode_number = decode_number | (code_unit->code[2] & 63);

    }else if(code_unit->lenght == 4){
        decode_number = (code_unit->code[0] & 7) << 6;              //  7 = 00000111
        decode_number = (decode_number | (code_unit->code[1] & 63)) << 6;
        decode_number = (decode_number | (code_unit->code[2] & 63)) << 6;
        decode_number = decode_number | (code_unit->code[3] & 63);
    }
    return decode_number;
} 

int write_code_unit(FILE *out, const CodeUnits *code_unit){
    for(int i = 0; i < code_unit->lenght; i++){
        fwrite(&code_unit->code[i], sizeof(uint8_t), 1, out);
    }
    return 0;
}

int read_next_code_unit(FILE *in, CodeUnits *code_unit){
    uint8_t unit_read = 1;  //0 - байты считаны и корректны, 1 - не считаны, x - последнее значение
    size_t lenght_code;

    while(unit_read != 0){              //пока байты не считаны
        while (!feof(in)){
            if(unit_read == 1){                         // Поиск лидирующего байта
                fread(&code_unit->code[0], sizeof(uint8_t), 1, in);   // Считать 1 байт
            }else{
                code_unit->code[0] = unit_read;
                unit_read = 1;
            }
                if ((code_unit->code[0] >> 3) == 30)        // 30 = 11110
                    lenght_code = 4;
                else if ((code_unit->code[0] >> 4) == 14)   // 14 = 1110
                    lenght_code = 3;
                else if ((code_unit->code[0] >> 5) == 6)    // 6 = 110
                    lenght_code = 2;
                else if ((code_unit->code[0] >> 7) == 0)
                    lenght_code = 1;
                else
                    lenght_code = 0;
                if(lenght_code > 0)
                    break;
        }
        if(lenght_code > 0){//если лид. байт найден
            if((lenght_code == 1)){//выйти из цикла,y закодирован 1 байт
                code_unit->lenght = lenght_code;
                return 0;
            }
            for(int i = 1; i < lenght_code; i++){
                fread(&code_unit->code[i], sizeof(uint8_t), 1, in);
                if((code_unit->code[i] >> 6) != 2){
                    unit_read = code_unit->code[i];
                    for(int i = 0; i < 4; i++)
                        code_unit->code[i] = 0;
                    break;
                }
            }
            if(unit_read == 1)
                unit_read = 0;
            code_unit->lenght = lenght_code;
        }else
            break;
    }
    return 0;
}


