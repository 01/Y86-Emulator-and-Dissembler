#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y86emul.h"

char* emem;
int reg[8];
int eip = 0; 
int OF, ZF, SF;
status CPUState = AOK;
int text = 0, line = 2, size = 0;

/*Performs a registry to registry move*/
void rrmovl() {
	eip++; //go to byte1
	bitfield* tempByte = (bitfield*)(emem+eip); //get byte1
	printf("----------------------RRMOVL OPERATION----------------\n");
	printf("rrmovl Reg:%x to Reg:%x\n", tempByte->low, tempByte->high);
	printf("Reg %x Value: %x Reg %x Value: %x \n", tempByte->low, reg[tempByte->low], tempByte->high, reg[tempByte->high]);
	
	if ((tempByte -> low >= 0 && tempByte -> low <= 7)&& (tempByte -> high >= 0 && tempByte -> high <= 7)) { //check if byte 1 contains two valid register number
		reg[tempByte->high] = reg[tempByte->low]; //perform rrmovl taking value of "low" register and storing it in "high"
	}
	else {
		fprintf(stderr, "test");
		exit(0);
	}
	printf("Reg %x Value After Move: %x \n", tempByte->high, reg[tempByte->high]); 
	eip++; //increment eip by 1 to point to the next location
	printf("%x%x\n" , (tempByte+1)->low, (tempByte+1)->high);
	return;	
}

/*Performs an immediate to registry move*/
void irmovl() {	
	eip++; //go to byte1
	bitfield* tempByte = (bitfield*)(emem+eip); //get byte1
	int* num = (int*)(emem + eip + 1); //get byte2-5 and interpret it as integer
	printf("----------------------IRMOVL OPERATION----------------\n");	
	printf("Irmovl %x Reg:%x\n", *num, tempByte->high);
	if ((tempByte->low == 15) && (tempByte->high >= 0 && tempByte->high <= 7)) { //check if byte1 is valid		
		reg[tempByte->high] = *num; //perform irmovl with stored integer from bytes 2-5
	}
	else {
		fprintf(stderr, "Error");
		exit(0);
	}	
	printf("Reg %x Value After Operation: %x\n", tempByte->high, reg[tempByte->high]);
	eip+=5; //increment eip by 5 to point to the next location
	return;	
}

/*Performs a registry to memory move*/
void rmmovl() {
	eip++; //go to byte1
	bitfield* tempByte = (bitfield*)(emem+eip); //get byte1
	int* num = (int*)(emem + eip + 1); //get byte2-5 and interpret it as integer
	printf("----------------------RMMOVL OPERATION----------------\n");	
	printf("rmmovl Reg:%x, %x(Reg: %x) \n", tempByte->low, *num, tempByte->high);
	printf("Value at memory Address (%x + %x) Before Operation: %x \n", *num, tempByte->high, *(int*)(emem+tempByte->high + *num));
	printf("Reg %x Value: %x   Reg %x Value: %x Displacement: %x \n", tempByte->low, reg[tempByte->low], tempByte->high, reg[tempByte->high], *num);
	if ((tempByte -> low >= 0 && tempByte -> low <= 7)&& (tempByte -> high >= 0 && tempByte -> high <= 7)) { //check if byte 1 contains two valid register number
	*(int *) (emem + reg[tempByte->high] + *num)= reg[tempByte->low]; //perform rrmovl taking value of "low" register and storing it in "high"
	}
	else {
		fprintf(stderr, "Error");
		exit(0);
	}
	printf("Value at memory Address %x After operation: %x\n", (tempByte->high + *num), *(int*)(emem+tempByte->high + *num));
	eip += 5; 
	return;
}

/*Performs a memory to registry move*/
void mrmovl() {
	eip++; //go to byte1
	bitfield* tempByte = (bitfield*)(emem+eip); //get byte1
	int* num = (int*)(emem + eip + 1); //get byte2-5 and interpret it as integer for displacement
	printf("----------------------MRMOVL OPERATION----------------\n");	
	printf("mrmovl %x(Reg:%x), Reg: %x) \n", *num, tempByte->low, tempByte->high);
	printf("Value at memory Address (%x + %x) Before Operation: %x \n", *num, tempByte->low, emem[tempByte->low + *num]);
	printf("Displacement: %x Reg %x Value: %x   Reg %x Value: %x \n", *num, tempByte->low, reg[tempByte->low], tempByte->high, reg[tempByte->high]);	
	if ((tempByte -> low >= 0 && tempByte -> low <= 7)&& (tempByte -> high >= 0 && tempByte -> high <= 7)) { //check if byte 1 contains two valid register numbers
		reg[tempByte->high]= *(int *)(emem + reg[tempByte->low] + *num); //perform rrmovl taking value of "low" register and storing it in "high"
	}
	else {
		fprintf(stderr, "Error");
		exit(0);
	}
	eip += 5; 



	return;
}

