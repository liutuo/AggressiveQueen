#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <stddef.h>

#define MAX_SOLUTION_SIZE 1024
struct Solution
{
	int size;
	int max_queen_count;
	int ** solution;
};


int base_case_index = 0;

int board_size;
int n;
int k;
int l;
int w;

//helper function to print the chess board
void print_chess_board(int chess_board[])
{	
	int i;
	for(i=0;i<board_size;i++)
	{
		if(chess_board[i])
			printf("%d, ", i);
	}
	printf("\n");
}


//Get number of queens on board
int get_queen_count(int chess_board[])
{
	int queen_count = 0;
	int i;
	for(i =0;i < board_size;i++)
	{
		if(chess_board[i])
			queen_count++;
	}

	return queen_count;
}

//Add array of position to chess board
void add_to_base_case(int data[], int r, int ** base_cases)
{
	int i;
	for (i = 0; i < r; i++)
	{
		//add queen to chess board
		base_cases[base_case_index][data[i]] = 1;
	}
	base_case_index++;
}

//Choose K+1 number from n^2, once a combination is generated, add it to chess board
void combinationUtil(int arr[], int data[], int start, int end, int index, int r, int ** base_cases)
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
    int i;
    for (i=start; i<=end && end-i+1 >= r-index; i++)
    {
        data[index] = arr[i];
        combinationUtil(arr, data, i+1, end, index+1, r, base_cases);
    }
}

