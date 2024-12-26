#include <stdio.h>
#include <stdlib.h>
#include "rule_based_helpers.h"

#define UNASSIGNED 0

sudoku_t *sudoku_init(int box_size)
{
	sudoku_t *sudoku = malloc(sizeof(sudoku_t));
	sudoku->box_size = box_size;
	sudoku->grid_size = box_size * box_size;

	sudoku->grid = malloc(sudoku->grid_size * sizeof(cell_t *));
	for (int i = 0; i < sudoku->grid_size; i++) {
		sudoku->grid[i] = malloc(sudoku->grid_size * sizeof(cell_t));
		for (int j = 0; j < sudoku->grid_size; j++) {
			int tmp;
			scanf("%d", &tmp);
			sudoku->grid[i][j].value = (char) tmp;
			sudoku->grid[i][j].possibilities = calloc(sudoku->grid_size, sizeof(char));
		}
	}

	calculate_possibilities(sudoku);

	return sudoku;
}

void sudoku_free(sudoku_t **sudoku)
{
	for (int i = 0; i < (*sudoku)->grid_size; i++) {
		for (int j = 0; j < (*sudoku)->grid_size; j++)
			free((*sudoku)->grid[i][j].possibilities);
		free((*sudoku)->grid[i]);
	}
	free((*sudoku)->grid);
	
	free(*sudoku);
	sudoku = NULL;
}

void solve_sudoku(sudoku_t *sudoku)
{
	apply_strategies(sudoku);

	int row, col;
	find_cell(sudoku, &row, &col);

	if (row == -1 || col == -1) {
		if (!is_complete(sudoku))
			return;
		
		print_grid(sudoku);
		return;
	}

	sudoku_t *old_sudoku = malloc(sizeof(sudoku_t));
	old_sudoku->grid_size = sudoku->grid_size;
	old_sudoku->box_size = sudoku->box_size;

	old_sudoku->grid = malloc(sudoku->grid_size * sizeof(cell_t *));
	for (int i = 0; i < sudoku->grid_size; i++) {
		old_sudoku->grid[i] = malloc(sudoku->grid_size * sizeof(cell_t));
		for (int j = 0; j < sudoku->grid_size; j++)
			old_sudoku->grid[i][j].possibilities = malloc(sudoku->grid_size * sizeof(char));
	}

	remember_old(sudoku, old_sudoku);
	for (int i = 0; i < sudoku->grid[row][col].pb_count; i++) {
		int num = sudoku->grid[row][col].possibilities[i];
		sudoku->grid[row][col].value = num;
		sudoku->grid[row][col].pb_count = 0;

		int box_row = row - row % sudoku->box_size;
		int box_col = col - col % sudoku->box_size;
		for (int j = 0; j < sudoku->grid_size; j++) {
			for (int k = 0; k < sudoku->grid[row][j].pb_count; k++) {
				if (sudoku->grid[row][j].possibilities[k] == num) {
					for (int t = k; t < sudoku->grid[row][j].pb_count - 1; t++)
						sudoku->grid[row][j].possibilities[t] = sudoku->grid[row][j].possibilities[t+1];
					sudoku->grid[row][j].pb_count--;
					break;
				}
			}

			for (int k = 0; k < sudoku->grid[j][col].pb_count; k++) {
				if (sudoku->grid[j][col].possibilities[k] == num) {
					for (int t = k; t < sudoku->grid[j][col].pb_count - 1; t++)
						sudoku->grid[j][col].possibilities[t] = sudoku->grid[j][col].possibilities[t+1];
					sudoku->grid[j][col].pb_count--;
					break;
				}
			}

			for (int k = 0; k < sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].pb_count; k++) {
				if (sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].possibilities[k] == num) {
					for (int t = k; t < sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].pb_count - 1; t++)
						sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].possibilities[t] = sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].possibilities[t+1];
					sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].pb_count--;
					break;
				}
			}
		}

		solve_sudoku(sudoku);

		restore_old(sudoku, old_sudoku);
	}

	sudoku_free(&old_sudoku);
}