/*Perform an operation between two registers,
 * can be addition, subtraction, multiplication
 * bitwise and or exclusive or */
 void op1() {
	bitfield* tempByte = (bitfield*)(emem+eip);
	bitfield* regByte = tempByte + 1;
	int val;
	ZF = 0;
	OF = 0;
	SF = 0;
	if ((regByte -> low >= 0 && regByte -> low <= 7)&& (regByte -> high >= 0 && regByte -> high <= 7)) { //check if byte 1 contains two valid register number			
		switch(tempByte->high){
			
			case 0: 
				printf("-------------------------ADDL OPERATION ----------------\n");
								
				printf("addl Reg:%x Reg:%x \n", regByte->low, regByte->high);
				printf("Reg %x: %x  += Reg %x: %x\n", regByte->high, reg[regByte->high], regByte->low, reg[regByte->low]);				
				val= reg[regByte->low]+ reg[regByte->high];
				if(val == 0) ZF =1;
				if(val < 0) SF =1;
				if((reg[regByte->low]>0 && reg[regByte->high] > 0 && val<0) || (reg[regByte->low]<0 && reg[regByte->high] < 0 && val>0)) OF = 1;
				reg[regByte->high]= val;	
				printf("Reg %x Value After Operation: %x \n", regByte->high, reg[regByte->high]);
				break;
			
			case 1:
				printf("-------------------------SUBL OPERATION ----------------\n");
				printf("subl Reg:%x Reg:%x \n", regByte->low, regByte->high);
				printf("Reg %x: %x  -= Reg %x: %x\n", regByte->high, reg[regByte->high], regByte->low, reg[regByte->low]);
				val = reg[regByte->high]-reg[regByte->low];			
				if(val == 0) ZF =1;
				if(val < 0) SF =1;
				if((reg[regByte->low]<0 && reg[regByte->high] > 0 && val<0) || (reg[regByte->low]>0 && reg[regByte->high] < 0 && val>0)) OF = 1;
				reg[regByte->high]=val;
				printf("Reg %x Value after Operation: %x \n", regByte->high, reg[regByte->high]);
				break;
		
			case 2:
				printf("-------------------------ANDL OPERATION ----------------\n");
				printf("andl Reg:%x Reg:%x \n", regByte->low, regByte->high);
				printf("Reg %x: %x  andl Reg %x: %x\n", regByte->high, reg[regByte->high], regByte->low, reg[regByte->low]);
				val = (reg[regByte->high]&& reg[regByte->low]);	
				if(val == 0) ZF =1;
				if(val < 0) SF =1;			
				reg[regByte->high]=(reg[regByte->high]&& reg[regByte->low]);
				printf("Reg %x Value after Operation: %x \n", regByte->high, reg[regByte->high]);
				break;
			
			case 3: 
				printf("-------------------------XORL OPERATION ----------------\n");
				printf("xorl Reg:%x Reg:%x \n", regByte->low, regByte->high);
				printf("Reg %x: %x  andl Reg %x: %x\n", regByte->high, reg[regByte->high], regByte->low, reg[regByte->low]);					
				val = reg[regByte->high]^reg[regByte->low];
				if(val == 0) ZF =1;
				if(val < 0) SF =1;
				reg[regByte->high] = val;
				printf("Reg %x Value after Operation: %x \n", regByte->high, reg[regByte->high]);
				break;
			
			case 4:
				printf("-------------------------MULL OPERATION ----------------\n");
				printf("mull Reg:%x Reg:%x \n", regByte->low, regByte->high);
				printf("Reg %x: %x  *= Reg %x: %x\n", regByte->high, reg[regByte->high], regByte->low, reg[regByte->low]);				
				val = reg[regByte->low] * reg[regByte->high];
				if(val == 0) ZF =1;
				if(val < 0) SF =1;
				if((reg[regByte->low]>0 && reg[regByte->high] > 0 && val<0) || (reg[regByte->low]<0 && reg[regByte->high] > 0 && val>0)||(reg[regByte->low]<0 && reg[regByte->high]<0 && 						val>0) || (reg[regByte->low]>0 && reg[regByte->high] < 0 && val>0)) OF=1; 			
				reg[regByte->high] = val;
				printf("Reg %x Value after Operation: %x \n", regByte->high, reg[regByte->high]);
				break;
		
			default:
				fprintf(stderr, "test");
				exit(0); 
				break;
				
		}					
	}
	else{
		fprintf(stderr, "test");
		exit(0); 
	} 
	eip = eip +2;
	return;
 }
 
 /*Will perform various code jumps depending 
  * on which jump is declared and what flags
  * are on/off.*/