//Generate base case from k and n, we are sure that k+1 queen on board will not have any queen have more than k attacks
void generate_base_case(int count, int  ** base_cases)
{
	/*
		Initialize the base cases array, for each chess board position, 0 represent no queen, 1 represent have a queen
	*/
	int i, j;
	for(i =0;i<count;i++)
	{
		
		for(j =0;j<board_size;j++)
		{
			base_cases[i][j] = 0;
		}
	}

	//arr represent a set of positions on board
	int arr[board_size];
	
	for(i=0;i<board_size;i++)
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

//To calculate queen i's attack count
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

//Calculate the attacks for every queen, check if they have no more than k attacks
int is_safe_attack(int old_chess_board[], int new_queen_position)
{
	int * chess_board = malloc(sizeof(int) * board_size);;
	memcpy(chess_board, old_chess_board, board_size * sizeof(int));
	chess_board[new_queen_position] = 1;
	int i;

	for(i=0;i<board_size;i++){
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

//Validate if every queen on board has exact k attacks
int is_valid_solution(int chess_board[])
{
	int i;
	for(i=0;i<board_size;i++){
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

//Check if the two boards have the same queen position
int compare_chess_board(int board1[], int board2[])
{
	int i;
	for(i=0;i<board_size;i++)
	{
		if(board1[i]!=board2[i])
			return 0;
	}
	return 1;
}



//if the chess board (which is a valid solutiin) has more than current maximum queen count, refresh solution
//if the chess board is the same as current queen count, add to solution
//else discard
void add_to_solution(int * chess_board, struct Solution *result)
{
	int queen_count = get_queen_count(chess_board);

	if(queen_count > result->max_queen_count)
	{
		//clean solution, add this to it
		//free_solution();
		result->size = 0;
		result->max_queen_count = queen_count;
		result->solution[result->size] = chess_board;
		result->size++;
	}
	else if(queen_count == result->max_queen_count)
	{
		//add to set
		if (result->max_queen_count<MAX_SOLUTION_SIZE)
		{
			int i;
			for(i = 0;i<result->size;i++)
			{
				if(compare_chess_board(result->solution[i], chess_board) == 1)
					return;
			}

			result->solution[result->size] = chess_board;
			result->size++;			
		}

	}
	else
	{
		//free(chess_board);
		return;
	}
		
}

/*
	solve the cases, the main part of the algorithm
*/
void solve(int *chess_board, int current_position, struct Solution *result) 
{
	int is_end_case = 1;
	int i;

	for(i = current_position;i < board_size;i++)
	{
		if(is_position_empty(chess_board, i) && is_safe_attack(chess_board, i))
		{
			int * new_chess_board;
			new_chess_board = add_queen(chess_board, i);
			solve(new_chess_board, i+1, result);
			is_end_case = 0;
			// break;
		}
	}

	if(is_end_case)
	{
		if(is_valid_solution(chess_board))
			add_to_solution(chess_board, result);

		return;
	}
}


//Helper function for printint the solution
void print_solution(struct Solution *result)
{
	printf("%d:%d:\n", result->max_queen_count, result->size);
	int i;
	if(l)
		for(i = 0; i < result->size;i++)
		{
			print_chess_board(result->solution[i]);
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
	MPI_Init(NULL, NULL);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

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
	
	

	/*
		Initialize storage
	*/
	int elements_per_proc_min = case_count/world_size;
	int elements_per_proc_extra = case_count%world_size;
	int displs[world_size], sendcounts[world_size];
	int recvcount;
	int **base_cases, **sub_base_cases, *base_data, *sub_base_data, *result_data, *result_set_data;
	int i,j;
	struct Solution *result;
	

	/* Initialize all storage*/
	if(world_rank ==0)
	{
		base_data = (int *)malloc(sizeof(int)*case_count*board_size);
		base_cases = (int **)malloc(sizeof(int*)*case_count);
		for (i = 0; i < case_count; ++i)
		{
			base_cases[i] = &(base_data[i*board_size]);
		}
	}

	result_data = (int *)malloc(sizeof(int)*MAX_SOLUTION_SIZE*board_size);
	result = malloc(sizeof(struct Solution));
	result->solution = (int **)malloc(sizeof(int*)*MAX_SOLUTION_SIZE);
	for(i=0; i < MAX_SOLUTION_SIZE; i++) {
		result->solution[i] = &(result_data[i*board_size]);
	}

	result->size = 0;
	result->max_queen_count = 0;

	struct Solution *result_set = NULL;

	if (world_rank==0){
		result_set = malloc(sizeof(struct Solution)*world_size);
		for (i = 0; i < world_size; ++i)
		{
			result_set_data = (int *)malloc(sizeof(int)*MAX_SOLUTION_SIZE*board_size);
			result_set[i].solution = (int **)malloc(sizeof(int*)*MAX_SOLUTION_SIZE);
			int j;
			for (j = 0; j < MAX_SOLUTION_SIZE; j++)
			{
				result_set[i].solution[j] = &(result_set_data[j*board_size]);
			}
		}
	}

	/* calculating offset for scatterv*/
	int offset = 0;
  	for (i=0; i<world_size; i++)
    {
    	if (i<elements_per_proc_extra) 
    		sendcounts[i] = elements_per_proc_min+1;
    	else 
    		sendcounts[i] = elements_per_proc_min;
    	displs[i] = offset;
    	offset = offset+sendcounts[i];
    }
	recvcount = sendcounts[world_rank];

	for(i=0; i<world_size; i++)
	{
		sendcounts[i] *= board_size;
	}

	/* defining a datatype for sub base cases*/

	MPI_Datatype strip;
	MPI_Type_vector(recvcount, board_size, board_size, MPI_INT, &strip);
	MPI_Type_commit(&strip);
	sub_base_data = (int *)malloc(sizeof(int)*recvcount*board_size);
	sub_base_cases = (int **)malloc(sizeof(int*)*recvcount);
	for (i = 0; i < recvcount; ++i)
	{
		sub_base_cases[i] = &(sub_base_data[i*board_size]);
	}

    /*  defining a datatype for result struct */
	MPI_Datatype board_type;
	MPI_Type_contiguous(board_size, MPI_INT, &board_type);
	//MPI_Type_commit(&board_type);


	MPI_Datatype solution_set_mpi;
	MPI_Datatype type[2] = {MPI_INT, board_type};
	int blocklen[2] = {2,MAX_SOLUTION_SIZE};

	MPI_Aint offsets[2];
	offsets[0] = offsetof(struct Solution, size);
	offsets[1] = offsetof(struct Solution, solution);

	MPI_Type_create_struct(2, blocklen, offsets, type, &solution_set_mpi);
	MPI_Type_commit(&solution_set_mpi);



	/*
		Start to solve the problem
	*/
	if (world_rank == 0) 
	{
		generate_base_case(case_count, base_cases);
		for (i = 0; i < case_count; ++i)
		{
			print_chess_board(base_cases[i]);
		}
	}

	MPI_Scatterv(base_data, sendcounts, displs, MPI_INT, &(sub_base_cases[0][0]), recvcount*board_size, MPI_INT, 0, MPI_COMM_WORLD);
	
	//Processes solve the probelm based on different base cases in parallel


	for(i =0;i<recvcount;i++)
	{	
		//print_chess_board(sub_base_cases[i]);
		solve(sub_base_cases[i], 0, result);
	}

		
	//Print the solution

	//print_solution(result);
	printf("---%p  %p---\n", &result, &(result->solution[0]));
	//printf("%d %d\n", result_set[x].size,result_set[x].max_queen_count);
	

	MPI_Gather(result, 1, solution_set_mpi, result_set, 1, solution_set_mpi, 0, MPI_COMM_WORLD);


	if (world_rank == 0) {
		int x,y;
		for(x=0;x<world_size;x++){
			
			printf("------%d-----\n", x);
			printf("%p  %p\n", &result_set[x], &(result_set[x].solution[0]));
			printf("%d %d\n", result_set[x].size,result_set[x].max_queen_count);
			//print_solution(&(result_set[x]));

		}
	}

	// Clean up
	// if (world_rank == 0) {
	// 	free(base_cases);
	// }
	// free(sub_base_cases);
 
  	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	return 0;
}
