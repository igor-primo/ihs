#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<inttypes.h>

FILE *in;
int16_t v64[16] = {0};
uint32_t ic[16] = {0}; // instruction counter
uint8_t code[128] = {0};

void interpret(uint8_t *code, uint32_t n);
void see_v64();

int main(int argc, char **argv){
    if(!(argc == 2))
        return 1;
    in = fopen(argv[1], "r");
    if(in == NULL)
        return 12;
    uint32_t byte = 0, n = 0;
    while(fscanf(in, "%X", &byte) == 1)
        code[n++] = byte;
    interpret(code, n);
    see_v64();
    fclose(in);
    return 0;
}

void interpret(uint8_t *ccode, uint32_t n){
	uint8_t eq = 0, gt = 0, lt = 0;
    int16_t neg = 0;
    int8_t i = 0;
    int8_t rx = 0, ry = 0;
	uint32_t index = 0;
    while(index < n){
        switch(code[index]){
            case 0x00:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                index++;
                v64[rx] = ((int16_t) code[index+1] << 8) | code[index];
                index += 2;
                ic[0x00]++;
                break;
            case 0x01:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                ry = 0b00001111 & i;
                v64[rx] = v64[ry];
                index += 3;
                ic[0x01]++;
                break;
            case 0x02:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                ry = 0b00001111 & i;
                v64[rx] = code[v64[ry]]; // ???
                index += 3;
                ic[0x02]++;
                break;
            case 0x03:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                ry = 0b00001111 & i;
                code[v64[rx]] = v64[ry];
                index += 3;
                ic[0x03]++;
                break;
            case 0x04:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                ry = 0b00001111 & i;

                eq = v64[rx] == v64[ry];
                gt = v64[rx] > v64[ry];
                lt = v64[rx] < v64[ry];
                index += 3;
                ic[0x04]++;
                break;
            case 0x05:
                index += 4 + (((int16_t) code[index+3] << 8) | code[index+2]);
                ic[0x05]++;
                break;
            case 0x06:
                if(gt) index += 4 + (((int16_t) code[index+3] << 8) | code[index+2]);
                ic[0x06]++;
                break;
            case 0x07:
                if(lt) index += 4 + (((int16_t) code[index+3] << 8) | code[index+2]);
                ic[0x07]++;
                break;
            case 0x08:
                if(eq) index += 4 + (((int16_t) code[index+3] << 8) | code[index+2]);
                ic[0x08]++;
                break;
            case 0x09:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                ry = 0b00001111 & i;
                v64[rx] += v64[ry];
                index += 3;
                ic[0x09]++;
                break;
            case 0x0A:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                ry = 0b00001111 & i;
                //v64[rx] -= v64[ry];
                neg = ~v64[ry];
                neg++;
                v64[rx] += neg;
                index += 3;
                ic[0x0A]++;
                break;
            case 0x0B:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                ry = 0b00001111 & i;
                v64[rx] &= v64[ry];
                index += 3;
                ic[0x0B]++;
                break;
            case 0x0C:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                ry = 0b00001111 & i;
                v64[rx] |= v64[ry];
                index += 3;
                ic[0x0C]++;
                break;
            case 0x0D:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                ry = 0b00001111 & i;
                v64[rx] ^= v64[ry];
                index += 3;
                ic[0x0D]++;
                break;
            case 0x0E:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                index += 2;
                v64[rx] <<= (0b00011111 & code[index]);
                index++;
                ic[0x0E]++;
                break;
            case 0x0F:
                index++;
                i = code[index];
                rx = (0b11110000 & i) >> 4;
                index += 2;
                v64[rx] >>= (0b00011111 & code[index]);
                index++;
                ic[0x0F]++;
                break;
            default:
                printf("I have seen shit worst than this.\n");
                break;
        }
    }
    return;
}

void see_v64(){
    for(int i=0;i<16;i++)
        printf("0%x:%"SCNu32"\n", i, ic[i]);
    for(int i=0;i<16;i++)
        printf("R[%d]=0x%08x\n", i, (uint32_t) v64[i]);
}
