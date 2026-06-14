#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SOLUTION_SIZE 2048

int base_case_index = 0;
int max_queen_count = 0;
int **solution;
int solution_count = 0;

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
    int arr[board_size];
    for (int i = 0; i < board_size; i++) arr[i] = i;
    int data[k + 1];
    combinationUtil(arr, data, 0, board_size - 1, 0, k + 1, base_cases);
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
                safe = 0;
                break;
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
    int *new_chess_board = malloc(sizeof(int) * board_size);
    memcpy(new_chess_board, chess_board, board_size * sizeof(int));
    new_chess_board[i] = 1;
    return new_chess_board;
}

int compare_chess_board(int board1[], int board2[]) {
    return memcmp(board1, board2, sizeof(int) * board_size) == 0;
}

void add_to_solution(int *chess_board) {
    int queen_count = get_queen_count(chess_board);
    if (queen_count > max_queen_count) {
        solution_count = 0;
        max_queen_count = queen_count;
        memcpy(solution[solution_count++], chess_board, sizeof(int) * board_size);
    } else if (queen_count == max_queen_count && solution_count < MAX_SOLUTION_SIZE) {
        for (int i = 0; i < solution_count; i++) {
            if (compare_chess_board(solution[i], chess_board)) return;
        }
        memcpy(solution[solution_count++], chess_board, sizeof(int) * board_size);
    }
}

void solve(int chess_board[], int current_position) {
    int is_end_case = 1;
    for (int i = current_position; i < board_size; i++) {
        if (is_position_empty(chess_board, i) && is_safe_attack(chess_board, i)) {
            int *new_chess_board = add_queen(chess_board, i);
            solve(new_chess_board, i + 1);
            free(new_chess_board);
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
    if (argc != 5) {
        printf("Usage: %s n k l w\n", argv[0]);
        return 0;
    }
    n = atoi(argv[1]);
    k = atoi(argv[2]);
    l = atoi(argv[3]);
    w = atoi(argv[4]);
    board_size = n * n;
    int case_count = nCr(board_size, k + 1);

    int **base_cases = malloc(sizeof(int *) * case_count);
    for (int i = 0; i < case_count; i++) base_cases[i] = malloc(sizeof(int) * board_size);
    solution = malloc(sizeof(int *) * MAX_SOLUTION_SIZE);
    for (int i = 0; i < MAX_SOLUTION_SIZE; i++) solution[i] = malloc(sizeof(int) * board_size);

    generate_base_case(case_count, base_cases);
    for (int i = 0; i < case_count; i++) solve(base_cases[i], 0);

    if (l == 0) {
        printf("%d,%d:%d:\n", n, k, max_queen_count);
    } else {
        for (int i = 0; i < solution_count; i++) {
            printf("%d,%d:%d:", n, k, max_queen_count);
            print_chess_board(solution[i]);
        }
    }

    for (int i = 0; i < case_count; i++) free(base_cases[i]);
    free(base_cases);
    for (int i = 0; i < MAX_SOLUTION_SIZE; i++) free(solution[i]);
    free(solution);
    return 0;
}
