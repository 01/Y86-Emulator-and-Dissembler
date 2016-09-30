
#define eax 0
#define ecx 1
#define edx 2
#define ebx 3
#define esp 4
#define ebp 5
#define esi 6
#define edi 7

#ifndef Y86EMUL_H
#define Y86EMUL_H


/*in C, it's not possible to access half-a-byte in memory
 * instead we use a strcture to have size = 1 byte
 * and use 2 fields to represent the higher half and the lower half*/

typedef struct bitfield_ {
	unsigned char high:4;
	unsigned char low:4;
}	bitfield;

//4 CPU states
typedef enum state {
	AOK,
	HLT,
	ADR,
	INS,
} status;

//functions that you need to implement
void rrmovl();
void irmovl();
void rmmovl();
void mrmovl();
void op1();
void jXX();
void call();
void ret();
void popl();
void readB();//has been implemented for you
void readL();
void writeB();//has been implemented for you
void writel();

const char delim[] = " \t\n\v\f\r";

#endif
