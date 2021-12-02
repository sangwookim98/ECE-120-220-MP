#include "game.h"
//Group Members: Akhil Nallacheruvu(analla6), Kevin Kim(sangwoo2)

/* The purpose of this code is to create the game 2048 which arranges tiles that are a multiple of 2 to form a sum of 2048.
 * The functions in this code are: make_game, remake_game, get_cell, move_w,move_s,move_a, move_d, and legal_move_check.
 *  make_game initializes the correct initial values to rows, cols, and score. remake_game reinitializes the game to be used again.
 * get_cell returns a pointer to the corresponding cell in the game and returns NULL if there is no cell. move_w moves up by using
 * the functions move_up, which slides the blocks up and merge_up, which merges 2 like adjacent blocks upwards. move_s moves down by using
 * the functions move_down, which slides the blocks downwards and merge_down, which merges 2 like adjacent blocks downwards. move_a moves
 * to the left by using the functions move_left, which slides the blocks to the left and merge_left which merges 2 like adjacent blocks
 * in the left direction. move_d moves to the right by using the functions move_right which slides the blocks to the right and merge_right
 * which merges 2 like adjacent blocks to the right.
 */

game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
	mygame->cols=cols;
	mygame->rows=rows;
    for(int i=0;i<rows;i++)
    {
		for(int j=0;j<cols;j++)
		{
			mygame->cells[i*cols+j]=-1;
		}
    }
	mygame->score=0;
	return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	//YOUR CODE STARTS HERE:  Re-initializing all other variables in game struct
	(*_cur_game_ptr) -> rows = new_rows;
	(*_cur_game_ptr) -> cols = new_cols;
	
	for (int i = 0; i<new_rows; i++) {
		for (int j = 0; j<new_cols; j++) {
			//access cells individually... using 1D Array notation at the dereferencing pointer _cur_game_ptr
			(*_cur_game_ptr) -> cells[(i*new_cols)+j] = -1;
		}
	}
	
	(*_cur_game_ptr) -> score = 0; 			//initializing score to 0	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
	//checking if cell is within the bounds of dimensions given with row and column
	if ((row<cur_game -> rows && row>=0) && (col < cur_game -> cols && col >=0)){
	
		//cell is within the bounds given by row and col parameters
		cell *point;
		point=&(cur_game->cells[row*(cur_game->cols)+col]);

		return point;
		
	}
	
    return NULL;
}

//making a move up function with cur_game as the argument to pass
int move_up(game * cur_game) {
    //algorithm followed based on lab 8
    //return a change variable
    int flag = 0;
	for (int i = 0; i<(cur_game->rows)	; i++) {                                        //checks through each row
		for (int j = 0; j<cur_game -> cols; j++) {                                      //checks through each column 
			if (cur_game->cells[i*(cur_game->cols)+j] != -1) {                          //checks if current cell is not empy
				//using k as a target row, sliding values up when detected an empty cell 
				for (int k = 0; k<i; k++) {
					if(cur_game -> cells[k*(cur_game->cols) + j] == -1){										//finds an empty space in target row
						cur_game -> cells[k*(cur_game->cols) + j] = cur_game->cells[i*(cur_game->cols)+j];		//swaps empty space in target row with selected current row
						cur_game->cells[i*(cur_game->cols)+j] = -1;								    			//sets space in current row with -1
                        flag = 1;
                        break;
				}	}	}
    
}
	}
	return flag;
}

//making a merge function for cells that have moved up
int merge_up (game * cur_game) {
    //checking for values above target row and at target row, add if cell above target row has the same value, add the two cells
	int flag=0;
    for (int i = 0; i<(cur_game->rows)	; i++) {                                        //checks through each row   
		for (int j = 0; j<cur_game -> cols; j++) {                                       //checks through each column                                        	    
                if ((cur_game -> cells[i*(cur_game->cols) + j] == cur_game->cells[(i+1)*(cur_game->cols)+j])&&(cur_game->cells[i*(cur_game->cols)+j]!=-1)) {
	                cur_game -> cells[i*(cur_game->cols) + j] = 2*(cur_game -> cells[i*(cur_game->cols)+j]);
                    cur_game->score+=cur_game->cells[(i+1)*(cur_game->cols)+j];
		            cur_game->cells[(i+1)*(cur_game->cols)+j] = -1; 
                    flag = 1;                           
                }
                
            
        }
    }
    return flag; 
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    //YOUR CODE STARTS HERE
	
	//first - moved all values up one.... 
	//second - add two cells if target row and row above has the same value, replace target row cell with -1
	//third - slide again to make sure in between spaces after the addition gets pushed
    //sequence of function : slide up -> merge -> slide up 
    int action_one = move_up(cur_game);
    int action_two = merge_up(cur_game);
    int action_three = move_up(cur_game);

    //check for valid or invalid move
    if (action_one || action_two || action_three == 1){
        return 1;           //valid move
    }
    else {
        return 0;           //invalid move
    }
};

