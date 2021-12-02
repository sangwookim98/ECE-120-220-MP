/*      
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */


/*
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */







//MP partners: Kevin Kim (sangwoo2), Akhil Nallacheruvu(analla6)

/*
 * In prog5.c, we defined 3 functions set_seed, start_game, and make_guess. set_seed sets up a pseudorandom seed
 * for the game which the user has to guess. start_game uses the seed to generate random solution pool. make_guess 
 * compares the user's guesses to the solutions and calculates a score 1000 if the guess is right and 100 if the guesses are mismatched. 
 */

#include <stdio.h>
#include <stdlib.h>
#include "prog5.h"

int guess_number;
int max_score;
char solutions[4][10];		 			//4 strings of length 10 (9 characters + 1 NULL)
char* pool[] = {"Vader", "Padme", "R2-D2", "C-3PO", "Jabba", "Dooku", "Lando", "Snoke",};
/* Show guessing pool -- This function prints "pool" to stdout
 * INPUT: none
 * OUTPUT: none
 */
void print_pool() {
    printf("Valid term to guess:\n\t");
    for(int i = 0; i < 8 ; ++i) {
        printf("%s ", pool[i]);
    }
    printf("\n");
}

/*
 * is_valid -- This function checks whether a string is valid
 * INPUTS: str -- a string to be checked
 * OUTPUTS: none
 * RETURN VALUE: 0 if str is invalid, or 1 if str is valid
 * SIDE EFFECTS: none
 */
int is_valid(char* str) {
    int i = 0;
    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        if (strcmp(str, pool[i]) == 0) {
            return 1;
        }
    }
    return 0;
}


/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the 
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid. 
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */
int set_seed (const char seed_str[]) {
//    Example of how to use sscanf to read a single integer and check for anything other than the integer
//    "int seed" will contain the number typed by the user (if any) and the string "post" will contain anything after the integer
//    The user should enter only an integer, and nothing else, so we will check that only "seed" is read. 
//    We declare
//    int seed;
//    char post[2];
//    The sscanf statement below reads the integer into seed. 
//    sscanf (seed_str, "%d%1s", &seed, post)
//    If there is no integer, seed will not be set. If something else is read after the integer, it will go into "post".
//    The return value of sscanf indicates the number of items read succesfully.
//    When the user has typed in only an integer, only 1 item should be read sucessfully. 
//    Check that the return value is 1 to ensure the user enters only an integer. 
//    Feel free to uncomment these statements, modify them, or delete these comments as necessary. 
//    You may need to change the return statement below



    int seed;      //create null string for no input condition?
    char post[2];
    if (sscanf (seed_str, "%d%1s", &seed, post) != 1) {
      // your code here
	printf("Set seed: invalid seed \n");
	return 0;     
    }
    // your code here
    else
	{
	   srand(seed);
	   return 1; 
	}
}

/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above. 
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 *               The score should be initialized to -1.  
 * INPUTS: none
 * OUTPUTS: none
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void start_game () {
    //your code here
	int i;
	for (i=0; i<4; i++){					//setting up solution array 
		int randNum;		
		randNum = (rand()%8);
		strcpy (solutions[i],pool[randNum]);	//copies from pool of 4 random guess to solution array
	}
	guess_number = 1;						//static variable
	max_score = -1;							//static variable
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str. 
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 strings from pool). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: none
 * RETURN VALUE: 2 if guess string is valid and got all 4 perfect matches, or 1 if the guess string is valid, 
 *               or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess) 
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int make_guess (const char guess_str[]) {
 	// your code here

	//Initializing arrays and variables
	char userGuess [4][10];				//user's guess of same 2-D array type as solution						
	char garbage [2];					//excess
 	
	//Initializing variables needed for score	
	int perfectmatch = 0;
	int mismatch = 0;
	int userScore = 0;
	int matchInput [4];
	int result;
	//check for valid number of strings
	if (sscanf (guess_str, "%s%s%s%s%1s", userGuess[0], userGuess[1], userGuess[2], userGuess[3], garbage) != 4) {
		//exceeds four strings = invalid guess
		printf("make_guess: invalid guess\n");
		return 0;
	}

	//using is_valid function
	for (int i=0; i<4; i++) {
		if (is_valid(userGuess[i]) == 0) {
			printf("make_guess: invalid guess\n");
			return 0;
		}
	}

	//Setting up user input and solution array comparison

	//checking for number of correct guesses
	for(int i = 0; i <4; i++) {
	//for each element correctly guessed
		result = strcmp(userGuess[i], solutions[i]);
		if (result == 0) {
			matchInput[i] = 1;
			perfectmatch = perfectmatch + 1;
			userScore = userScore + 1000;
			}
		}
	for(int i = 0; i <4; i++) {
		for (int j = 0; j<4; j++) {
			if (strcmp(userGuess[i], solutions[j]) == 0 && matchInput[j] != 1){
				matchInput[j] = 1;
				mismatch = mismatch + 1;
				userScore = userScore + 100;
				}
											
			}			
		}

	//setting up score calculation for user
	if (max_score < userScore) {
		max_score = userScore;
	}

	printf("With guess %d, you got %d perfect matches and %d misplaced matches.\nYour score is %d and current max score is %d.\n", guess_number, perfectmatch, mismatch, userScore, max_score);
	guess_number = guess_number + 1;
	
	//conditions to finish game
	if (perfectmatch == 4) {
		return 2;					//following MP instructions
	}
	
	else {
		return 1;					//following MP instructions
	}
	
}