void calculate_possibilities(sudoku_t *sudoku)
{
	for (int i = 0; i < sudoku->grid_size; i++) {
		for (int j = 0; j < sudoku->grid_size; j++) {
			int k = 0;
			if (sudoku->grid[i][j].value == 0)
				for (int num = 1; num <= sudoku->grid_size; num++)
					if (is_safe(sudoku, i, j, num))
						sudoku->grid[i][j].possibilities[k++] = num;
			sudoku->grid[i][j].pb_count = k;
		}
	}
}

void apply_strategies(sudoku_t *sudoku)
{
    while (1) {
    	if (elimination_strategy(sudoku))
    		continue;
    	if (lone_ranger_strategy(sudoku))
    		continue;
    	break;
    }
}

int elimination_strategy(sudoku_t *sudoku)
{
    for (int row = 0; row < sudoku->grid_size; row++)
        for (int col = 0; col < sudoku->grid_size; col++) {
            if (sudoku->grid[row][col].pb_count == 1) {
                sudoku->grid[row][col].value = sudoku->grid[row][col].possibilities[0];
                sudoku->grid[row][col].pb_count = 0;
                int num = sudoku->grid[row][col].possibilities[0];

                int box_row = row - row % sudoku->box_size;
				int box_col = col - col % sudoku->box_size;
				for (int j = 0; j < sudoku->grid_size; j++) {
					for (int k = 0; k < sudoku->grid[row][j].pb_count; k++) {
						if (sudoku->grid[row][j].possibilities[k] == num) {
							for (int t = k; t < sudoku->grid[row][j].pb_count - 1; t++)
								sudoku->grid[row][j].possibilities[t] = sudoku->grid[row][j].possibilities[t+1];
							sudoku->grid[row][j].pb_count--;
							break;
						}
					}

					for (int k = 0; k < sudoku->grid[j][col].pb_count; k++) {
						if (sudoku->grid[j][col].possibilities[k] == num) {
							for (int t = k; t < sudoku->grid[j][col].pb_count - 1; t++)
								sudoku->grid[j][col].possibilities[t] = sudoku->grid[j][col].possibilities[t+1];
							sudoku->grid[j][col].pb_count--;
							break;
						}
					}

					for (int k = 0; k < sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].pb_count; k++) {
						if (sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].possibilities[k] == num) {
							for (int t = k; t < sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].pb_count - 1; t++)
								sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].possibilities[t] = sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].possibilities[t+1];
							sudoku->grid[box_row + j / sudoku->box_size][box_col + j % sudoku->box_size].pb_count--;
							break;
						}
					}
				}

               	return 1;
            }
        }

    return 0;
}

int lone_ranger_strategy(sudoku_t * sudoku)
{
    for (int row = 0; row < sudoku->grid_size; row++)
        for (int col = 0; col < sudoku->grid_size; col++) {
            int value_count = 0;
            int value = 0;
            for (int k = 0; k < sudoku->grid[row][col].pb_count; k++) {
                if (strategy_in_row(sudoku, row, sudoku->grid[row][col].possibilities[k]) == 1
                 && strategy_in_col(sudoku, col, sudoku->grid[row][col].possibilities[k]) == 1
                 && strategy_in_box(sudoku, row - row % sudoku->box_size, col - col % sudoku->box_size, sudoku->grid[row][col].possibilities[k]) == 1) {
                    value_count++;
                    value = sudoku->grid[row][col].possibilities[k];
                }
            }

            if (value_count == 1) {
                sudoku->grid[row][col].value = value;
                sudoku->grid[row][col].pb_count = 0;
                return 1;
            }
        }
    
    return 0;
}

int strategy_in_row(sudoku_t *sudoku, int row, int num)
{
    int count = 0;
    for (int j = 0; j < sudoku->grid_size; j++) {
        for (int k = 0; k < sudoku->grid[row][j].pb_count; k++) {
            if (sudoku->grid[row][j].possibilities[k] == num)
                count++;
        }
        if (count > 1)
            return count;
    }
    return count;
}