void jXX(){	
	bitfield* tempByte = (bitfield*)(emem+eip);
	int* num = (int*)(emem + eip +1);
	if ((tempByte->high >= 0 && tempByte->high <= 6)) { //check if byte 1 contains two valid register number		
		switch(tempByte->high){
			
			case 0: 
				printf("-------------------------JMP OPERATION ----------------\n");
				eip = *num;
				break;
			
			case 1:
				printf("-------------------------JLE OPERATION ----------------\n");
				if((SF ^ OF) || ZF) eip = *num;
				else eip+=5;
				break;
		
			case 2:
				printf("-------------------------JL OPERATION ----------------\n");
				if(SF ^ OF) eip = *num;
				else eip+=5;
				break;
			
			case 3: 
				printf("-------------------------JE OPERATION ----------------\n");
				if(ZF) eip = *num;
				else eip+=5;
				break;
			
			case 4:
				printf("-------------------------JNE OPERATION ----------------\n");
				if(!ZF) eip = *num;
				else eip+=5;
				printf("Displacement: %x\n", *num);
				break;
			
			case 5:
				printf("-------------------------JGE OPERATION ----------------\n");
				if(!(SF ^ OF)) eip = *num;
				else eip+=5;
				break;

			case 6:
				printf("-------------------------JG OPERATION ----------------\n");
				if(!(SF ^ OF) && !ZF) eip = *num;
				else eip+=5;
				break;
		
			default:
				fprintf(stderr, "test");
				exit(0); 
				break;
				
		}					
	}
	else{
		fprintf(stderr, "test");
		exit(0); 
	}
	return;
}

/* Calls the function at the destination*/
void call() {
	eip++;
	int* num = (int*)(emem + eip); // gets displacement for Call
	printf("-------------------------Call OPERATION ----------------\n");
	printf("Call (Displacement: %i\n_", *num);
	eip += 4;
	printf("reg[esp] = %x\n", reg[esp]);
	reg[esp]-=4;	
	*(int *)(emem + reg[4]) = eip;
	eip = *num;
	printf("EIP: %d\n", eip);
	//printf("%x%x\n", tempByte->low, tempByte->high); 
	return;
}

/*returns from a function call*/
void ret() {
	/**** fill your code here ****/
	printf("-------------------------Ret OPERATION ----------------\n");
	eip = *(int *) (emem + reg[esp]);
	printf("Ret\n");
	reg[esp]+=4;
	return;
}

/*Pushes onto the top of the stack*/
void pushl() {
	/**** fill your code here ****/	
	
	eip++;
	bitfield* tempByte = (bitfield*)(emem+eip);
	printf("-------------------------pushl OPERATION ----------------\n");
	printf("Reg[%x]= %x\n", tempByte->low, reg[tempByte->low]);
	printf("pushl Reg[%x]\n", tempByte->low);
	if ((tempByte -> low >= 0 && tempByte -> low <= 7)&& tempByte->high == 15) {
		reg[esp]-=4;
		*(int*)(emem + reg[esp]) = reg[tempByte->low];		
	}
	else {
		fprintf(stderr, "Invalid Address");
		exit(0); 
				
	}	
	eip+=1;
	return;
}

