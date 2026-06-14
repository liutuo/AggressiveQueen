# Developer Guide for Aggressive Queens (AQ) Solvers

## Problem Domain
The 'Aggressive Queens' algorithm finds board configurations where each queen has exactly `k` attacks. The board is represented as a 1D integer array of size `n*n`.

## Core Logic
- `get_attack_count(i, n, board)`: Calculates attacks on a queen at index `i`. Supports toroidal boards (`w=1`) with wrap-around directions.
- `is_safe_attack(board, pos)`: Optimized check. It temporarily places a queen at `pos`, checks all queens for exceeding `k` attacks, and then removes it. **Crucial:** Never use heap allocation inside this function.
- `solve(board, pos)`: Recursive backtracking. Always `free` the board returned by `add_queen` after the recursive call.

## MPI Synchronization (`find_aq_par.c`)
1. **Work Distribution:** Uses `MPI_Scatterv` to send base case configurations to all processes.
2. **Global Max:** Uses `MPI_Allreduce` with `MPI_MAX` to find the highest number of queens found by any process.
3. **Result Collection:** Processes only contribute solutions that match the global maximum. Solutions are collected using `MPI_Gatherv` and deduplicated on rank 0.

## Memory Management
- The `solution` array in both solvers contains pre-allocated buffers. Use `memcpy` to store valid configurations; don't overwrite pointers.
- Ensure all auxiliary buffers (like those used in `generate_base_case`) are freed locally.

## Testing
Always run `bash test_aq.sh` before submitting changes to ensure no regressions in the sequential core logic.
