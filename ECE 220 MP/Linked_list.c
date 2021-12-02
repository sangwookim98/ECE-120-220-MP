#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>
//Group: Akhil Nallacheruvu(analla6), Kevin Kim(sangwoo2)

/* The purpose of this program is to fill up a sparse in row-major order given a cell index. The
 * functions used in this program are matrix_index, create_node, remove_node,  get_node, load_tuples,
 * gv_tuples, set_tuples, save_tuples, add_tuples, and destroy_tuples. matrix_index takes the column 
 * size, row number, and column number as its parameters and reutrns the position of the cell in row
 * major order. create_node takes a pointer to the head pointer, the size of the columns, the row
 * number, column number, and value in the cell as parameters so that it creates a new node and
 * redirects the head pointer to point to the node. remove_node takes a pointer to the head pointer, 
 * the size of the columns, the row number, and the column number to free the node from the heap and 
 * redirects the head pointer to the new beginning node. get_node takes the head pointer, row number, 
 * and column number to select a node in the linked list, update the next pointer, and point to the 
 * next node. load_tuples takes an input file as its parameter and returns a pointer to the matrix. gv_tuples
 * takes a matrix, row number, and column number as its parameter, and returns the value of the element that 
 * corresponds to the given coordinates. set_tuples takes a matrix, row number, column, number and a value as 
 * its parameters to set the value of a matrix in a certain position to the corresponding value. save_tuples
 * takes a file and matrix as its parameters and saves the matrix to the text file. destroy_tuples takes a 
 * matrix as its parameter and frees any memory associated with the matrix.
 */


//getting index of matrix given
int matrix_index (int col_size, int row, int col) {
    return row*col_size+col;
}

//creating a function that inserts node at a particular position
void create_node (sp_tuples_node **head_pointer, int col_size, int row, int col, double value) {
    // 3 types of pointers needed - new_node, left_node, current_node
    sp_tuples_node *new_node;
    sp_tuples_node *left_node;
    sp_tuples_node *current_node;

    //getting the certain index of the matrix based on the node
    int node_index = matrix_index(col_size, row, col);

    // using malloc for creating nodes
    new_node = malloc(sizeof(sp_tuples_node));

    // error cases when creating new node - when new_node pointer has NULL
    if(new_node == NULL) {
        printf("No memory allocated for creating new node.");
        return;
    }

    // assigning values from sp_tuples_node struct
    new_node->col = col;
    new_node->row = row;
    new_node->value = value;
    new_node->next = NULL;

    // case when creating new node from the start
    left_node = NULL;
    current_node = *head_pointer;

    // finding the correct location to add new node in the linked list
    // checking if node doesn't have an empty pointer and index is greater than current cell's index
    while ((current_node != NULL) && (node_index > matrix_index(col_size, current_node->row, current_node->col))){
        // goes to the next node in list and checks again
        left_node = current_node;
        current_node = current_node->next;
    }

    // adding new node at the beginning
    if (left_node == NULL) {
        new_node->next = *head_pointer;
        *head_pointer = new_node;
    }
    // case when new node is added between the start and the current_node pointer
    else {
        left_node->next = new_node;
        new_node->next = current_node;
    }
}

void remove_node (sp_tuples_node **head_pointer, int col_size, int row, int col) {
    // similar set up as add_node, 3 pointers to nodes except with a temporary node pointer instead
    sp_tuples_node *current_node;
    sp_tuples_node *left_node;
    sp_tuples_node *temp_node;

    int given_index = matrix_index(col_size, row, col);

    // case where the first node of the list is removed
    if (given_index == matrix_index(col_size, (*head_pointer)->row, (*head_pointer)->col)) {
        // setting up by saving that pointer to temp_pointer first to prevent memory leak
        temp_node = *head_pointer;
        *head_pointer = (*head_pointer)->next;
        free(temp_node);
    }

    // if node to be removed is not the first node in the list
    else {
        // setting node to be the first node of the linked list
        left_node = *head_pointer;
        current_node = (*head_pointer)->next;

        // finding through the right node
        while (((current_node != NULL) && (given_index != matrix_index(col_size, current_node->row, current_node->col)))) {
            // updating by setting pointers to next node
            left_node = current_node;
            current_node = current_node->next;
        }

        // when found right node to delete
        if (current_node != NULL) {
            // using temp_node to prevent memory leak
            temp_node = current_node;
            left_node->next = current_node->next;
            free(temp_node);
        }
    }
}

