#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SOLUTION_SIZE 1024


int base_case_index = 0;
int max_queen_count = 0;
int ** solution;
int solution_count = 0;

int board_size;
int n;
int k;
int l;
int w;

void print_chess_board(int chess_board[])
{
	for(int i=0;i<board_size;i++)
	{
		if(chess_board[i])
			printf("%d, ", i);
	}
	printf("\n");
}
int get_queen_count(int chess_board[])
{
	int queen_count = 0;

	for(int i =0;i < board_size;i++)
	{
		if(chess_board[i])
			queen_count++;
	}

	return queen_count;
}
//Add array of position to chess board
void add_to_base_case(int data[], int r, int **base_cases)
{
	for (int i = 0; i < r; i++)
	{
		//add queen to chess board
		base_cases[base_case_index][data[i]] = 1;
	}
	base_case_index++;
}

//Choose K+1 number from n^2, once a combination is generated, add it to chess board
void combinationUtil(int arr[], int data[], int start, int end, int index, int r, int **base_cases)
{
    // Current combination is ready to be printed, print it
    if (index == r)
    {
     	//add to base_cases
     	add_to_base_case(data, r, base_cases);   
        return;
    }
 
    // replace index with all possible elements. The condition
    // "end-i+1 >= r-index" makes sure that including one element
    // at index will make a combination with remaining elements
    // at remaining positions
    for (int i=start; i<=end && end-i+1 >= r-index; i++)
    {
        data[index] = arr[i];
        combinationUtil(arr, data, i+1, end, index+1, r, base_cases);
    }
}

void generate_base_case(int count, int **base_cases)
{
	/*
		Initialize the base cases array, for each chess board position, 0 represent no queen, 1 represent have a queen
	*/
	for(int i =0;i<count;i++)
	{
		for(int j =0;j<board_size;j++)
		{
			base_cases[i][j] = 0;
		}
	}

	//arr represent a set of positions on board
	int arr[board_size];
	for(int i=0;i<board_size;i++)
	{
		arr[i] = i;
	}
	int data[k+1];
	combinationUtil(arr, data, 0, board_size-1, 0, k+1, base_cases);

}

//Check if the position i have a queen
int is_position_empty(int chess_board[], int i)
{
	return !chess_board[i];
}
int min(int a, int b)
{
	if(a>b)
		return b;
	else
		return a;
}
int get_attack_count(int i, int n, int chess_board[])
{
	int attack_count = 0;
	int curr = i;
	int curr_col = i%n;
	int curr_row = i/n;


	/* check in eight directions whether it attacks any queens
      w = 0 or 1 does not affect check diagonally
    */

    // to top-left direction
    int j;
    for(j = 1 ; j <= min(curr_col, curr_row); j++)
    {
    	if(!is_position_empty(chess_board, curr-j*(n+1)))
    	{
    		attack_count++;
            break; 
    	}
    }
            

    // to top-right direction

    for(j = 1; j <= min(n - curr_col - 1, curr_row); j++)
    {
		if(!is_position_empty(chess_board, curr-j*(n-1)))
		{
			attack_count++;
			break;
		}
    }

    // to bottom-left direction

    for(j = 1; j <= min(curr_col, n-curr_row-1); j++)
    {
    	if(!is_position_empty(chess_board, curr+j*(n-1)))
    	{
    		attack_count++;
            break; 
    	}
    }     
                                                
 
     // to bottom-right direction

    for(j = 1; j <= min(n-curr_col-1, n-curr_row-1); j++)
    {
    	if(!is_position_empty(chess_board, curr+j*(n+1)))
    	{
    		attack_count++;
            break;
    	}
    }
         
              

    // to up direction with w = 0
    if (w == 0)
    {
    	for(j = 1; j <= curr_row; j++)
	        if(!is_position_empty(chess_board, curr-n*j))
	        {
	        	attack_count++;
	            break;
	        }
	              

	    // to down direction with w = 0

	    for(j = 1; j <= n-curr_row-1; j++)
	        if(!is_position_empty(chess_board, curr+n*j))
	        {
	        	attack_count++;
	            break;
	        }
	             

	    // to left direction with w = 0

	    for(j = 1; j <= curr_col; j++)
	        if(!is_position_empty(chess_board, curr-j))
	        {
	        	attack_count++;
	            break;	
	        }
	            

	     // to right direction with w = 0

	    for(j = 1; j <= n-curr_col-1; j++)
	        if(!is_position_empty(chess_board, curr+j))
	        {
	       		attack_count++;
	            break;
	        }
    }
    else
    {
    	// to top direction
    	int stop_position;
    	for(j = 1; j < n; j++)
    	{
    		int check_position = curr-n*j;
    		if(check_position<0)
    			check_position+=board_size;
    		if(!is_position_empty(chess_board, check_position))
	        {
	        	attack_count++;
	        	stop_position = check_position;
	            break;
	        }
    	}

    	// to bottom position
    	for(j = 1; j < n; j++)
    	{
    		int check_position = curr+n*j;
    		if(check_position >= board_size)
    			check_position -= board_size;
    		if(check_position == stop_position)
    			break;
    		if(!is_position_empty(chess_board, check_position))
	        {
	        	attack_count++;
	        	stop_position = check_position;
	            break;
	        }
    	}

    	//to left direction
    	for(j = 1; j < n; j++)
    	{
    		int check_position = curr-j;
    		if(check_position<curr_row*n)
    			check_position+=n;
    		if(!is_position_empty(chess_board, check_position))
	        {
	        	attack_count++;
	        	stop_position = check_position;
	            break;
	        }
    	}

    	//to right direction
    	for(j = 1; j < n; j++)
    	{
    		int check_position = curr+j;
    		if(check_position>=(curr_row+1)*n)
    			check_position -= n;
    		if(check_position == stop_position)
    			break;
    		if(!is_position_empty(chess_board, check_position))
	        {
	        	attack_count++;
	        	stop_position = check_position;
	            break;
	        }
    	}

    }
    

    return attack_count;
}

