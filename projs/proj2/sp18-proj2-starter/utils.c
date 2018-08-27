#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Sign extends the given field to a 32-bit integer where field is
 * interpreted an n-bit integer. */
int sign_extend_number(unsigned int field, unsigned int n);
Instruction parse_rtype(uint32_t instruction_bits);
Instruction parse_itype(uint32_t instruction_bits);
Instruction parse_stype(uint32_t instruction_bits);
Instruction parse_utype(uint32_t instruction_bits);
//int main(int argc, char **argv){
//    /*int32_t num = sign_extend_number(9,32);
//      printf("number is %d", num);
//    */
//    Instruction inst = parse_instruction(0x009402b3);
//    printf(inst.rtype.opcode);
//    return 0;
//}
int sign_extend_number( unsigned int field, unsigned int n) {
    /* YOUR CODE HERE */
    int32_t num = 0;
    unsigned int i;
    unsigned int bitnum =1;
    for(i = 0; i < n; i++){
        if(i == n-1){
            num = -(1 << (i)) * (field >> i) + num;
        } else {
            bitnum = 1 << i;
            num += field & bitnum;
        }
    }
    return num;
}

/* Unpacks the 32-bit machine code instruction given into the correct
 * type within the instruction struct */ 
Instruction parse_instruction(uint32_t instruction_bits) {
    /* YOUR CODE HERE */
    Instruction instruction;
    unsigned int opcode = instruction_bits & 0x7F;
    switch(opcode) {
    case 0x33:
        instruction = parse_rtype(instruction_bits);
        break;
    case 0x03:
    case 0x13:
    case 0x73:
        instruction = parse_itype(instruction_bits);
        break;
    case 0x37:
        // instruction.utype.opcode = opcode;
        instruction = parse_utype(instruction_bits);
        break;
    case 0x6f:
        //  instruction.ujtype = opcode;
        instruction = parse_utype(instruction_bits);
        break;
    case 0x63:
        //  instruction.sbtype = opcode;
        instruction = parse_rtype(instruction_bits);
        break;
    case 0x23:
        //  instruction.stype = opcode;
        instruction = parse_stype(instruction_bits);
        break;
    }
    return instruction;
}
Instruction parse_rtype(uint32_t instruction_bits) {
    Instruction instruction;
    instruction.rtype.opcode = instruction_bits & 0x7F;
    instruction.rtype.rd = (instruction_bits >> 7) & 0x1F;
    instruction.rtype.funct3 = (instruction_bits >> 12) & 0x07;
    instruction.rtype.rs1 = (instruction_bits >> 15) & 0x1F;
    instruction.rtype.rs2 = (instruction_bits >> 20) & 0x1F;
    instruction.rtype.funct7 = (instruction_bits >> 25) & 0x7F;
    return instruction;
}

Instruction parse_itype(uint32_t instruction_bits) {
    Instruction instruction;
    instruction.itype.opcode = instruction_bits & 0x7F;
    instruction.itype.rd = (instruction_bits >> 7) & 0x1F;
    instruction.itype.funct3 = (instruction_bits >> 12) & 0x07;
    instruction.itype.rs1 = (instruction_bits >> 15) & 0x1F;
    instruction.itype.imm = (instruction_bits >> 20) & 0xFFF;
    return instruction;
}

Instruction parse_stype(uint32_t instruction_bits) {
    Instruction instruction;
    instruction.stype.opcode = instruction_bits & 0x7F;
    instruction.stype.imm5 = (instruction_bits >> 7) & 0x1F;
    instruction.stype.funct3 = (instruction_bits >> 12) & 0x07;
    instruction.stype.rs1 = (instruction_bits >> 15) & 0x1F;
    instruction.stype.rs2 = (instruction_bits >> 20) & 0x1F;
    instruction.stype.imm7 = (instruction_bits >> 25) & 0x7F;
    return instruction;
}
Instruction parse_utype(uint32_t instruction_bits) {
    Instruction instruction;
    instruction.utype.opcode = instruction_bits & 0x7F;
    instruction.utype.rd = (instruction_bits >> 7) & 0x1F;
    instruction.utype.imm = (instruction_bits >> 12) & 0xFFFFF;
    return instruction;
}

/* Return the number of bytes (from the current PC) to the branch label using the given
 * branch instruction */
int get_branch_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    unsigned int imm5 = instruction.sbtype.imm5;
    unsigned int imm7 = instruction.sbtype.imm7;
    return imm5 + imm7 * (1 << 5);
}

/* Returns the number of bytes (from the current PC) to the jump label using the given
 * jump instruction */
int get_jump_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    int imm =(instruction.ujtype.imm & 0xFF) * (1 << 11)
              + ((instruction.ujtype.imm >> 8) & 0x01) * (1 << 10)
              + ((instruction.ujtype.imm  >> 9) & 0x3FF) +
        ((instruction.ujtype.imm >> 19) * ( 1 << 19));
    return imm << 1;
}


int get_store_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    unsigned int imm5 = instruction.stype.imm5;
    unsigned int imm7 = instruction.stype.imm7;
    return imm5 + imm7 * (1 << 5);
}

void handle_invalid_instruction(Instruction instruction) {
    printf("Invalid Instruction: 0x%08x\n", instruction.bits); 
}

void handle_invalid_read(Address address) {
    printf("Bad Read. Address: 0x%08x\n", address);
    exit(-1);
}

void handle_invalid_write(Address address) {
    printf("Bad Write. Address: 0x%08x\n", address);
    exit(-1);
}

