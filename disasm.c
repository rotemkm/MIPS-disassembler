#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define WRONG_AGRUMENTS -1
#define CANNOT_OPEN_FILE -2
#define CANNOT_CLOSE_FILE -3

static const char *r_funct_codes[] = {
	[0x20] = "add", [0x21] = "addu", [0x24] = "and", [0x1A] = "div",
	[0x1B] = "divu", [0x09] = "jalr", [0x08] = "jr", [0x10] = "mfhi",
	[0x11] = "mthi", [0x12] = "mflo", [0x13] = "mtlo", [0x18] = "mult",
	[0x19] = "multu", [0x27] = "nor", [0x26] = "xor", [0x25] = "or",
	[0x2A] = "slt", [0x2B] = "sltu", [0x00] = "sll", [0x02] = "srl",
	[0x03] = "sra", [0x22] = "sub", [0x23] = "subu"
};

static const char *opcodes[] = {
	[0x08] = "addi", [0x09] = "addiu", [0x0C] = "andi", [0x04] = "beq",
	[0x06] = "blez", [0x05] = "bne", [0x07] = "bgtz", [0x20] = "lb",
	[0x24] = "lbu", [0x25] = "lhu", [0x0F] = "lui", [0x23] = "lw",
	[0x0D] = "ori", [0x28] = "sb", [0x29] = "sh", [0x0A] = "slti",
	[0x0B] = "sltiu", [0x2B] = "sw", [0x02] = "j", [0x03] = "jal"
};

static const char *register_names[] = {
	"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
	"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
	"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
	"$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

static char instruction_type_identifier(uint8_t opcode)
{
	if (opcode == 0x00){
        return 'R';
    } else if (opcode == 0x02 || opcode == 0x03) {
        return 'J';
    } else {
        return 'I';
    }	
}

static void disassemble_r_type(uint32_t instruction)
{
	uint8_t rs, rt, rd, funct;
	const char *mnemonic;

	rs = (instruction >> 21) & 0x1F;
	rt = (instruction >> 16) & 0x1F;
	rd = (instruction >> 11) & 0x1F;
	funct = instruction & 0x3F;

	mnemonic = r_funct_codes[funct];
	if (mnemonic){
        printf("%s %s, %s, %s\n", mnemonic, register_names[rd],
		       register_names[rs], register_names[rt]);
    } else {
        printf("Unknown R-type instruction\n");
    }	
}

static void disassemble_j_type(uint32_t instruction, uint8_t opcode)
{
	uint32_t address;
	const char *mnemonic;

	address = (instruction & 0x03FFFFFF) << 2;
	mnemonic = opcodes[opcode];

	if (mnemonic){
        printf("%s 0x%08X\n", mnemonic, address);
    } else {
        printf("Unknown J-type instruction\n");
    }	
}

static void disassemble_i_type(uint32_t instruction, uint8_t opcode)
{
	uint8_t rs, rt;
	int16_t imm;
	const char *mnemonic;

	rs = (instruction >> 21) & 0x1F;
	rt = (instruction >> 16) & 0x1F;
	imm = instruction & 0xFFFF;

	mnemonic = opcodes[opcode];
	if (mnemonic){
        printf("%s %s, %d(%s)\n", mnemonic, register_names[rt],
		       imm, register_names[rs]);
    } else {
        printf("Unknown I-type instruction\n");
    }
}

static void disassemble(uint32_t instruction)
{
	uint8_t opcode;
	char instruction_type;

	opcode = (instruction >> 26) & 0x3F;
	instruction_type = instruction_type_identifier(opcode);

	switch (instruction_type) {
	case 'R':
		disassemble_r_type(instruction);
		break;
	case 'J':
		disassemble_j_type(instruction, opcode);
		break;
	case 'I':
		disassemble_i_type(instruction, opcode);
		break;
	default:
		printf("Invalid instruction\n");
		break;
	}
}

int process_instructions(FILE *fp) 
{
    uint32_t instruction;

    while (fread(&instruction, sizeof(uint32_t), 1, fp) == 1) {
        disassemble(instruction);
    }

    return 0;
}

int load_file(char * file_name)
{
    FILE * fp = NULL;
    fp = fopen(file_name, "r");

    if(!fp){
        printf("Couldn't open %s.\n", file_name);
        return CANNOT_OPEN_FILE;
    }

    process_instructions(fp);

    if(fclose(fp) != 0){
        printf("Couldn't close %s.\n", file_name);
        return CANNOT_CLOSE_FILE;
    }
 
    return 0;
}

int main(int argc, char ** argv)
{
	if (argc < 2) {
        fprintf(stderr, "Usage: %s <elf-file>\n", argv[0]);
        return WRONG_AGRUMENTS;
    }

	load_file(argv[1]);

	/* Test with an example instruction (MIPS `sw $t0, 8($t1)`) */
	uint32_t test_instruction = 0xAD200008;

	disassemble(test_instruction);
	return 0;
}