/*pop off the top of the stack*/
void popl() {
	
	eip++;
	bitfield* tempByte = (bitfield*)(emem+eip);
	printf("popl reg[%x]\n", tempByte->low);
	if ((tempByte -> low >= 0 && tempByte -> low <= 7)&& tempByte->high == 15) {		
		reg[tempByte->low]= *(int *) (emem + reg[esp]);
		reg[esp]+=4;
	}
	else {
		fprintf(stderr, "Invalid Address");
		exit(0); 
				
	}	
	eip++;
	return;

}

/*Reads in character input*/
void readB() {
	eip++;
	bitfield* tempByte = (bitfield*)(emem+eip);//get the first byte
	int* num = (int*)(emem + eip + 1);//get the displacement by type casting the pointer to (int*) so that the computer would interpret the next 4 bytes as an integer
	char input;
	OF = 0;
	SF = 0;
	if (scanf("%c",&input) == EOF) { 
		ZF = 1;	//set ZF = 1 when end of file symbol is read in
	}
	else {
		ZF = 0;
		if ((tempByte -> low >= 0 && tempByte -> low <= 7) && (tempByte -> high == 15)) {//if the first half of byte1 is 0~7(which is a valid register), and the second half of byte2 is 'F', it is considered as a valid readB instruction
			emem[reg[tempByte -> low]+ *num] = input;
		}
		else {
			fprintf(stderr,"Read error.\n");
			exit(0);
		}
	}
	eip+=5; //eip should be incremented by 5 to point to the start of next instruction
	return;
}

/*Reads in long*/
void readL() {	
	eip++;;
	bitfield* tempByte = (bitfield*)(emem+eip);//get the first byte
	int* num = (int*)(emem + eip + 1);//get the displacement by type casting the pointer to (int*) so that the computer would interpret the next 4 bytes as an integer
	int input;
	OF = 0;
	SF = 0;
	printf("Enter Integer: \n");
	if(scanf("%i", &input) == EOF) { 
		printf("1m\n");		
		ZF = 1;	//set ZF = 1 when end of file symbol is read in
	}
	else {
		ZF = 0;
		printf("2m\n");	
		if ((tempByte -> low >= 0 && tempByte -> low <= 7) && (tempByte -> high == 15)) {//if the first half of byte1 is 0~7(which is a valid register), and the second half of byte2 is 'F', it is considered as a valid readL instruction
			printf("Makes it here");			
			*(int *)(emem + reg[tempByte -> low]+ *num) = input;
		}
		else {
			fprintf(stderr,"Read error.\n");
			exit(0);
		}
	}
	eip+=5; //eip should be incremented by 5 to point to the start of next instruction

	return;
}

/*Writes a char to terminal*/
void writeB() {
	eip++; //go to byte1
	bitfield* tempByte = (bitfield*)(emem+eip); //get byte1
	int* num = (int*)(emem + eip + 1); //get byte2-5 and interpret it as integer
	char charCurrByte;
	if ((tempByte -> high == 15) && (tempByte -> low >= 0 && tempByte -> low <= 7)) { //check if byte1 is valid
		charCurrByte = emem[reg[tempByte -> low]+*num]; //set charCurrByte to be the character prepared to be printed out
	}
	else {
		fprintf(stderr,"Write error.\n");
		exit(0);
	}
	printf("\n%c\n\n",charCurrByte);
	eip+=5; //increment eip by 5 to point to the next location
	return;	
}

/*Writes a long to terminal*/
void writeL() {
	eip++; //go to byte1
	bitfield* tempByte = (bitfield*)(emem+eip); //get byte1
	int* num = (int*)(emem + eip + 1); //get byte2-5 and interpret it as integer
	int longBytes;
	if ((tempByte -> high == 15) && (tempByte -> low >= 0 && tempByte -> low <= 7)) { //check if byte1 is valid
		longBytes = *(int *)(emem + reg[tempByte -> low]+*num); //set intCurrByte to be the 4 bytes prepared to be printed out
	}
	else {
		fprintf(stderr,"Write error.\n");
		exit(0);
	}
	printf("%i",longBytes);
	eip+=5; //increment eip by 5 to point to the next location*/
	return;		/****fill your code here****/

}

/*Will scan the instrunction string
 * and begin running the operations 
 * as defined. */