sp_tuples_node * get_node(sp_tuples_node *head_pointer, int row, int col) {
    // searching through the linked list from the first node
    sp_tuples_node *current_node = head_pointer;
    while (current_node != NULL) {
        if ((current_node->row == row) && (current_node->col == col)) {
            return current_node;
        }
        current_node = current_node->next;
    }
    return NULL;
}

sp_tuples * load_tuples(char* input_file)
{
    // File I/O
    FILE* file=fopen(input_file, "r");

    // checking if file is empty
    if (file == NULL) {
        printf("File cannot open");
        return NULL;
    }

    // setting up memory allocation for *matrix
    sp_tuples * matrix=malloc(sizeof(sp_tuples));
    // checking for errors
    if(matrix == NULL) {
        printf("memory allocation error");
        return NULL;
    }

    // initializing values within sp_tuples struct
    matrix->m=0;
    matrix->n=0;
    matrix->nz=0;
    matrix->tuples_head=NULL;

    // initializing variables
    int col, row;
    double value;

    // reading row and column size of array
    fscanf(file, "%d %d\n", &matrix->m, &matrix->n);

    // reading through all values of the file until it reaches end
    while (feof(file) != 0) {
        set_tuples(matrix, row, col, value);
        
        fscanf(file, "%d %d %lf\n", &row, &col, &value);
    }
    // closing file after scanned
    fclose(file);

    //set the tuples by calling set_tuples
    return matrix;
}


double gv_tuples(sp_tuples * mat_t,int row,int col)
{
    sp_tuples_node * pointer= get_node(mat_t->tuples_head, row, col);
    if (pointer != NULL){
        return pointer->value;
    }
    return 0;
}


void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
    sp_tuples_node *set_node = get_node(mat_t->tuples_head, row, col);
    // case where same node is in the list
    if (set_node != NULL) {
        if (value !=0) {
            set_node->value = value;
        }
        else {
            // removing since value of the node is 0
            remove_node(&mat_t->tuples_head, mat_t->n, row, col);
            mat_t->nz--;
        }
    }
    // case where that node is unique
    else {
        if (value != 0) {
            create_node(&mat_t->tuples_head, mat_t->n, row, col, value);
            mat_t->nz++;
        }
    }
    return;
}



void save_tuples(char * file_name, sp_tuples * mat_t)
{
    // file to open
	FILE* file=fopen(file_name, "w");
    if (file == NULL) {
        printf("Error in opening file \n");
        return;
    }

    // writing to that file, dimensions of matrix
    fprintf(file, "%d %d\n", mat_t->m,mat_t->n);
    sp_tuples_node * pointer=mat_t->tuples_head;

    while(pointer!=NULL){
        fprintf(file, "%d %d %lf\n",pointer->row, pointer->col, pointer->value);
        // going to next node
        pointer=pointer->next;
    }
    fclose(file);
    return;
}


sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){

    // checking for dimensions of matA and matB
    if ((matA->m == matB->m) && (matA->n == matB->n)) {
        // memory allocation for sp_tuples
        sp_tuples * matC=malloc(sizeof(sp_tuples));

        // checking if matC is empty
        if (matC == NULL) {
            printf("Memory allocation error for matC");
            return NULL;
        }

        // setting up components 
        matC->m=matA->m;
        matC->n=matA->n;
        matC->nz=0;
        matC->tuples_head = NULL;

        // copy A to C first, then add B to C, return C
        sp_tuples_node * pointer = matA->tuples_head;
        while(pointer != NULL) {
            int row = pointer->row;
            int col = pointer->col;
            double valueMatA = pointer->value;
            set_tuples(matC, row, col, valueMatA);
            pointer = pointer->next;
        }

        pointer = matB->tuples_head;
        while(pointer != NULL){
            int row = pointer->row;
            int col = pointer->col;
            double valueMatB = pointer->value;
            // using gv_tuples to for Matrix C
            double valueMatC = gv_tuples(matC, row, col);
            set_tuples(matC, row, col, valueMatB + valueMatC);
            pointer = pointer->next;
        }
        return matC;
    }
    return NULL;
}


	
void destroy_tuples(sp_tuples * mat_t) {
    // setting up temporary pointer
    sp_tuples_node * temp_pointer=NULL;

    // setting up head_pointer to desginated node
    sp_tuples_node * head_pointer= mat_t->tuples_head;
    while(head_pointer != NULL) {
        head_pointer = temp_pointer;
        temp_pointer = head_pointer->next;
        free(head_pointer);
    }
    free(mat_t);
    return;
}  