//making a move up function with cur_game as the argument to pass
int move_down(game * cur_game) {
    //algorithm followed based on lab 8
    //return a change variable
    int flag = 0;
	for (int i = (cur_game->rows)-1; i>=0; i--) {                                           //checks through each row
		for (int j = 0; j<(cur_game->cols); j++) {                                          //checks through each column 
			if (cur_game->cells[i*(cur_game->cols)+j] != -1) {                              //checks if current cell is not empy
				//using k as a target row, sliding values up when detected an empty cell 
				for (int k = (cur_game->rows)-1; k>i; k--) {
					if((cur_game -> cells[k*(cur_game->cols) + j] == -1)){									//finds an empty space in target row
						cur_game -> cells[k*(cur_game->cols) + j] = cur_game->cells[i*(cur_game->cols)+j];		    //swaps empty space in target row with selected current row
						cur_game->cells[i*(cur_game->cols)+j] = -1;								    			    //sets space in current row with -1
                        flag = 1;
			        	}	
                    }	
                }
            }
	    }
	return flag;
}

//making a merge function for cells that have moved up
int merge_down (game * cur_game, int flag_state) {
    //checking for values above target row and at target row, add if cell above target row has the same value, add the two cells
	int flag=0;
    for (int i = (cur_game->rows); i>=0; i--) {                                         //checks through each row   
		for (int j = 0; j<cur_game ->cols; j++) {                                       //checks through each column                                        	    
                if ((cur_game -> cells[i*(cur_game->cols) + j] == cur_game->cells[(i-1)*(cur_game->cols)+j])&&(cur_game->cells[i*(cur_game->cols)+j]!=-1)) {
                    if(flag_state == 0){
                        cur_game -> cells[i*(cur_game->cols) + j] = 2*(cur_game -> cells[i*(cur_game->cols)+j]);
                        cur_game->score+=cur_game->cells[(i-1)*(cur_game->cols)+j];
		                cur_game->cells[(i-1)*(cur_game->cols)+j] = -1; 
                        flag = 1;
                    }
	                if (flag == 0){
                        flag = 1;
                        }                        
                    }
                }
            }
    return flag; 
}

int move_s(game * cur_game) //slide down
{
    //YOUR CODE STARTS HERE
    //same as move up function, except instead of going up, everything is going down 
    int action_one = move_down(cur_game);
    printf("%d\n",action_one);
    int action_two = merge_down(cur_game, 0);
    printf("%d\n",action_two);
    int action_three = move_down(cur_game);
    printf("%d\n",action_three);

    //check for valid or invalid move
    if (action_one && action_two && action_three == 0){
        return 0;           //valid move
    }
    else {
        return 1;           //invalid move
    }
}

int move_left(game * cur_game) {
    //algorithm followed based on lab 8
    //return a change variable
    int flag = 0;
	for (int i = 0; i<(cur_game->rows)	; i++) {                                        //checks through each row
		for (int j = 0; j<(cur_game -> cols); j++) {                                      //checks through each column 
			if (cur_game->cells[i*(cur_game->cols)+j] != -1) {                          //checks if current cell is not empy
				//using k as a target row, sliding values left when detected an empty cell 
				for (int k = 0; k<j; k++) {
					if(cur_game -> cells[i*(cur_game->cols) + k] == -1){										//finds an empty space in target row
						cur_game -> cells[i*(cur_game->cols)+k] = cur_game->cells[i*(cur_game->cols)+j];		//swaps empty space in target row with selected current row
						cur_game->cells[i*(cur_game->cols)+j] = -1;								    			//sets space in current row with -1
                        flag = 1;
                        break;
				}	}	}
    
}
	}
	return flag;
}

