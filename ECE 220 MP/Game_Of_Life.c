//MP Group: Akhil Nallacheruvu(analla6), Kevin Kim(sangwoo2)

/*
 * In this program, we are defining 3 functions: countLiveNeighbor, updateBoard, and aliveStable. countLiveNeighbor takes a pointer pointing at a 2D array, its row and column size, and the row and column
 * numbers and checks if there are any alive neighbors next to the cell in that position and returns the number of alive neighbors. This value is used to update the values on the board in the function
 * updateBoard. When given the pointer, row size, and column size, updateBoard calls countLiveNeighbor and updates the values based on the rules of the Game of Life. aliveStable checks if the updated board is 
 * the same as the old board. If it is, then it returns 1. Otherwise, 0 is returned.
 */

/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

#include<stdio.h>
#include<stdlib.h>
int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){

int live=0;		//initial number of live neighbors
for(int i=row-1; i<=row+1; i++)		
{
	for(int j=col-1; j<=col+1; j++)
        {
           if((i<=boardRowSize-1)&&(i>=0))
           {
		if((j<=boardColSize-1)&&(j>=0))			//if the row and column are within bounds
		{
			if(!((i==row)&&(j==col)))		//if the row and column going through the loop aren't equal to the current row and column
			{
				if(board[i*boardColSize+j]==1)
				{
					live=live+1;		//increments the number of alive neighbors if all of these conditions are met
				}
			}
		}
           }
         
        }
}

return live;
}
/*
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
int num_elements=boardRowSize*boardColSize;
int newBoard[num_elements]; //create an empty array that's the same size as board


for(int i=0;i<=boardRowSize-1;i++)
{
	for(int j=0;j<=boardColSize-1;j++)
	{
		if(board[i*boardColSize+j]==0);    // conditions for if the cell is dead
		{
			if((countLiveNeighbor(board, boardRowSize, boardColSize, i,j))==3)   //if dead cell has 3 neighbors the value at that point becomes 1. otherwise it is 0
			{
				newBoard[i*boardColSize+j]=1;
			}
			else
			{
				newBoard[i*boardColSize+j]=0;
			}
		}
		if(board[i*boardColSize+j]==1)  //conditions for if the cell is alive
		{
			if(((countLiveNeighbor(board, boardRowSize, boardColSize, i,j))==2)||((countLiveNeighbor(board, boardRowSize, boardColSize, i,j))==3))	   
			{
				newBoard[i*boardColSize+j]=1;
			}
			else
			{
				newBoard[i*boardColSize+j]=0;		//if the number of alive neighbors is 2 or 3, the value at the point is 1. otherwise it is 0
			}
		}
		
			
		
	}
}


for(int i=0; i<=boardRowSize-1; i++)		//deep copying the values in the new array onto the old one to update the board
{
	for(int j=0;j<=boardColSize-1;j++)
	{
		board[i*boardColSize+j]=newBoard[i*boardColSize+j];		//using 1d array notation to access elements in a 2d array
	}
}
}
/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */ 
int aliveStable(int* board, int boardRowSize, int boardColSize){
int input1;     //stores the original board
int input2;	//stores the updated board
int input3;	//differences of the two boards 
int newboard[boardRowSize*boardColSize];   //array that will contain the updated board

for(int i=0;i<boardRowSize;i++)			//creates a deep copy of the original board and stores it in newboard
{
	for(int j=0;j<boardColSize;j++)
	{
		newboard[i*boardColSize+j]=board[i*boardColSize+j];      //using 1d array notation to access elements in a 2d array
	}
}

//updateBoard function is being called again to update the value of newboard

for(int i=0;i<=boardRowSize-1;i++)
{
	for(int j=0;j<=boardColSize-1;j++)
	{
		if(board[i*boardColSize+j]==0);   				//case for dead cell
		{
			if((countLiveNeighbor(board, boardRowSize, boardColSize, i,j))==3)
			{
				newboard[i*boardColSize+j]=1;			//using 1d array notation to set a specific element in 2D array as alive
			}
			else
			{
				newboard[i*boardColSize+j]=0;			//using 1d array notation to set a specific element in 2D array as dead
			}
		}
		if(board[i*boardColSize+j]==1)  				//case for alive cell
		{
			if(((countLiveNeighbor(board, boardRowSize, boardColSize, i,j))==2)||((countLiveNeighbor(board, boardRowSize, boardColSize, i,j))==3))
			{
				newboard[i*boardColSize+j]=1;			//using 1d array notation to set a specific element in 2D array as alive
			}
			else
			{
				newboard[i*boardColSize+j]=0;			//using 1d array notation to set a specific element in 2D array as dead
			}
		}
		
			
		
	}
}



for(int i=0;i<boardRowSize;i++)
{
	for(int j=0;j<boardColSize;j++)
	{
		input1=board[i*boardColSize+j];
		input2=newboard[i*boardColSize+j];
		input3=input1-input2;
		if(input3!=0)			//if the boards aren't the same after updateBoard is called, return 0. Otherwise, return 1.
		{
			return 0;		//returns 0 if change happenend
		}
	}
}						//returns 1 if no change happened 
return 1;
}
