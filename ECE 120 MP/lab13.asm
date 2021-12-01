; The table below represents an 8x16 font.  For each 8-bit extended ASCII
; character, the table uses 16 memory locations, each of which contains
; 8 bits (the high 8 bits, for your convenience) marking pixels in the
; line for that character.

;The way how this program works is first, one register is used to locate the first address of FONT_DATA
;and the other register to locate the first letter of the word to print. R6 is the register that stores
;the starting address of FONT_DATA and R2 will be the address of the first letter of the word. 
;Next, the offset is then calculated by having R1 as the register that holds the Hex value
;of the current letter it is reading starting from x5002. Since each letter in FONT_DATA has a dimension
;of 8 by 16, 16 being the height of each word, this information is then used to multiply 16
;to the Hex value of the current letter the program is reading. After multiplying by 16, this value in R1 
;is then added to R6, which has the starting address of FONT_DATA in order to locate the starting
;location of the letter the program is at. After that, R1 is cleared and will be used to load memory
;at the current location that R6 holds. R1 at this point will now be the register that is used to check 
;if the memory at that location after the adding of offset holds a 0 or an 1. Depending on the result,
;using the Branch opcode, if the value is 0, it will go to the PC labelled OUTZERO, and if the value is 1,
;the Branch instruction will make the PC move to OUTONE in order to print 1 in the console.
;The way how this information is printed is by having R5 load memory from the address at x5001 and x5000.
;As lab13 stated, the symbol to print if the bit is 1 in the FONT_DATA is at x5001, and x5000 has the content
;if the bit in FONT_DATA is 0. This process of printing 1's and 0's repeat 8 times per letter.
;R4 will be used as a counter set at 8. Each time a memory at location in FONT_DATA is read, it will decrement by 1.
;Using a Branch statement, the program moves back to printing out after reading if the bit is 1 or 0
;as long as R4 is positive. As soon as R4 hits negative, it will indicate that it has read the first
;row of the first character of the word. Then it is time to move to the next letter of the word. The program does this
;by adding 1 to R2 so that it would move to x5003 for the 2nd letter of the word. After that, the program would then 
;have the PC move to the label called NEXTLETTER unconditionally by having a Branch statement set to that label. 

;It's going to be the same process repeated earlier - finding offset, printing symbols for 1 and 0 detected per space,
;all the things explained earlier for each letter at x5002 until R1 detects 0, the NULL case.
;The detection of 0 indicates that the first row of the entire word has been read.
;This means that the next row needs to be read. Once a Branch instruction reads zero in R1, it will 
;move to the Label named ROWCOUNTER in order to proceed to the next row. At this label,
;R5 reads the Newline information through LD which is then loaded into R0 to print out a 'new line'.
;After that, R2 needs to be resetted to x5002 since this is a new row and the process has to start over.
;R4 would be decremented by 1, but R3 will be incremented from 0. This program has R3 as the row counter,
;but instead of starting at 16, it starts at 0. This is set in this way because in order to find the 
;next row, this would mean that it would start after incrementing FONT_DATA address. 
;However, this would need to be incremented AFTER the addition of the offset (that is multiplied by 16).
;The way how the program set up the OFFSET calculation as well as incrementing the FONT_DATA address
;is the following: (FONT_DATA + (16*Offset value in hex))+Row Counter(R3)
;By doing this, the program does the two required steps in one instruction, under the label LETTERPOINTER.
;Also, the way how the row counter is "decremented" is to have R0, the temporary register, set to a fixed value of 16.
;The program uses the LD instruction to R0 with a value 16 (labelled ROWCNT). After that, the program sets R0
;for subtraction operation, R3-R0. R0 would then be negated and then added 1 to the negated R0. 
;This subtraction will be stored in R0. By using a branch statement, the whole process will be repeated until
;R0 becomes 0. R0 = 0 means that the program has finished reading 16 rows and the program would come to a halt. 
;If R0 isn't 0, the program resets R0 to 0, resets R4 to 8, and moves back to label FIRSTLETTER.
;This moves to a new row where each letter of the word would then be read from left to right until
;R1 reads NULL which then indicates reading of a new row. 

;table of registers
;R0 - temporary register for short calculations, mainly used for printing word out
;R1 - used for both Hex value of letter selected as well as memory at certain letter's address
;R2 - character counter to indicate which character it is reading from x5002
;R3 - row counter set at 0, program ends when it reaches 16
;R4 - column counter, used for printing out character by character
;R5 - register used to print out 1 or 0, even for new lines too.
;R6 - address location of FONT_DATA including the offset
;R7 - not used at all. 

		.ORIG x3000		;Initializing at x3000
