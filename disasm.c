#include <inttypes.h>
#include <stdio.h>

#define WRONG_AGRUMENTS -1
#define CANNOT_OPEN_FILE -2
#define CANNOT_CLOSE_FILE -3

const char *R_FUNCT_CODES[] = {
    [0x20] = "add", [0x21] = "addu", [0x24] = "and", [0x1A] = "div", [0x1B] = "divu", [0x09] = "jalr", [0x08] = "jr",
    [0x10] = "mfhi", [0x11] = "mthi", [0x12] = "mflo", [0x13] = "mtlo", [0x18] = "mult", [0x19] = "multu", [0x27] = "nor",
    [0x26] = "xor", [0x25] = "or", [0x2A] = "slt", [0x2B] = "sltu", [0x00] = "sll", [0x02] = "srl", [0x03] = "sra",
    [0x22] = "sub", [0x23] = "subu"
};

const char *OPCODES[] = {
    [0x08] = "addi", [0x09] = "addiu", [0x0C] = "andi",[0x04] = "beq", [0x06] = "blez", [0x05] = "bne", [0x07] = "bgtz",
    [0x20] = "lb", [0x24] = "lbu", [0x25] = "lhu", [0x0F] = "lui", [0x23] = "lw", [0x0D] = "ori", [0x28] = "sb",
    [0x29] = "sh", [0x0A] = "slti", [0x0B] = "sltiu", [0x2B] = "sw", [0x02] = "j", [0x03] = "jal"
};

const char *REGISTER_NAMES[] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

/*
int load_file(char * file_name , ){
    FILE * fp = NULL;
    fp = fopen(file_name, "r");

    if(NULL == fp){
        printf("Couldn't open %s.\n", file_name);
        return CANNOT_OPEN_FILE;
    }

    int c = 0;
    while((c = fgetc((fp))) != EOF){
        printf("%c", c);
    }

    if(fclose(fp) != 0){
        printf("Couldn't close %s.\n", file_name);
        return CANNOT_CLOSE_FILE;
    }
 
    return 0;
}
*/

void disassemble(uint32_t instruction){
    

    uint8_t opcode = (instruction >> 26) & 0x3F;
    uint8_t rs,rt;
     
    if(opcode == 0x00){
        rs = (instruction >> 21) & 0x1F;
        rt = (instruction >> 16) & 0x1F;
        uint8_t rd = (instruction >> 11) & 0x1F;
        uint8_t shift = (instruction >> 6) & 0x1F;
        uint8_t funct = instruction & 0x3F; 

        const char *mnemonic = R_FUNCT_CODES[funct];  
        printf("%s %s, %s, %s\n", mnemonic, REGISTER_NAMES[rd], REGISTER_NAMES[rs], REGISTER_NAMES[rt]);

    } else if(opcode == 0x02 || opcode == 0x03){
        uint32_t address = instruction & 0x03FFFFFF;
        printf("%s %u\n", OPCODES[opcode], address);
    } else {
        rs = (instruction >> 21) & 0x1F;
        rt = (instruction >> 16) & 0x1F;
        uint16_t imm = instruction & 0xFFFF;
        printf("%s %s, %d(%s)\n", OPCODES[opcode], REGISTER_NAMES[rt], imm, REGISTER_NAMES[rs]);

    }

}


int main(int argc, char* argv[]){

   /*
   if(argc != 2){
        return WRONG_AGRUMENTS;
    }

    load_file(argv[1]);
    return 0;
   */ 
  disassemble(0b10101111110000100000000000001000);
  return 0;
}