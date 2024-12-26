#ifndef PTHREAD_HELPERS_H
#define PTHREAD_HELPERS_H

#include "threadpool/os_threadpool.h"

typedef struct {
	int box_size;
	int grid_size;
	int **grid;
} sudoku_t;

typedef struct {
	sudoku_t *sudoku;
	int row;
	int col;
	int num;
	int depth;

	os_threadpool_t *tp;
} task_arg_t;

sudoku_t *sudoku_init(int box_size);
void sudoku_free(sudoku_t **sudoku);
void solve_sudoku_parallel(void *arg);
void solve_sudoku_serial(sudoku_t *sudoku, os_threadpool_t *tp);

// We try to preprocess the matrix as much as possible using different strategies.
void apply_strategies(sudoku_t *sudoku);
// If for a cell there is a single possible value, we place it.
int elimination_strategy(sudoku_t *sudoku, int ***strategy_grid);
// If for a cell there is a value that is unique for that row, column and box, we place it.
int lone_ranger_strategy(sudoku_t *sudoku, int ***strategy_grid);
int strategy_in_row(int ***strategy_grid, int row, int num, int grid_size);
int strategy_in_col(int ***strategy_grid, int col, int num, int grid_size);
int strategy_in_box(int ***strategy_grid, int box_start_row, int box_start_col, int num, int box_size, int grid_size);
void strategy_free(int ***strategy_grid, int grid_size);

int find_unassigned_location(sudoku_t *sudoku, int *row, int *col);
int used_in_row(sudoku_t *sudoku, int row, int num);
int used_in_col(sudoku_t *sudoku, int col, int num);
int used_in_box(sudoku_t *sudoku, int box_start_row, int box_start_col, int num);
int is_safe(sudoku_t *sudoku, int row, int col, int num);

task_arg_t *create_task_arg(sudoku_t *sudoku, int row, int col,
							int num, int depth, os_threadpool_t *tp);

void print_grid(sudoku_t *sudoku, os_threadpool_t *tp);

#endif