//making a merge function for cells that have moved up
int merge_left (game * cur_game) {
    //checking for values above target row and at target row, add if cell above target row has the same value, add the two cells
	int flag=0;
    for (int i = 0; i<(cur_game->rows)	; i++) {                                        //checks through each row   
		for (int j = 0; j<(cur_game -> cols)-1; j++) {                                       //checks through each column                                        	    
                if ((cur_game -> cells[i*(cur_game->cols) + j] == cur_game->cells[i*(cur_game->cols)+(j+1)])&&(cur_game->cells[i*(cur_game->cols)+j]!=-1)) {
	                cur_game -> cells[i*(cur_game->cols) + j] = 2*(cur_game -> cells[i*(cur_game->cols)+j]);
                    cur_game->score+=cur_game->cells[i*(cur_game->cols)+(j+1)];
		            cur_game->cells[i*(cur_game->cols)+(j+1)] = -1;
                    flag = 1;                           
                }
                
            
        }
    }
    return flag; 
}

int move_a(game * cur_game) //slide left
{
    //YOUR CODE STARTS HERE
    int action_one = move_left(cur_game);
    int action_two = merge_left(cur_game);
    int action_three = move_left(cur_game);
    //check for valid or invalid move
    if (action_one || action_two || action_three == 1){
        return 1;           //valid move
    }
    else {
        return 0;           //invalid move
    }
}

int move_right(game * cur_game) {
    //algorithm followed based on lab 8
    //return a change variable
    int flag = 0;
	for (int i = 0; i<(cur_game->rows)	; i++) {                                        //checks through each row
		for (int j = (cur_game->cols)-1; j>=0; j--) {                                     //checks through each column 
			if (cur_game->cells[i*(cur_game->cols)+j] != -1) {                          //checks if current cell is not empy
				//using k as a target column, sliding values right when detected an empty cell 
				for (int k =(cur_game->cols)-1; k>j; k--) {
					if(cur_game -> cells[i*(cur_game->cols) + k] == -1){										//finds an empty space in target row
						cur_game -> cells[i*(cur_game->cols) + k] = cur_game->cells[i*(cur_game->cols)+j];		//swaps empty space in target row with selected current row
						cur_game->cells[i*(cur_game->cols)+j] = -1;								    			//sets space in current row with -1
                        flag = 1;
                        break;
                    }
                }
            }
        }
    }
    return flag;
}
		
    

	

//making a merge function for cells that have moved up
int merge_right (game * cur_game) {
    //checking for values above target row and at target row, add if cell above target row has the same value, add the two cells
	int flag=0;
    for (int i = 0; i<(cur_game->rows)	; i++) {                                        //checks through each row   
		for (int j = 1; j<cur_game -> cols; j++) {                                       //checks through each column                                        	    
                if ((cur_game -> cells[i*(cur_game->cols) + j] == cur_game->cells[i*(cur_game->cols)+(j-1)])&&(cur_game->cells[i*(cur_game->cols)+j]!=-1)) {
	                cur_game -> cells[i*(cur_game->cols) + j] = 2*(cur_game -> cells[i*(cur_game->cols)+j]);
                    cur_game->score+=cur_game->cells[i*(cur_game->cols)+(j-1)];
		            cur_game->cells[i*(cur_game->cols)+(j-1)] = -1; 
                    flag = 1;                           
                }
                
            
        }
    }
    return flag; 
}

int move_d(game * cur_game){ //slide to the right
    //YOUR CODE STARTS HERE
    int action_one = move_right(cur_game);
    int action_two = merge_right(cur_game);
    int action_three = move_right(cur_game);
    //check for valid or invalid move
    if (action_one || action_two || action_three == 1){
        return 1;           //valid move
    }
    else {
        return 0;           //invalid move
    }
}

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE

    for(int i=0;i<(cur_game->rows)*(cur_game->cols);i++)
    {
        if(cur_game->cells[i]==-1)
        {
            return 1;
        }
    }

	int move1=merge_up(cur_game);
    int move2=merge_down(cur_game,1);
    int move3=merge_right(cur_game);
    int move4=merge_left(cur_game);
    if(move1||move2||move3||move4)
    {
        return 1;
    }
    return 0;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
