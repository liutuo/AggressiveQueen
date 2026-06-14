#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_SOLUTION_SIZE 2048

int local_max_queen_count = 0;
int **local_solutions;
int local_solution_count = 0;

int board_size;
int n;
int k;
int l;
int w;

void print_chess_board(int chess_board[]) {
    int first = 1;
    for (int i = 0; i < board_size; i++) {
        if (chess_board[i]) {
            if (!first) printf(",");
            printf("%d", i + 1);
            first = 0;
        }
    }
    printf("\n");
}

int get_queen_count(int chess_board[]) {
    int queen_count = 0;
    for (int i = 0; i < board_size; i++) {
        if (chess_board[i]) queen_count++;
    }
    return queen_count;
}

int base_case_index = 0;
void add_to_base_case(int data[], int r, int **base_cases) {
    for (int i = 0; i < r; i++) {
        base_cases[base_case_index][data[i]] = 1;
    }
    base_case_index++;
}

void combinationUtil(int arr[], int data[], int start, int end, int index, int r, int **base_cases) {
    if (index == r) {
        add_to_base_case(data, r, base_cases);
        return;
    }
    for (int i = start; i <= end && end - i + 1 >= r - index; i++) {
        data[index] = arr[i];
        combinationUtil(arr, data, i + 1, end, index + 1, r, base_cases);
    }
}

void generate_base_case(int count, int **base_cases) {
    for (int i = 0; i < count; i++) {
        memset(base_cases[i], 0, sizeof(int) * board_size);
    }
    int *arr = malloc(sizeof(int) * board_size);
    for (int i = 0; i < board_size; i++) arr[i] = i;
    int *data = malloc(sizeof(int) * (k + 1));
    combinationUtil(arr, data, 0, board_size - 1, 0, k + 1, base_cases);
    free(arr); free(data);
}

int is_position_empty(int chess_board[], int i) {
    return !chess_board[i];
}

int get_attack_count(int i, int n, int chess_board[]) {
    int attack_count = 0;
    int r = i / n;
    int c = i % n;
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int d = 0; d < 8; d++) {
        for (int step = 1; step < n; step++) {
            int nr, nc;
            if (w == 0) {
                nr = r + dr[d] * step;
                nc = c + dc[d] * step;
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) break;
            } else {
                nr = (r + dr[d] * step) % n;
                if (nr < 0) nr += n;
                nc = (c + dc[d] * step) % n;
                if (nc < 0) nc += n;
            }
            if (!is_position_empty(chess_board, nr * n + nc)) {
                attack_count++;
                break;
            }
        }
    }
    return attack_count;
}

int is_safe_attack(int chess_board[], int new_queen_position) {
    chess_board[new_queen_position] = 1;
    int safe = 1;
    for (int i = 0; i < board_size; i++) {
        if (!is_position_empty(chess_board, i)) {
            if (get_attack_count(i, n, chess_board) > k) {
                safe = 0; break;
            }
        }
    }
    chess_board[new_queen_position] = 0;
    return safe;
}

int is_valid_solution(int chess_board[]) {
    for (int i = 0; i < board_size; i++) {
        if (!is_position_empty(chess_board, i)) {
            if (get_attack_count(i, n, chess_board) != k) return 0;
        }
    }
    return 1;
}

int *add_queen(int chess_board[], int i) {
    int *new_board = malloc(sizeof(int) * board_size);
    memcpy(new_board, chess_board, sizeof(int) * board_size);
    new_board[i] = 1;
    return new_board;
}

int compare_chess_board(int board1[], int board2[]) {
    return memcmp(board1, board2, sizeof(int) * board_size) == 0;
}

void add_to_solution(int *chess_board) {
    int queen_count = get_queen_count(chess_board);
    if (queen_count > local_max_queen_count) {
        local_solution_count = 0;
        local_max_queen_count = queen_count;
        memcpy(local_solutions[local_solution_count++], chess_board, sizeof(int) * board_size);
    } else if (queen_count == local_max_queen_count && local_solution_count < MAX_SOLUTION_SIZE) {
        for (int i = 0; i < local_solution_count; i++) {
            if (compare_chess_board(local_solutions[i], chess_board)) return;
        }
        memcpy(local_solutions[local_solution_count++], chess_board, sizeof(int) * board_size);
    }
}

void solve(int chess_board[], int current_position) {
    int is_end_case = 1;
    for (int i = current_position; i < board_size; i++) {
        if (is_position_empty(chess_board, i) && is_safe_attack(chess_board, i)) {
            int *new_board = add_queen(chess_board, i);
            solve(new_board, i + 1);
            free(new_board);
            is_end_case = 0;
        }
    }
    if (is_end_case && is_valid_solution(chess_board)) {
        add_to_solution(chess_board);
    }
}