int strategy_in_col(sudoku_t *sudoku, int col, int num)
{
    int count = 0;
    for (int i = 0; i < sudoku->grid_size; i++) {
        for (int k = 0; k < sudoku->grid[i][col].pb_count; k++) {
            if (sudoku->grid[i][col].possibilities[k] == num)
                count++;
        }
        if (count > 1)
            return count;
    }
    return count;
}

int strategy_in_box(sudoku_t *sudoku, int box_start_row, int box_start_col, int num)
{
    int count = 0;
    for (int i = 0; i < sudoku->box_size; i++)
        for (int j = 0; j < sudoku->box_size; j++) {
            for (int k = 0; k < sudoku->grid[i+box_start_row][j + box_start_col].pb_count; k++) {
                if (sudoku->grid[i + box_start_row][j + box_start_col].possibilities[k] == num)
                    count++;
            }
            if (count > 1)
                return count;
        }
    return count;
}

int used_in_row(sudoku_t *sudoku, int row, int num)
{
	for (int col = 0; col < sudoku->grid_size; col++) {
        if (sudoku->grid[row][col].value == num) {
            return 1;
        }
    }

	return 0;
}

int used_in_col(sudoku_t *sudoku, int col, int num)
{
	for (int row = 0; row < sudoku->grid_size; row++) {
        if (sudoku->grid[row][col].value == num) {
            return 1;
        }
    }
			
	return 0;
}

int used_in_box(sudoku_t *sudoku, int box_start_row, int box_start_col, int num)
{
	for (int row = 0; row < sudoku->box_size; row++) {
        for (int col = 0; col < sudoku->box_size; col++) {
            if (sudoku->grid[row + box_start_row][col + box_start_col].value == num) {
                return 1;
            }
        }
    }
			
	return 0;
}

int is_safe(sudoku_t *sudoku, int row, int col, int num)
{
	return !used_in_row(sudoku, row, num)
        && !used_in_col(sudoku, col, num)
		&& !used_in_box(sudoku, row - row % sudoku->box_size, col - col % sudoku->box_size, num);
}

int is_complete(sudoku_t *sudoku)
{
	for (int i = 0; i < sudoku->grid_size; i++)
		for (int j = 0; j < sudoku->grid_size; j++)
			if (sudoku->grid[i][j].value == UNASSIGNED)
				return 0;
	return 1;
}

void find_cell(sudoku_t *sudoku, int *row, int *col)
{
	*row = *col = -1;
	int min_pb = sudoku->grid_size;

	for (int i = 0; i < sudoku->grid_size; i++) {
		for (int j = 0; j < sudoku->grid_size; j++) {
			if (sudoku->grid[i][j].pb_count > 0 && sudoku->grid[i][j].pb_count <= min_pb) {
				*row = i;
				*col = j;
				min_pb = sudoku->grid[i][j].pb_count;
			}
		}
	}
}

void remember_old(sudoku_t *sudoku, sudoku_t *old)
{
	for (int i = 0; i < sudoku->grid_size; i++) {
		for (int j = 0; j < sudoku->grid_size; j++) {
			old->grid[i][j].value = sudoku->grid[i][j].value;
			old->grid[i][j].pb_count = sudoku->grid[i][j].pb_count;
			for (int k = 0; k < sudoku->grid_size; k++)
				old->grid[i][j].possibilities[k] = sudoku->grid[i][j].possibilities[k];
		}
	}
}

void restore_old(sudoku_t *sudoku, sudoku_t *old)
{
	for (int i = 0; i < sudoku->grid_size; i++) {
		for (int j = 0; j < sudoku->grid_size; j++) {
			sudoku->grid[i][j].value = old->grid[i][j].value;
			sudoku->grid[i][j].pb_count = old->grid[i][j].pb_count;
			for (int k = 0; k < sudoku->grid_size; k++)
				sudoku->grid[i][j].possibilities[k] = old->grid[i][j].possibilities[k];
		}
	}
}

void print_grid(sudoku_t *sudoku)
{
	for (int row = 0; row < sudoku->grid_size; row++) {
		for (int col = 0; col < sudoku->grid_size; col++) {
            printf("%d ", sudoku->grid[row][col].value);
        }

		printf("\n");
	}
}
