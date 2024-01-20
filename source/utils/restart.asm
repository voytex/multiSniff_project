.global RestartMCU
.global _c_int00
.global __stack

/* === RestartMCU
/* Short routine to perform MCU restart by zeroing R0..R12 registers
/* and setting Stack Pointer register to the start address of stack
/* ('__stack' symbol)
/*/
RestartMCU:

	LDR		SP,=__stack		/* Set Stack Pointer to the beginning of the stack memory */

	MOV		R0,#0			/* Zero out R0.. R12 registers */
	MOV		R1,#0			/* (R13 register is somehow mapped to the SP) */
	MOV		R2,#0
	MOV		R3,#0
	MOV		R4,#0
	MOV		R5,#0
	MOV		R6,#0
	MOV		R7,#0
	MOV		R8,#0
	MOV		R9,#0
	MOV		R10,#0
	MOV		R11,#0
	MOV		R12,#0
	MOV		R14,#0xFFFFFFFF /* Set R14 to the initial value */

	B 		_c_int00		/* Branch into the init sequence */