int nCr(int n, int r) {
    if (r > n) return 0;
    if (n == 0 || r == 0 || n == r) return 1;
    return (int)lround(((double)n / (double)(n - r) / (double)r) * exp(lgamma(n) - lgamma(n - r) - lgamma(r)));
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (argc != 5) {
        if (world_rank == 0) printf("Usage: %s n k l w\n", argv[0]);
        MPI_Finalize(); return 0;
    }

    n = atoi(argv[1]); k = atoi(argv[2]); l = atoi(argv[3]); w = atoi(argv[4]);
    board_size = n * n;
    int case_count = nCr(board_size, k + 1);

    local_solutions = malloc(sizeof(int *) * MAX_SOLUTION_SIZE);
    for (int i = 0; i < MAX_SOLUTION_SIZE; i++) local_solutions[i] = malloc(sizeof(int) * board_size);

    int rem = case_count % world_size;
    int *sendcounts = malloc(sizeof(int) * world_size);
    int *displs = malloc(sizeof(int) * world_size);
    int sum = 0;
    for (int i = 0; i < world_size; i++) {
        sendcounts[i] = (case_count / world_size) + (i < rem ? 1 : 0);
        displs[i] = sum; sum += sendcounts[i];
    }

    int local_case_count = sendcounts[world_rank];
    int *local_flat_base_cases = malloc(sizeof(int) * local_case_count * board_size);

    if (world_rank == 0) {
        int **base_cases = malloc(sizeof(int *) * case_count);
        for (int i = 0; i < case_count; i++) base_cases[i] = malloc(sizeof(int) * board_size);
        generate_base_case(case_count, base_cases);
        int *flat_base_cases = malloc(sizeof(int) * case_count * board_size);
        for (int i = 0; i < case_count; i++) memcpy(&flat_base_cases[i * board_size], base_cases[i], sizeof(int) * board_size);

        int *sc_bytes = malloc(sizeof(int) * world_size);
        int *ds_bytes = malloc(sizeof(int) * world_size);
        for (int i = 0; i < world_size; i++) {
            sc_bytes[i] = sendcounts[i] * board_size;
            ds_bytes[i] = displs[i] * board_size;
        }
        MPI_Scatterv(flat_base_cases, sc_bytes, ds_bytes, MPI_INT, local_flat_base_cases, local_case_count * board_size, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = 0; i < case_count; i++) free(base_cases[i]);
        free(base_cases); free(flat_base_cases); free(sc_bytes); free(ds_bytes);
    } else {
        MPI_Scatterv(NULL, NULL, NULL, MPI_INT, local_flat_base_cases, local_case_count * board_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    for (int i = 0; i < local_case_count; i++) solve(&local_flat_base_cases[i * board_size], 0);

    int global_max = 0;
    MPI_Allreduce(&local_max_queen_count, &global_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    if (local_max_queen_count < global_max) local_solution_count = 0;

    int *all_counts = NULL;
    if (world_rank == 0) all_counts = malloc(sizeof(int) * world_size);
    MPI_Gather(&local_solution_count, 1, MPI_INT, all_counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int total_sol = 0;
    int *rc_sol = NULL, *ds_sol = NULL, *flat_all_sol = NULL;
    if (world_rank == 0) {
        rc_sol = malloc(sizeof(int) * world_size);
        ds_sol = malloc(sizeof(int) * world_size);
        for (int i = 0; i < world_size; i++) {
            rc_sol[i] = all_counts[i] * board_size;
            ds_sol[i] = total_sol * board_size;
            total_sol += all_counts[i];
        }
        flat_all_sol = malloc(sizeof(int) * (total_sol > 0 ? total_sol : 1) * board_size);
    }

    int *flat_local_sol = malloc(sizeof(int) * (local_solution_count > 0 ? local_solution_count : 1) * board_size);
    for (int i = 0; i < local_solution_count; i++) memcpy(&flat_local_sol[i * board_size], local_solutions[i], sizeof(int) * board_size);

    MPI_Gatherv(flat_local_sol, local_solution_count * board_size, MPI_INT, flat_all_sol, rc_sol, ds_sol, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        if (l == 0) {
            printf("%d,%d:%d:\n", n, k, global_max);
        } else if (total_sol > 0) {
            int unique_count = 0;
            int *unique_sol = malloc(sizeof(int) * total_sol * board_size);
            for (int i = 0; i < total_sol; i++) {
                int dup = 0;
                for (int j = 0; j < unique_count; j++) {
                    if (memcmp(&flat_all_sol[i * board_size], &unique_sol[j * board_size], sizeof(int) * board_size) == 0) {
                        dup = 1; break;
                    }
                }
                if (!dup) memcpy(&unique_sol[unique_count++ * board_size], &flat_all_sol[i * board_size], sizeof(int) * board_size);
            }
            for (int i = 0; i < unique_count; i++) {
                printf("%d,%d:%d:", n, k, global_max);
                print_chess_board(&unique_sol[i * board_size]);
            }
            free(unique_sol);
        }
        free(all_counts); free(rc_sol); free(ds_sol); free(flat_all_sol);
    }

    for (int i = 0; i < MAX_SOLUTION_SIZE; i++) free(local_solutions[i]);
    free(local_solutions); free(sendcounts); free(displs); free(local_flat_base_cases); free(flat_local_sol);
    MPI_Finalize();
    return 0;
}
