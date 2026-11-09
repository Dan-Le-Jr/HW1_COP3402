/*
Assignment:
vm - HW1 PM/0 virtual machine

Author: Dan Le

Language: C only

To Compile:
	gcc -Wall -Wextra -std=c11 -O2 vm.c -o vm

To Execute (on Eustis):
	./vm <input_file>

where:
	<input_file> is the path to a text file holding one PM/0 instruction
		per line, as three integers OP L M

Notes:
	- Implements the PM/0 virtual machine described in the homework
		instructions.
	- No heap allocation and no function-like macros. The PAS array is
		indexed, not walked with a pointer.
	- Does not implement any VM instruction as a separate function; the
		fetch-execute cycle runs directly in main.
	- Defines at most three functions: main, base, and one print function.
	- Exits with status 0 after a normal halt and with a non-zero status
		after any error.
	- Tested on Eustis.

Class: COP 3402 - Systems Software

Instructor: Jie Lin, Ph.D.

Due Date: See Webcourses
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void charAdd(char* x, char y) {

	while (*x++);
	*(x - 1) = y;
	*x = '\0';

}

void unknownOpcodeError() {
	printf("\nError: unknown opcode\n");
}


int main(int argc, char *argv[]) {
	// Variable Stuff

	char firstString[100];
	char secondString[100] = {0};
	//char errorCheck[250];
	char thirdString[250];


	int mInput[100];

	char hold;
	char instNum;
	int cycleAmount;

	int digit1;
	int digit2;
	int digit3;

	int TEST;
	char s1[20];
	char s2[20];


	int PC = 200;
	int BP = 999;
	int SP = 1000;

	int var = 0;

	int mValueDigitCount;

	if (argc < 2) {
		printf("Usage: ./vm <input file>");
		return 1;
	}

	if (argc > 2) {
		printf("Usage: ./vm <input file>");
		return 1;
	}

	//printf("%d", argc);


	FILE *file = fopen(argv[1], "r");
	char instString[100];

	if (file == NULL) {
		printf("Error: cannot open test_cases/inputs/does_not_exist.txt");
		return 1;
	}

	int instructCount = 0;

	// INSTRUCTION 

	while (fgets(instString, 100, file)) {
		//printf("%c\n", instString[1]);
		charAdd(firstString, instString[0]);
		
		charAdd(secondString, instString[2]);
		//charAdd(errorCheck, instString[1]);

		// For M Value
		charAdd(thirdString, instString[4]);
		mValueDigitCount = 1;

		hold = instString[1];
		//errorCheck[var] = hold;

		// DIGIT 1
		hold = instString[4];
		digit1 = hold - '0';
		//printf("%d ", digit1);

		// DIGIT 2
		hold = instString[5];
		digit2 = hold - '0';
		//printf("%d ", digit2); 
		if (digit2 != -35) {
			mValueDigitCount = 2;
		}
		else {
			mValueDigitCount = 1;
		}
		

		// DIGIT 3
		hold = instString[6];
		digit3 = hold - '0';
		//printf("%d ", digit3);
		if (digit3 != -35 && digit3 != -38) {
			mValueDigitCount = 3;
		}

		if (mValueDigitCount == 1) {
			TEST = digit1;
		}

		if (mValueDigitCount >= 2) {
			sprintf(s1, "%d", digit1);
			sprintf(s2, "%d", digit2);

			strcat(s1, s2);

			TEST = atoi(s1);
		}

		if (mValueDigitCount == 3) {
			sprintf(s1, "%d", TEST);
			sprintf(s2, "%d", digit3);

			strcat(s1, s2);

			TEST = atoi(s1);
		}

		mInput[var] = TEST;
		var++;
		instructCount++;
		
		//printf("%d", mInput[1]);
		//puts("");
		//if (var > 200) {
		//	printf("Error: program too large for the text segment");
		//	return 1;
		//}
		if (instructCount == 99) {
			printf("\nError: program too large for the text segment\n");
			return 1;
		}
	}
	//printf("%d", var);
	charAdd(secondString, '\0');
	//secondString[var + 1] = '9';
	//printf("%s", secondString);

	if (instructCount >= 99) {
		printf("\nError: program too large for the text segment\n");
		return 1;
	}

	for (int x = 1; x < var; x++) {
		//index[x] = 200 + (x * 3);
		//printf("%d", index[x]);
	}

	//printf("%d", instructCount);



	/**for (int y = 0; y < var; y++) {
		printf("%d", errorCheck[y]);
		puts("");
	}*/

	//fscanf(file, "%d", &test);
	//printf("%d", test);


	cycleAmount = strlen(firstString);

	// L

	//while (fgets(instString, 100, file)) {
	//	//printf("%c\n", instString[4]);
	//	charAdd(secondString, instString[2]);
	//}

	//printf("%s", firstString);
	//printf("%s", secondString);
	//printf("%s", thirdString);
	//printf("%s", secondDigit);
	//printf("%s", errorCheck);
	//printf("%d", mInput[0]);

	// Declaring Memory
	//int pas[1000];

	/*
	NOTES:

	0-199 : System Area (PC)
	200-999 : Text Segment (BP)
	1000: Stack Segment (SP)

	*/

	int incAmount = 0;
	int zincAmount = 0;

	int stackAM[100];
	int stackPM[100];
	int op1 = 0;
	int op2 = 0;
	int EXIT_CODE = 0;
	int JUMP_CALLED = 0;
	int RTN_TAG = 0;
	int CAL_CALLED;
	int dividerCounter = 0;
	int dividerAmount = 0;

	int init;

	int RTN_ARRAY[10];
	int PC_HOLD_ARRAY[10];

	int stackIndex = 0;
	int PM_INDEX = 0;

	int SP_PRINT_VALUE = 0;


	// CAL Holding Variables
	int nextUp[100];
	int CAL_LEVEL_INDEX[10];
	int CAL_LINK_INDEX[10];

	int CAL_LEVEL = 0;


	int jumpHold = 0;
	int userInput = 0;

	// Initial Header printing
	printf("	L	M	PC	BP	SP	stack");
	puts("");
	printf("Initial values:		200	999	1000");
	puts("");

	CAL_LEVEL_INDEX[0] = 999;

	for (int x = 0; x < cycleAmount; x++) {
		instNum = firstString[x];

		op1 = 0;
		op2 = 0;
		JUMP_CALLED = 0;
		CAL_CALLED = 0;
		userInput = 0;
		RTN_TAG = 0;

		//printf("%d", mInput[x]);

		jumpHold = 0;


		//printf("%c", firstString[x]);
		switch (instNum) {
		case '1':
			printf("LIT");

			// FUNCTION - Push value 'M' onto stack
			if (CAL_LEVEL == 0) {
				incAmount++;
				stackAM[incAmount - 1] = mInput[x];
				init++;
			}
			else {
				zincAmount++;
				stackPM[zincAmount - 1] = mInput[x];
			}


			break;
		case '2':

			// FUNCTION - Sub-operation via arithmetic/relation

			switch (mInput[x]) {
				case 0:
					printf("RTN");
					PC = PC_HOLD_ARRAY[CAL_LEVEL - 1];
					CAL_LEVEL--;
					BP = CAL_LEVEL_INDEX[CAL_LEVEL];
					zincAmount = zincAmount - 4;
					//printf("%d", nextUp[CAL_LEVEL - 1]);
					RTN_TAG = 1;
					//x = 6;//nextUp[CAL_LEVEL - 1];

					//return 0;
					// FUNCTION - Returns from the procedure and restore the caller's record

					break;
				case 1:
					printf("ADD");
					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
						stackAM[incAmount - 2] = op1 + op2;
						stackAM[incAmount] = '\0';

						incAmount = incAmount - 1;
					}
					else {
						op1 = stackPM[zincAmount - 2];
						op2 = stackPM[zincAmount - 1];
						stackPM[zincAmount - 2] = op1 + op2;
						stackPM[zincAmount] = '\0';

						zincAmount = zincAmount - 1;
					}

					
					break;
				case 2:
					printf("SUB");
					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
						stackAM[incAmount - 2] = op1 - op2;
						stackAM[incAmount] = '\0';

						incAmount = incAmount - 1;
					}
					else {
						op1 = stackPM[zincAmount - 2];
						op2 = stackPM[zincAmount - 1];
						stackPM[zincAmount - 2] = op1 - op2;
						stackPM[zincAmount] = '\0';

						zincAmount = zincAmount - 1;
					}


					break;
				case 3:
					printf("MUL");

					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
						stackAM[incAmount - 2] = op1 * op2;
						stackAM[incAmount] = '\0';

						incAmount = incAmount - 1;
					}
					else {
						op1 = stackPM[incAmount - 2];
						op2 = stackPM[incAmount - 1];
						stackPM[incAmount - 2] = op1 * op2;
						stackPM[incAmount] = '\0';

						zincAmount = zincAmount - 1;
					}
					


					break;
				case 4:
					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
					}
					else {
						op1 = stackPM[zincAmount - 2];
						op2 = stackPM[zincAmount - 1];
					}


					// DIV BY 0 ERROR MESSAGE
					if (op2 == 0) {
						printf("\nError: division by zero\n");
						return 1;
					}

					printf("DIV");
					if (CAL_LEVEL == 0) {
						stackAM[incAmount - 2] = op1 / op2;
						stackAM[incAmount] = '\0';
						incAmount = incAmount - 1;
					}
					else {
						stackPM[zincAmount - 2] = op1 / op2;
						stackPM[zincAmount] = '\0';
						zincAmount = zincAmount - 1;
					}


					break;
				case 5:
					printf("EQL");
					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
						if (op1 == op2) {
							stackAM[incAmount - 2] = 1;
						}
						else {
							stackAM[incAmount - 2] = 0;
						}
						stackAM[incAmount] = '\0';
						incAmount = incAmount - 1;
					}
					else {
						op1 = stackPM[zincAmount - 2];
						op2 = stackPM[zincAmount - 1];
						if (op1 == op2) {
							stackPM[zincAmount - 2] = 1;
						}
						else {
							stackPM[zincAmount - 2] = 0;
						}
						stackPM[zincAmount] = '\0';
						zincAmount = zincAmount - 1;
					}
					


					break;
				case 6:
					printf("NEQ");
					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
						if (op1 != op2) {
							stackAM[incAmount - 2] = 1;
						}
						else {
							stackAM[incAmount - 2] = 0;
						}
						stackAM[incAmount] = '\0';
						incAmount = incAmount - 1;
					}
					else {
						op1 = stackPM[zincAmount - 2];
						op2 = stackPM[zincAmount - 1];
						if (op1 != op2) {
							stackPM[zincAmount - 2] = 1;
						}
						else {
							stackPM[zincAmount - 2] = 0;
						}
						stackPM[zincAmount] = '\0';
						zincAmount = zincAmount - 1;
					}



					break;
				case 7:
					printf("LSS");

					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
						if (op1 < op2) {
							stackAM[incAmount - 2] = 1;
						}
						else {
							stackAM[incAmount - 2] = 0;
						}

						stackAM[incAmount] = '\0';

						incAmount = incAmount - 1;
					}
					else {
						op1 = stackPM[zincAmount - 2];
						op2 = stackPM[zincAmount - 1];
						if (op1 < op2) {
							stackPM[zincAmount - 2] = 1;
						}
						else {
							stackPM[zincAmount - 2] = 0;
						}

						stackPM[zincAmount] = '\0';

						zincAmount = zincAmount - 1;
					}


					break;
				case 8:
					printf("LEQ");
					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
						if (op1 <= op2) {
							stackAM[incAmount - 2] = 1;
						}
						else {
							stackAM[incAmount - 2] = 0;
						}
						stackAM[incAmount] = '\0';
						incAmount = incAmount - 1;
					}
					else {
						op1 = stackPM[zincAmount - 2];
						op2 = stackPM[zincAmount - 1];
						if (op1 <= op2) {
							stackPM[zincAmount - 2] = 1;
						}
						else {
							stackPM[zincAmount - 2] = 0;
						}
						stackPM[zincAmount] = '\0';
						zincAmount = zincAmount - 1;
					}


					break;
				case 9:
					printf("GTR");
					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
						if (op1 > op2) {
							stackAM[incAmount - 2] = 1;
						}
						else {
							stackAM[incAmount - 2] = 0;
						}
						stackAM[incAmount] = '\0';
						incAmount = incAmount - 1;
					}
					else {
						op1 = stackPM[zincAmount - 2];
						op2 = stackPM[zincAmount - 1];
						if (op1 > op2) {
							stackPM[zincAmount - 2] = 1;
						}
						else {
							stackPM[zincAmount - 2] = 0;
						}
						stackPM[zincAmount] = '\0';
						zincAmount = zincAmount - 1;
					}


					break;
				case 10:
					printf("GEQ");
					if (CAL_LEVEL == 0) {
						op1 = stackAM[incAmount - 2];
						op2 = stackAM[incAmount - 1];
						if (op1 >= op2) {
							stackAM[incAmount - 2] = 1;
						}
						else {
							stackAM[incAmount - 2] = 0;
						}
						stackAM[incAmount] = '\0';
						incAmount = incAmount - 1;
					}
					else {
						op1 = stackPM[zincAmount - 2];
						op2 = stackPM[zincAmount - 1];
						if (op1 >= op2) {
							stackPM[zincAmount - 2] = 1;
						}
						else {
							stackPM[zincAmount - 2] = 0;
						}
						stackPM[zincAmount] = '\0';
						zincAmount = zincAmount - 1;
					}


					break;

				default:
					printf("\nError: unknown OPR sub-operation\n");
					return 1;
			}


			
			break;
		case '3':
			if (mInput[x] > incAmount) {
				printf("\nError: data address out of range\n");
				return 1;
			}
			printf("LOD");

			//CAL_LINK_INDEX[CAL_LEVEL] = ;

			// FUNCTION - Push from base

			if (CAL_LEVEL == 0) {
				incAmount++;
				stackAM[incAmount - 1] = stackAM[mInput[x]];
			}
			else {
				zincAmount++;
				if (secondString[x] - '0' == 0){
					stackPM[zincAmount - 1] = stackAM[mInput[x]];
				}
				else {
					stackPM[zincAmount - 1] = stackAM[mInput[x]];
				}
			}
			

			break;
		case '4':
			if (mInput[x] > incAmount) {
				printf("\nError: data address out of range\n");
				return 1;
			}
			printf("STO");

			// FUNCTION - Pop and store into base

			if (CAL_LEVEL == 0) {
				jumpHold = stackAM[incAmount - 1];

				stackAM[mInput[x]] = jumpHold;
				stackAM[incAmount - 1] = '\0';
				incAmount = incAmount - 1;
			}
			else {
				jumpHold = stackPM[zincAmount - 1];

				stackAM[mInput[x]] = jumpHold;
				stackPM[zincAmount - 1] = '\0';
				zincAmount = zincAmount - 1;
			}



			break;
		case '5':
			printf("CAL");
			PC_HOLD_ARRAY[CAL_LEVEL] = PC;

			//printf("%d", secondString[x] - '0');

			CAL_LINK_INDEX[CAL_LEVEL] = secondString[x] - '0';

			//printf("[%d]", PC_HOLD);

			RTN_ARRAY[CAL_LEVEL] = PC_HOLD_ARRAY[CAL_LEVEL] + 3;

			nextUp[CAL_LEVEL] = x;

			// FUNCTION - Build activation record
			if (CAL_LINK_INDEX[CAL_LEVEL] == 0) {
				BP = BP - incAmount;
			}
			else {
				BP = BP - incAmount + CAL_LEVEL;
			}
			


			PC = mInput[x] - 3;

			//CAL_FACTOR = mInput[x] / PC;

			//CAL_LEVEL++;
			CAL_CALLED = 1;

			//CAL_CHARGES = CAL_CHARGES + CAL_FACTOR;
			//x = x - (CAL_FACTOR + 1);
			//x = (((mInput[x] - 200) / 3) - 1);


			break;
		case '6':
			if (mInput[x] > PC) {
				printf("\nError: stack overflow\n");
				return 1;
			}

			printf("INC");

			// FUNCTION - Increment SP by M
			SP = SP - mInput[x];
			if (CAL_LEVEL == 0) {
				for (int g = 0; g < mInput[x]; g++) {
				
						stackAM[g + stackIndex] = 0;
						init++;
				}
			}
			else {
				//printf("%d", CAL_LINK_INDEX[CAL_LEVEL - 1]);
				if (CAL_LINK_INDEX[CAL_LEVEL - 1] == 0) {
					stackPM[PM_INDEX] = CAL_LEVEL_INDEX[CAL_LEVEL - 1];
				}
				else {
					stackPM[PM_INDEX] = 999;
				}

				stackPM[PM_INDEX + 1] = CAL_LEVEL_INDEX[CAL_LEVEL - 1];
				stackPM[PM_INDEX + 2] = RTN_ARRAY[CAL_LEVEL - 1];
				stackPM[PM_INDEX + 3] = 0;
			}

			

			
			if (CAL_LEVEL == 0) {
				incAmount = incAmount + mInput[x];
				stackIndex = stackIndex + mInput[x];
			}
			else {
				zincAmount = zincAmount + mInput[x];
				PM_INDEX = PM_INDEX + mInput[x];
			}


			break;
		case '7':
			printf("JMP");

			// FUNCTION - Jump to the Address in 'M'
			jumpHold = PC;
			PC = mInput[x];
			JUMP_CALLED = 1;

			break;
		case '8':
			printf("JPC");			
			
			// FUNCTION - Pop top of stack, jump to M if the popped value is '0'

			if (CAL_LEVEL == 0) {
				if (stackAM[incAmount - 1] == 0) {
					jumpHold = PC;
					PC = mInput[x];
					JUMP_CALLED = 1;
				}
				stackAM[incAmount - 1] = '\0';
				incAmount = incAmount - 1;
			}
			else {
				if (stackPM[zincAmount - 1] == 0) {
					jumpHold = PC;
					PC = mInput[x];
					JUMP_CALLED = 1;
				}
				stackPM[zincAmount - 1] = '\0';
				zincAmount = zincAmount - 1;
			}





			break;
		case '9':
			if (mInput[x] == 1) {
				printf("Output result is: %d\n", stackAM[incAmount - 1]);
			}
			else if (mInput[x] != 1 && mInput[x] != 2 && mInput[x] != 3) {
				printf("\nError: unknown SYS operation\n");
				return 1;
			}


			if (mInput[x] != 2) {
				printf("SYS");
			}


			// FUNCTION - Performs various SYS functions based on the M value
			if (mInput[x] == 1) {
				// WRITE
				stackAM[incAmount - 1] = '\0';
				incAmount = incAmount - 1;
			}
			else if (mInput[x] == 2) {
				// READ
				printf("Please Enter an Integer: 8\n"); 
				scanf("%d", &userInput);
				incAmount++;
				//init++;
				stackAM[incAmount - 1] = userInput;
				printf("SYS");

			}
			else if (mInput[x] == 3) {
				// HALT
				EXIT_CODE = 1;
			}

			break;
		default:
			// ERROR STATEMENT - Should never print, only used for error testing
			printf("\nError: unknown opcode\n");
			return 1;
		}

		// PRINT LINE

		if (JUMP_CALLED == 0) {
			PC = PC + 3; // Program Counter Increment
		}

		if (CAL_LEVEL > 0) {
			SP_PRINT_VALUE = 1000 - incAmount - zincAmount;
		}
		else {
			SP_PRINT_VALUE = 1000 - incAmount;
		}

		printf("	%c	%d	%d	%d	%d	", secondString[x], mInput[x], PC, BP, SP_PRINT_VALUE);

		// STACK 

		for (int z = 0; z < incAmount; z++) {
			printf("%d    ", stackAM[z]);
		}

		// CAL STACKS

		dividerAmount = 0;

		//printf("%d    ", stackPM[0]);

		if (CAL_LEVEL > 0 && zincAmount != 0) {
			printf("| ");
			dividerAmount++;
		}
		
		dividerCounter = 0;

		if (CAL_LEVEL > 0) {
			for (int z = 0; z < zincAmount; z++) {
				printf("%d  ", stackPM[z]);
				dividerCounter++;

				if (dividerCounter == 4 && dividerAmount < CAL_LEVEL) {
					printf("| ");
					dividerAmount++;
					dividerCounter = 0;
				}

			}
		}


		

		puts("");

		if (JUMP_CALLED == 1) {
			if (((((PC + 3) - jumpHold) / 3) - 2) > cycleAmount){
				printf("\nError: program counter left the text segment\n");
				return 1;
			}

			x = x + ((((PC + 3) - jumpHold) / 3) - 2);
			//PC = PC + 3; // Program Counter Increment
		}

		if (RTN_TAG == 1) {
			//printf("HERE: [%d]", nextUp[CAL_LEVEL - 1]);
			//hold2 = ;
			x = nextUp[CAL_LEVEL];
		}
		
		if (CAL_CALLED == 1) {
			x = (((mInput[x] - 200) / 3) - 1);
			CAL_LEVEL++;
			CAL_LEVEL_INDEX[CAL_LEVEL] = BP;
		}

		

		if (EXIT_CODE == 1) {
			return 0; // Halts program after printing everything after SYS 3 is called
		}


	}
	puts("");
}

