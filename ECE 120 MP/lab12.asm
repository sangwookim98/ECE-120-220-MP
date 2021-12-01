; The table below represents an 8x16 font.  For each 8-bit extended ASCII
; character, the table uses 16 memory locations, each of which contains
; 8 bits (the high 8 bits, for your convenience) marking pixels in the
; line for that character.


;Table of registers used
;R0 - register mainly used to print out on the console
;R1 - register has the offset value at first, but later loads memory of R6 to check if current address has a 1 or 0.
;R2 - register that has value -1 loaded for decrementing counters
;R3 - row counter set at 16
;R4 - column counter set at 8
;R5 - register used to load content from x5000 or x5000 depending on bit is 1 or 0
;R6 - location of first address in FONT_DATA


;The program first loads the address of FONT_DATA into R6. 
;At the same time, the Hex value of x5002 is then loaded into R1. As lab 12 stated, each letter in FONT_DATA
;has a height of 16, width of 8. In order to calculate the offset, the Hex value needs to be multiplied by 16.
;This multiplication result is then stored into R1 and then will be added to R6, which will be the
;starting address location of the letter selected at x5002. After locating the appropriate address of the letter,
;the registers for column and row counters will now be set to 8 and 16 accordingly. 
;Using LDR, the memory at address in R6 now will be loaded to R1. This is to have R1 read 1 or 0 at the memory
;in the address of FONT_DATA. Using a Branch statement on R1, if R1 detects 1, it will move to the label OUTONE,
;and if R1 detects 0, it will move to the label OUTZERO. From here, depending on R1 reading 1 or 0, 
;the program will either print out x5000 or x5001. The way how this works is by setting R5 load the contents
;at either x5000 or x5001. For the case of R1 reading a 1, R5 would then be loaded with the memory at address x5001.
;For the other case of R1 reading a 0, R5 would be loaded with the memory at address x5000.
;Each time R1 is read, the column counter R4 will be decrementing by 1. As long as R4 is not zero, this will repeat. 
;The way how the program prints out is by using LDR to R0 from R5. This way then the R0 would have the information
;needed to print out to console once detecting if the memory at that address is a 1 or 0. 
;As R4 hits zero, it will indicate that the program has finished reading a row and move to label ROWCOUNTER.
;R5 this time will be loaded memory at address called NEWLINE which has the value x0A. Same way as earlier,
;using LDR allows the program to print out a new line in the console. As soon as a new row is created,
;the row counter decrements by 1 and the whole process begins again when PC moves back to CHECKSPACE
;as long as the row counter remains positive. As soon as the row counter register hits zero,
;the branch statement will take the program to a Halt, indicating that the printing of the letter has finished. 

		.ORIG x3000		;Initializing at x3000
;initializing all registers
		AND R1,R1,#0		;register for memory at that letter address 
		AND R2,R2,#0		;register to be used as a decrementer
		AND R3,R3,#0		;R3 to be used as counter with value 16
		AND R4,R4,#0		;R4 to be used as counter with value 8
		AND R5,R5,#0		;register used for determining to print 1 or 0
		AND R6,R6,#0		;address location of FONT_DATA
		AND R0,R0,#0		;register used for OUTS

;Finding the offset to the desired letter
		LEA  R6,FONT_DATA	;address of FONT_DATA
		
		LDI  R1,LETTER		;Loading Hex value of desired letter into R1, the offset in ASCII table

		ADD R1,R1,R1		;Doubling Hex value
		ADD R1,R1,R1		;Multiplying by 4 on Hex value
		ADD R1,R1,R1		;Multiplying by 8 on Hex value
		ADD R1,R1,R1		;Mulitplying by 16 on Hex value

		ADD R6,R1,R6		;adding to the first address of FONT_DATA

		AND R1,R1,#0		;clearing for next operation, pointer of current space of the row
		AND R2,R2,#0		;clearing for next operation, what it prints out based on R1
		AND R0,R0,#0		;clearing for next operation//going to be used for OUT
		AND R3,R3,#0		;clearing for next operation

;pointing to the starting address of the specifc ASCII letter using offset value in R5	
		
		LD  R2,CDOWN		;loading in -1 into R2
		AND R5,R5,#0		;clear R5

;Checking if each space in a row has 1 or 0, this will loop 8 times per row
		ADD R4,R4,#8		;R4 for column counter
		ADD R3,R4,#8		;R3 for row counter of 16		
		
CHECKSPACE	LDR R1,R6,#0		;load memory of starting address from register R6 to R1
CHECKR1		ADD R1,R1,#0		;setting CC on R1
		BRn OUTONE		;print if R1 is negative

OUTZERO	 	LD  R5,SETZERO		;getting contents out of x5000
		LDR  R0,R5,#0		;having contents move to R0 to print out to console
		OUT			;prints out on the console
		AND R5,R5,#0		;clearing R5		
		BRnzp INNERLOOP
		
OUTONE		LD R5,SETONE		;getting contents out of x5001
		LDR R0,R5,#0		;having contents move to R0 to print out to console
		OUT			;prints out on the console
		AND R5,R5,#0		;clearing R5
		BRnzp INNERLOOP		;prepares to move to next space

INNERLOOP	ADD R1,R1,R1		;Left shift R1
		ADD R4,R4,R2		;Decrement column counter
		BRp CHECKR1		;Moving to next space

;after going through entire row of spaces
ROWCOUNTER	LD R5,NEWLINE		;NEWLINE into R5 
		AND R0,R0,#0		;clearing R0
		ADD R0,R0,R5		;moving into new line for next row
		OUT			;Moving to next row using newline by printing
		AND R5,R5,#0		;clear R5
		ADD R3,R3,R2		;Decrement row counter
		BRnz FINISH		;stop program after running 16 rows

		LDR R1,R6,#0		;resetting for new row since R1 is the memory of address at new row
		AND R4,R4,#0		;resetting column counter
		ADD R4,R4,#8		;Restarting column counter
		ADD R6,R6,#1		;incrementing current address to next address
		BRp CHECKSPACE		;Analyzing each space of new row


FINISH		TRAP x25		;HALT

;Adding appropriate assembly code and its respective pseudo-ops;
CDOWN		.FILL xFFFF
NEWLINE		.FILL x000A

SETZERO		.FILL x5000
SETONE		.FILL x5001
LETTER		.FILL x5002
FD		.FILL FONT_DATA
