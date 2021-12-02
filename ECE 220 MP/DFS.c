#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
//Group: Akhil Nallacheruvu(analla6), Kevin Kim(sangwoo2)


/* The purpose of this code is to solve a maze using dynamic memory allocation and Depth First Search algorithm. It has 
 * four functions: createMaze, destroyMaze, printMaze, and solveMazeDFS. The purpose of createMaze is to intialize a maze
 * given a maze file. It dynamically allocates a pointer to the memory location of the file and returns a filled maze structure
 * that represents the contents of the input file. destroyMaze frees all the memory associated with the maze structure and returns 
 * the file to its intial position using the pointer from createMaze. printMaze prints out the maze onto the file and solveMazeDFS uses 
 * a depth first search algorithm to test each direction that the maze can go in. It finds the correct direction of the maze by going 
 * in a direction until it fails and using recursion and backtracking to change direction of travel and find the correct path out of the 
 * maze while keeping track of the previously traveled paths and the walls.
 */


/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    
    int i;   //integer used for rows
    int j;  //integers used for columns

    char newLine;   //variable used to read characters from file and separate the new line characters

    //allocating memory of type maze_t that contains all its components
    maze_t *maze_pointer = (maze_t*)malloc(sizeof(maze_t));
    //if the pointer is null, then print "malloc failed" and return the value that the pointer is pointing to
    if (maze_pointer == NULL) {
        printf("malloc failed");
        return NULL;
    }

    //assigning a FILE pointer name maze_file
    FILE* maze_file;      
    //reads and open filename - character array
    maze_file = fopen(fileName, "r");
    //if the pointer is null, then return 0 and print "empty file"
    if (maze_file == NULL) {
        printf("empty file.");
        return 0;
    }
    //getting the dimensions of height and width from fileName
    fscanf(maze_file, "%d, %d", &(maze_pointer->height), &(maze_pointer->width));

    //using char type to allocate array for maze, allocating for rows
    maze_pointer->cells = (char**)malloc((maze_pointer-> height)*sizeof(char*));
    //for every row array, allocating each row with respective size of columns

    for (i = 0; i<(maze_pointer->height); i++) {
        //making cells for array
        maze_pointer->cells[i] = (char*)malloc((maze_pointer->width)*sizeof(char));
    }
    fscanf(maze_file, "%d %d", &(maze_pointer->width), &(maze_pointer->height));
    //setting dimensions of the maze - starting position
    for (i = 0; i<(maze_pointer->height); i++) {
        for (j=0; j<(maze_pointer->width); j++) {
            
            //newline detection
            newLine=fgetc(maze_file);
            if(newLine=='\n'){
                j--;
                continue;
            }
            maze_pointer->cells[i][j]=newLine;
            

            //detecting if the cell has START
            if (maze_pointer->cells[i][j] == START) {
                maze_pointer->startRow = i;
                maze_pointer->startColumn = j;
            }

            //detecting if cell has END
            if (maze_pointer->cells[i][j] == END) {
                maze_pointer->endRow = i;
                maze_pointer->endColumn = j;
            }
        }

    }

    //closing the file
    fclose(maze_file);
    //returning the main maze_pointer
    return maze_pointer;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    //freeing starting from each column 
    int i;
    for(i=0; i<(maze->height); i++) {
        free(maze->cells[i]);
    }
    //freeing each row
    free(maze->cells);

    //Freeing memory of the OG pointer
    free(maze);
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    //printing out each cell of the maze
    int i;
    int j;
    for(i=0; i<=(maze->height); i++){
        for(j=0; j<=(maze->width); j++){
            printf("%c", maze->cells[i][j]);    //print out each character in the cells of every row and column 
        }
        printf("\n");       //print new lines after every row
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.

    //base cases:
    //current cell is out of bounds
    if ((col<0) || (row<0) || (col >= maze->width) || (row >= maze->height)){
        return 0;       //return false
    }

    //current cell is not an empty cell
    if (((maze->cells[row][col]) != EMPTY) || ((maze->cells[row][col]) != START)) {
        return 0;       //return false
    }

    //current cell is at the end of maze
    if ((maze->cells[row][col]) == END) {
        return 1;       //return true or game is set
    }

    //setting as path
    maze->cells[row][col] = PATH;
    
    // setting START location
    if ((row == maze->startRow) && (col == maze->startColumn)) {
        maze->cells[row][col] = START;
    }
    //recursive case, checking through each cell 
    
    //checking left
    if (solveMazeDFS(maze, col-1, row) == 1) {
        return 1;
    }

    //checking right
    if (solveMazeDFS(maze, col+1, row) == 1) {
        return 1;
    }

    //checking down
    if (solveMazeDFS(maze, col, row-1) == 1) {
        return 1;
    }

    //checking up
    if (solveMazeDFS(maze, col, row+1) == 1) {
        return 1;
    }

    //unmarking
    maze->cells[row][col] = VISITED;

    // reset game
    if ((row == maze->startRow) && (col == maze->startColumn)) {
        maze->cells[row][col] = START;
    }
    return 0;
}
