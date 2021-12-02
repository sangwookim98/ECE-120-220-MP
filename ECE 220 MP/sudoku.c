#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

// You are free to declare any private functions if needed.



//MP Group: Akhil Nallacheruvu(analla6), Kevin Kim(sangwoo2)

/* Our code in this MP defines 5 functions: int is_val_in_row, int is_val_in_col,int is_val_in_3x3_zone, int is_val_valid, and int solve_sudoku.
   The purpose of these 5 functions is to create a sudoku solver for a 9x9 sudoku. is_val_in_row checks whether a value already exists in a row 
   and returns 1 if it does or 0 if it doesn't. is_val_in_col checks whether a value already exists in a column and returns 1 if it does and 0 if
   it doesn't. is_val_in_3x3_zone checks whether a value exists within the 3x3 zone and returns 1 if it does and 0 if it doesn't. is_val_valid makes
   use of the 3 aforementioned functions to determine whether or not a value can be inputted. 1 is returned if the value can be inputted. Otherwise,
   0 is returned. Finally, solve_sudoku uses is_val_valid and recursion and backtracking to fill in the sudoku with the correct values.
*/

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  for(int j=0;j<9;j++)		   //checks every column inside the row
  {
	if(sudoku[i][j]==val)	   //if value exists return 1
        {
		return 1;
	}
  }  
  return 0;			//if value doesn't already exist, return 0
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  for(int i=0;i<9;i++)		//checks every row inside the column
  {
	if(sudoku[i][j]==val)	//if value exists return 1
        {
		return 1;
	}
  }  
  
  return 0;			//if value doesn't already exist, return 0
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9);
  
  // BEG TODO
  int izone,jzone;   
  izone=(i/3)*3;      //determines the first row of the 3x3 zone given i
  jzone=(j/3)*3;      //determines the first column of the 3x3 zone given j
  for(int row=izone;row<=izone+2;row++)          //checks the rows in the 3x3 zone
  {
	for(int col=jzone;col<=jzone+2;col++)	 //checks the columns in the 3x3 zone
        {
		if(sudoku[row][col]==val)	//if the value exists in the 3x3 zone, return 1
                {
			return 1;
                }
	}
  }
  
  return 0;					//if the value doesn't exist in the 3x3 zone, return 0
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  //3 variables below simplify the condition calls
  int row=is_val_in_row(val,i,sudoku);       
  int col=is_val_in_col(val,j,sudoku);
  int zone=is_val_in_3x3_zone(val,i,j,sudoku);
  if((val>=1)&&(val<=9))  //if value is between 1 and 9 inclusive
  {
	if(row==1)	 //if the value exists in the row, return 0
        {
		return 0;
        }
	if(col==1)      //if the value exists in the column, return 0
	{
		return 0;
	}
	if(zone==1)	//if the value exists in the 3x3 zone, return 0
	{
		return 0;
	}
	return 1;	//if the value doesn't exist and can be inputted, return 1
  }


return 0;  //if the value is not between 1 and 9 inclusive, return 0





  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int flag=0;		//variable used to break out of the base case in order to preserve the row and column numbers
  int i,j;		//i is the row number, j is the column number
  //base case
  for(i=0;i<9;i++)	//checks the rows	
  {
	for(j=0;j<9;j++)	//checks the columns
	{
		if(sudoku[i][j]==0)  //if there is a nonfilled cell, set flag=1 and break out of the column checking for loop
		{
			flag=1;
			break;
		}
	}
	if(flag==1)    //if flag=1, break out of the row checking for loop
	{
		break;
	}
  }
  if(flag==0)  //if the value of flag never changes, return 1
  {
	return 1;
  }
  //recursive case
  for(int num=1;num<=9;num++)   //checking all the possible input values at the empty cell found in base case
  {
	if(is_val_valid(num, i,j,sudoku))   //if num is a valid value, set the cell value equal to num
	{
		sudoku[i][j]=num;
		if(solve_sudoku(sudoku))  //calls solve_sudoku to fill in the empty values until there are no more empty cells and if there are no more empty cells, return 1
		{
			return 1;
		}
		sudoku[i][j]=0;  //if a solution can't be found, keep the specific cell at 0 until another solution is found
	}
	
  }
	return 0;     //return 0 if the base cases and recursive cases are false and the sudoku can't be solved.
     //END TODO
}
  
  
// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