;initializing all registers by clearing all registers except R7
		AND R1,R1,#0		
		AND R2,R2,#0		
		AND R3,R3,#0		
		AND R4,R4,#0		
		AND R5,R5,#0		
		AND R6,R6,#0		
		AND R0,R0,#0		

;setting registers 
		ADD R4,R4,#8		;R4 for column counter

;Finding the offset to the desired letter


FIRSTLETTER	LD  R2,LETTER		;Loading location of first letter of the word to R2
		LDI R1,LETTER		;getting and storing hex value of LETTER to R1
		BRnzp FIRSTPOINTER	;moves on to finding the starting location of first letter		

NEXTLETTER	LDR R1,R2,#0		;character counter pointing to next letter
		BRz ROWCOUNTER		;going to new line if letter is a NULL case

FIRSTPOINTER	LEA  R6,FONT_DATA	;address of FONT_DATA
				
LETTERPOINTER	ADD R1,R1,R1		;Doubling Hex value
		ADD R1,R1,R1		;Multiplying by 4 on Hex value
		ADD R1,R1,R1		;Multiplying by 8 on Hex value
		ADD R1,R1,R1		;Mulitplying by 16 on Hex value

		ADD R6,R1,R6		;adding to the first address of FONT_DATA
		ADD R6,R6,R3		;incrementing to the next row through the addition of R3 the row counter

		AND R1,R1,#0		;clearing for next operation, pointer of current space of the row
		AND R0,R0,#0		;clearing for next operation//going to be used for OUT
		
CHECKSPACE	LDR R1,R6,#0		;load memory of starting address from register R6 to R1
CHECKR1		ADD R1,R1,#0		;setting CC on R1
		BRn OUTONE		;print if R1 is negative

OUTZERO	 	LD  R5,SETZERO		;getting contents out of x5000
		LDR  R0,R5,#0		;loading R5 into R0 for print
		OUT			;prints R0
		AND R5,R5,#0		;clearing R5		
		BRnzp INNERLOOP		;proceeds to next space/letter
		
OUTONE		LD R5,SETONE		;getting contents out of x5001
		LDR R0,R5,#0		;loads R5 into R0 for printing
		OUT			;prints R0 out
		AND R5,R5,#0		;clearing R5
		BRnzp INNERLOOP		;goes to next space/letter of the letter

INNERLOOP	ADD R1,R1,R1		;Left shift R1
		ADD R4,R4,#-1		;Decrement column counter
		BRp CHECKR1		;Moving to next space of the letter

;after going through 8 spaces per character or letter
CHECKFIRSTROW	ADD R2,R2,#1		;Increment character counter to next character (not incrementing to x5003 from x5002)
		ADD R4,R4,#8		;reset column counter for next character
		AND R6,R6,#0		;clears offset value for new letter offset value in Hex
		BRnzp NEXTLETTER	;back to repeating process of finding location for next word

;after setting to next character
ROWCOUNTER	LD R5,NEWLINE		;NEWLINE into R5 
		AND R0,R0,#0		;clearing R0
		ADD R0,R0,R5		;moving into new line for next row
		OUT			;Moving to next row using newline by printing
		AND R5,R5,#0		;clear R5
		AND R1,R1,#0		;clear R1
		AND R2,R2,#0		;resetting back to x5002

		ADD R3,R3,#1		;Increment row counter

		LD R0,ROWCNT		;Loading value of 16 to temporary register R0
		NOT R0,R0		;negation for subtraction 
		ADD R0,R0,#1		;adding one to negated R0
		ADD R0,R3,R0		;subtracting R3 with R0, storing result in R0
	
		BRz FINISH		;stop program after running 16 rows
		
		AND R0,R0,#0		;reset R0 after checking if row counter has not reached 16

		AND R4,R4,#0		;resetting column counter
		ADD R4,R4,#8		;Restarting column counter
		BRnzp FIRSTLETTER	;back to first letter of the word

FINISH		TRAP x25		;HALT


;Adding appropriate assembly code and pseudo-ops;
CDOWN		.FILL xFFFF
NEWLINE		.FILL x000A
ROWCNT		.FILL x0010

SETZERO		.FILL x5000
SETONE		.FILL x5001

LETTER		.FILL x5002
