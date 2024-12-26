#ifndef RULE_BASED_HELPERS_H
#define RULE_BASED_HELPERS_H

typedef struct {
	char value;
	char *possibilities;
	int pb_count;
} cell_t;

typedef struct {
	cell_t **grid;
	int grid_size;
	int box_size;
} sudoku_t;

sudoku_t *sudoku_init(int box_size);
void sudoku_free(sudoku_t **sudoku);
void solve_sudoku(sudoku_t *sudoku);
void calculate_possibilities(sudoku_t *sudoku);

void apply_strategies(sudoku_t *sudoku);
int elimination_strategy(sudoku_t *sudoku);
int lone_ranger_strategy(sudoku_t *sudoku);
int strategy_in_row(sudoku_t *sudoku, int row, int num);
int strategy_in_col(sudoku_t *sudoku, int col, int num);
int strategy_in_box(sudoku_t *sudoku, int box_start_row, int box_start_col, int num);

int used_in_row(sudoku_t *sudoku, int row, int num);
int used_in_col(sudoku_t *sudoku, int col, int num);
int used_in_box(sudoku_t *sudoku, int box_start_row, int box_start_col, int num);
int is_safe(sudoku_t *sudoku, int row, int col, int num);
int is_complete(sudoku_t *sudoku);

void find_cell(sudoku_t *sudoku, int *row, int *col);
void remember_old(sudoku_t *sudoku, sudoku_t *old);
void restore_old(sudoku_t *sudoku, sudoku_t *old);

void print_grid(sudoku_t *sudoku);

#endif