void runProgram() {
	bitfield* tempByte;
	while (CPUState == AOK) {
		tempByte = (bitfield*)(emem+eip);
		printf("Current Instruction %x%x\n" , tempByte->low, tempByte->high);
		printf("Next Instruction: %x%x\n", (tempByte +1)->low, (tempByte+1)->high);
		if (tempByte -> low == 0) {					//nope code
			if (tempByte -> high == 0) {
				eip++;
				continue;
			}			
		}
		else if (tempByte -> low == 1) {				//halt code
			if (tempByte -> high == 0) {
				CPUState = HLT;
				continue;
			}
		}
		else if (tempByte -> low == 2)	{			//registry move
			if (tempByte -> high == 0) {
				rrmovl();
				continue;
			}				
		}
		else if (tempByte -> low == 3)	{			//immediate to registry move
			if (tempByte -> high == 0) {				
				irmovl();
				continue;
			}
		}
		else if (tempByte -> low == 4)	{			//registry to memory move
			if (tempByte -> high == 0) {
				rmmovl();
				continue;
			}
		}		
		else if (tempByte -> low == 5)	{			//memory to registry move
			if (tempByte -> high == 0) {
				mrmovl();
				continue;
			}
		}
		else if (tempByte -> low == 6)	{			//operation
			if (tempByte -> high >= 0 && tempByte -> high <= 4) {
				op1();
				continue;
			}
		}
		else if (tempByte -> low == 7)	{			//jump
			if (tempByte -> high >= 0 && tempByte -> high <= 6) {
				jXX();
				continue;
			}
		}
		else if (tempByte -> low == 8)	{			//call
			if (tempByte -> high == 0) {
				call();
				continue;
			}
		}
		else if (tempByte -> low == 9)	{			//return
			if (tempByte -> high == 0) {
				ret();
				continue;
			}
		}
		else if (tempByte -> low == 10)	{			//pushl
			if (tempByte -> high == 0) {
				pushl();
				continue;			
			}
		}
		else if (tempByte -> low == 11)	{			//popl
			if (tempByte -> high == 0) {
				popl();
				continue;
			}
		}
		else if (tempByte -> low == 12)	{			//read
			if (tempByte -> high == 0 ) {
				readB();
				continue;
			}
			else if (tempByte -> high == 1) {
				readL();
				continue;
			}
		}
		else if (tempByte -> low == 13)	{			//write
			if (tempByte -> high == 0) {
				writeB();
				continue;
			}
			else if (tempByte -> high == 1) {
				writeL();
				continue;
			}
		}
		fprintf(stderr,"Invalid instruction found.\n");
		exit(0);
	}
	return;
}

/*Takes in the .byte string and input
 * the byte into the memmory location in 
 * array. */ 
void byteToMem(char* memAddr, char* val) {
	int addrStart = strtol(memAddr,NULL,16);
	if (addrStart < 0 || addrStart > size) {
		fprintf(stderr,"OUT OF BOUNDS AT LINE %d.\n",line);
		exit(0);
	}
	int addVal = (int)strtol(val,NULL,16);
	emem[addrStart] = (char)addVal;
	return;
}

/* Takes in the .text string and inputs the 
 * instruction set 2 characters per "byte" of
 * array. Use of bitfield casting allows for
 * entry into a char array. */
void textToMem(char* memAddr, char* val) {
	bitfield* tempByte;
	int addrStart = strtol(memAddr,NULL,16), i = 0;
	if (addrStart < 0 || addrStart > size) {
		fprintf(stderr,"OUT OF BOUNDS AT LINE %d.\n",line);
		exit(0);
	}
	if (strlen(val)%2 != 0) {
		fprintf(stderr,"The length of the instruction set is odd, code missing in instruction set.\n");
		exit(0);
	}
	eip = addrStart;
	for (; i < strlen(val); i+=2, addrStart++) {
		tempByte = (bitfield*) emem+addrStart;
		char* upper = (char*)malloc(1);
		char* lower = (char*)malloc(1);
		strncpy(upper,val+i,1);
		strncpy(lower,val+i+1,1);
		tempByte -> high = strtol(lower,NULL,16);
		tempByte -> low = strtol(upper,NULL,16);		
	}	
	text = 1;	
	return;
}

/*Stores an inputted .long directive 
 * into one array location. */
void longToMem(char* memAddr, char* val) {
	bitfield* tempByte;
	int addrStart = strtol(memAddr,NULL,16);
	if (addrStart < 0 || addrStart > size) {
		fprintf(stderr,"OUT OF BOUNDS AT LINE %d.\n",line);
		exit(0);
	}
	int Value = (int)strtol(val,NULL,10);
	int* num = (int*)(emem + addrStart);
	*num = Value;			
}

