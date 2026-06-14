# Almost Queens (AQ) Solver

This repository contains solvers for the "Almost Queens" problem, a variation of the N-Queens problem. The goal is to find configurations on an $n \times n$ board where every queen experiences exactly $k$ attacks from other queens. The solvers aim to find the maximum possible number of queens for a given $k$ and $w$ (toroidal board flag).

## Files

- `findAQ_seq.c`: Optimized sequential solver.
- `findAQ_par.c`: Optimized parallel solver using MPI.
- `test_aq.sh`: Automated test script for the sequential solver.
- `machinefile.1` / `rankfile.1`: Configuration files for MPI execution.

## Usage

### Sequential Solver

**Compilation:**
```bash
gcc -o findAQ_seq findAQ_seq.c -lm
```

**Running:**
```bash
./findAQ_seq n k l w
```
- `n`: Board dimension (e.g., 4 for a 4x4 board).
- `k`: Number of attacks allowed per queen.
- `l`: Print solutions flag (1 to print, 0 to skip).
- `w`: Toroidal board flag (1 for wrap-around, 0 for standard).

**Example:**
```bash
./findAQ_seq 4 0 1 0
```

### Parallel Solver

**Compilation:**
```bash
mpicc -o findAQ_par findAQ_par.c -lm
```

**Running:**
```bash
mpirun -np <num_procs> ./findAQ_par n k l w
```

### Testing

Run the automated test script to verify the sequential solver:
```bash
./test_aq.sh
```

## Optimizations and Bug Fixes

- **In-place Safety Checks:** The solvers modify the board array in-place during attack calculations to avoid expensive `malloc` and `memcpy` operations.
- **Memory Management:** Fixed several critical memory leaks in the recursive backtracking logic and added comprehensive cleanup in `main`.
- **Toroidal Logic Fix:** Initialized `stop_position` in the directional attack scan to prevent undefined behavior when $w=1$.
- **Correct MPI Aggregation:** The parallel solver correctly synchronizes the global maximum queen count and aggregates/deduplicates solutions on the root process.