int is_safe_attack(int old_chess_board[], int new_queen_position)
{
	int * chess_board = malloc(sizeof(int) * board_size);;
	memcpy(chess_board, old_chess_board, board_size * sizeof(int));
	chess_board[new_queen_position] = 1;
	for(int i=0;i<board_size;i++){
		if(!is_position_empty(chess_board, i))
		{
			
			int attack_count = get_attack_count(i, n, chess_board);

		     // check whether it is safe
		     
		    if (attack_count > k)
		        return 0;
		}
	}
	return 1;
}

int is_valid_solution(int chess_board[])
{
	for(int i=0;i<board_size;i++){
		if(!is_position_empty(chess_board, i))
		{
			
			int attack_count = get_attack_count(i, n, chess_board);

		     // check whether it is safe
		    if (attack_count != k)
		        return 0;
		}
	}
	return 1;
}

//add the queen on position i, return a new chess board
int * add_queen(int chess_board[], int i)
{
	//create a new exact same array
	int * new_chess_board = malloc(sizeof(int) * board_size);;
	memcpy(new_chess_board, chess_board, board_size * sizeof(int));
	

	//add queen to new chess board
	new_chess_board[i] = 1;
	return new_chess_board;
}

int compare_chess_board(int board1[], int board2[])
{
	for(int i=0;i<board_size;i++)
	{
		if(board1[i]!=board2[i])
			return 0;
	}
	return 1;
}

void free_solution()
{
	for(int i =0;i<MAX_SOLUTION_SIZE;i++)
	{
		free(solution[i]);
	}
}
//if the chess board (which is a valid solutiin) has more than current maximum queen count, refresh solution
//if the chess board is the same as current queen count, add to solution
//else discard
void add_to_solution(int * chess_board)
{
	int queen_count = get_queen_count(chess_board);

	if(queen_count > max_queen_count)
	{
		//clean solution, add this to it
		//free_solution();
		solution_count = 0;
		max_queen_count = queen_count;
		solution[solution_count] = chess_board;
		solution_count++;
	}
	else if(queen_count == max_queen_count)
	{
		//add to set
		if (solution_count<MAX_SOLUTION_SIZE)
		{
			for(int i=0;i<solution_count;i++)
			{
				if(compare_chess_board(solution[i], chess_board) == 1)
					return;
			}
			solution[solution_count] = chess_board;
			solution_count++;			
		}

	}
	else
	{
		free(chess_board);
		return;
	}
		
}

/*
	solve the cases
*/
void solve(int chess_board[], int current_position) 
{
	int is_end_case = 1;
	for(int i = current_position;i < board_size;i++)
	{
		if(is_position_empty(chess_board, i) && is_safe_attack(chess_board, i))
		{
			int * new_chess_board;
			new_chess_board = add_queen(chess_board, i);
			solve(new_chess_board, i+1);
			is_end_case = 0;
			// break;
		}
	}

	if(is_end_case)
	{
		if(is_valid_solution(chess_board))
		{

			add_to_solution(chess_board);
		}
		else
			return;
	}
}



void print_solution(int board_size)
{
	printf("%d:\n", max_queen_count);

	if(l)
		for(int i = 0; i < solution_count;i++)
		{
			print_chess_board(solution[i]);
		}
}


//helper function for calculating combination
int nCr(int n, int r) {
   if(r>n) {printf("FATAL ERROR"); return 0;}       
   if(n==0 || r==0 || n==r) {
      return 1;
   } else {
      return (int)lround( ((double)n/(double)(n-r)/(double)r) * exp(lgamma(n) - lgamma(n-r) - lgamma(r)));
   }
}
int main(int argc, char **argv)
{

	if(argc != 5)
	{
		printf("Invalid Number of Arguments.\n");
		return 0;
	}
	
	n = atoi(argv[1]);
	k = atoi(argv[2]);
	l = atoi(argv[3]);
	w = atoi(argv[4]);

	//printf("%d %d %d %d\n", n,k,l,w);
	//initialize base cases
	//we use 1d array to represent the chess board, from 0 to N^2 - 1
	board_size = n*n;
	int case_count = nCr(board_size,k+1);
	printf("%d,%d:", n, k);
	
	/*
		Initialize storage
	*/
	int **base_cases;
	base_cases = (int **)malloc(sizeof(int *)*case_count);
	for(int i=0; i < case_count; i++) {
		base_cases[i] = (int *)malloc(sizeof(int)*board_size);
	}


	solution = (int **)malloc(sizeof(int *)*MAX_SOLUTION_SIZE);
	for(int i=0; i < MAX_SOLUTION_SIZE; i++) {
		solution[i] = (int *)malloc(sizeof(int)*board_size);
	}

	/*
		Start to solve the problem
	*/

	generate_base_case(case_count, base_cases);

	for(int i =0;i<case_count;i++)
	{
		solve(base_cases[i], 0);
	}

	print_solution(board_size);
	return 0;
}