/*Stores an inputted .string directive one char
 * into one array location. */
void stringToMem(char* memAddr) {
	char* val = strtok(NULL,delim);
	if (val == NULL) {
		fprintf(stderr,"No string given after .string directive.\n");
		exit(0);
	}	
	int addrStart = strtol(memAddr,NULL,16);	
	if (*val == '"') {
		val++;
	}
	else {
		fprintf(stderr,"String must be contained within double quotes.\n");
		exit(0);
	}	
	while (val != NULL) {
		while (*val != '\0') {
			emem[addrStart] = *val;
			addrStart++;
			val++;
		}
		emem[addrStart] = ' ';
		addrStart++;
		val = strtok(NULL,delim);
	}
	addrStart-=2;
	if (emem[addrStart] != '"') {
		fprintf(stderr,"String must be contained within double quotes.\n");
		exit(0);
	}
	else {
		emem[addrStart] = '\0';
	}	
	return;
}

/*Starts reading in file and storing based on string inputed*/
void storeToMemory(char* temp) {
	if (temp == NULL ) {
		return;
	}
	char* stringTemp = temp;
	char* typeIn = strtok(temp,delim);
	char* memAddr = strtok(NULL,delim);	
	if (typeIn != NULL && memAddr == NULL) {
		fprintf(stderr,"Error at line: %d\n",line);
		exit(0);
	}	
	if (typeIn == NULL && memAddr == NULL) {
		return;
	}	
	if (strcmp(typeIn, ".string")==0) {//handle the .string line
		stringToMem(memAddr);
		line++;
		return;
	}
	char* val = strtok(NULL,delim);
	if (val == NULL) {
		fprintf(stderr,"Error at line: %d\n",line);
		exit(0);
	}	
	if (strcmp(typeIn,".text")==0) {//handle the .text line
		textToMem(memAddr,val);
	}
	else if (strcmp(typeIn,".byte")==0) {//handle the .byte line
		byteToMem(memAddr,val);
	}
	else if(strcmp(typeIn, ".long")==0) {//handle the .long line
		longToMem(memAddr,val);
	}
	else {
		fprintf(stderr,"Unknown directive encountered. Line: %d\n",line);
		exit(0);
	}	
	line++;
	return;
}

/* Set array size for memory */
void setMemory(char* temp) {
	//check the first line's content
	//it should begin with .size with a valid number
	if (strcmp(temp,"\n") == 0) {
		fprintf(stderr,"The first line should contain the memmory size.\n");
		exit(0);
	}	
	char* token;
	token = strtok(temp,delim);	
	if (strcmp(token,".size") != 0) {
		fprintf(stderr,"The first line should contain the memmory size.\n");
		exit(0);
	}
	token = strtok(NULL,delim);
	if (token == NULL) {
		fprintf(stderr,"No size given after size directive.\n");
		exit(0);
	}
	if (sscanf(token,"%x",&size) <= 0) {
		fprintf(stderr,"A number is needed after size directive.\n");
		exit(0);
	}
	emem = (char*)malloc(sizeof(char)*size);
	return;
}

/* Main takes in a file path and runs the code
 * contained within the file. User can enter in 
 * "-h" flag for help. */
int main(int argc, char** argv) {
	if (argc < 2 || argc > 3) {
		fprintf(stderr,"Arguments given are not in valid form\n");
		return -1;
	}
	
	if (strcmp(argv[1], "-h") == 0) {
		printf("Enter in \"y86emul\" followed by a space and then the filename\n");
		return 0;
	}
		
	FILE* fp;
	char strTemp[5000];	
	fp = fopen(argv[1], "r");	
	if (fp == NULL) {
		printf("The filename could not be found\n");
		exit(1);
	}		
	if (fgets(strTemp,5000,fp)==NULL) {
		printf("File is empty.");
		exit(1);
	}
	setMemory(strTemp);//initialize the emulated memory to have the required size	
	while (fgets(strTemp,5000,fp) != NULL) {
		storeToMemory(strTemp);
	}//read in the file content and fill in the emulated memory
	if (text == 0) {
		fprintf(stderr,".text directive not found.\n");
		exit(0);
	}
	
	runProgram();
	return 0;	
}
