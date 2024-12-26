#include <stdio.h>
#include <stdlib.h>
#include "serial_helpers.h"

#define UNASSIGNED 0

sudoku_t *sudoku_init(int box_size) {
    sudoku_t *sudoku = malloc(sizeof(sudoku_t));
    sudoku->box_size = box_size;
    sudoku->grid_size = box_size * box_size;

    sudoku->grid = malloc(sudoku->grid_size * sizeof(int *));
    for (int i = 0; i < sudoku->grid_size; i++) {
        sudoku->grid[i] = malloc(sudoku->grid_size * sizeof(int));
        for (int j = 0; j < sudoku->grid_size; j++) {
            scanf("%d", &(sudoku->grid[i][j]));
        }
    }

    return sudoku;
}

void sudoku_free(sudoku_t **sudoku) {
    for (int i = 0; i < (*sudoku)->grid_size; i++) {
        free((*sudoku)->grid[i]);
    }
    free((*sudoku)->grid);
    
    free(*sudoku);
    sudoku = NULL;
}

int find_unassigned_location(sudoku_t *sudoku, int *row, int *col)
{
	for (*row = 0; *row < sudoku->grid_size; (*row)++) {
        for (*col = 0; *col < sudoku->grid_size; (*col)++) {
            if (sudoku->grid[*row][*col] == UNASSIGNED) {
                return 1;
            }
        }
    }

	return 0;
}

int used_in_row(sudoku_t *sudoku, int row, int num)
{
	for (int col = 0; col < sudoku->grid_size; col++) {
        if (sudoku->grid[row][col] == num) {
            return 1;
        }
    }

	return 0;
}

int used_in_col(sudoku_t *sudoku, int col, int num)
{
	for (int row = 0; row < sudoku->grid_size; row++) {
        if (sudoku->grid[row][col] == num) {
            return 1;
        }
    }
			
	return 0;
}

int used_in_box(sudoku_t *sudoku, int box_start_row, int box_start_col, int num)
{
	for (int row = 0; row < sudoku->box_size; row++) {
        for (int col = 0; col < sudoku->box_size; col++) {
            if (sudoku->grid[row + box_start_row][col + box_start_col] == num) {
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

void apply_strategies(sudoku_t *sudoku)
{
    int modified = 0;
    // Will memorize all possible values for each cell.
    // When iterating through them, we go until 0 (or until we reach grid_size).
    int ***strategy_grid = malloc(sudoku->grid_size * sizeof(int **));
    for (int i = 0; i < sudoku->grid_size; i++)
        strategy_grid[i] = malloc(sudoku->grid_size * sizeof(int *));

    do {
        modified = elimination_strategy(sudoku, strategy_grid) || lone_ranger_strategy(sudoku, strategy_grid);
    } while (modified);

    strategy_free(strategy_grid, sudoku->grid_size);
}

int elimination_strategy(sudoku_t *sudoku, int ***strategy_grid)
{
    for (int i = 0; i < sudoku->grid_size; i++) {
        for (int j = 0; j < sudoku->grid_size; j++) {
            // Using calloc so that the default value is 0.
            strategy_grid[i][j] = calloc(sudoku->grid_size, sizeof(int));
            int k = 0;
            if (sudoku->grid[i][j] == 0)
                for (int num = 1; num <= sudoku->grid_size; num++)
                    if (is_safe(sudoku, i, j, num))
                        strategy_grid[i][j][k++] = num;
        }
    }

    int modified = 0;

    for (int i = 0; i < sudoku->grid_size; i++)
        for (int j = 0; j < sudoku->grid_size; j++) {
            // We check if there is more than one possible value for the current cell.
            if (strategy_grid[i][j][1] == 0 && strategy_grid[i][j][0] != 0) {
                sudoku->grid[i][j] = strategy_grid[i][j][0];
                modified = 1;
            }
            // We can now free this cell.
            free(strategy_grid[i][j]);
        }

    return modified;
}

int lone_ranger_strategy(sudoku_t * sudoku, int ***strategy_grid)
{
    for (int i = 0; i < sudoku->grid_size; i++) {
        for (int j = 0; j < sudoku->grid_size; j++) {
            // Using calloc so that the default value is 0.
            strategy_grid[i][j] = calloc(sudoku->grid_size, sizeof(int));
            int k = 0;
            if (sudoku->grid[i][j] == 0)
                for (int num = 1; num <= sudoku->grid_size; num++)
                    if (is_safe(sudoku, i, j, num))
                        strategy_grid[i][j][k++] = num;
        }
    }

    int modified = 0;
    for (int i = 0; i < sudoku->grid_size; i++)
        for (int j = 0; j < sudoku->grid_size; j++) {
            int value_count = 0;
            int value = 0;
            int k = 0;
            while (k < sudoku->grid_size && strategy_grid[i][j][k] != 0) {
                if (strategy_in_row(strategy_grid, i, strategy_grid[i][j][k], sudoku->grid_size) == 1
                 && strategy_in_col(strategy_grid, j, strategy_grid[i][j][k], sudoku->grid_size) == 1
                 && strategy_in_box(strategy_grid, i - i % sudoku->box_size, j - j % sudoku->box_size, strategy_grid[i][j][k], sudoku->box_size, sudoku->grid_size) == 1) {
                    value_count++;
                    value = strategy_grid[i][j][k];
                }
                k++;
            }

            if (value_count == 1) {
                sudoku->grid[i][j] = value;
                modified = 1;
            }
        }

    for (int i = 0; i < sudoku->grid_size; i++)
        for (int j = 0; j < sudoku->grid_size; j++)
            free(strategy_grid[i][j]);
    
    return modified;
}

int strategy_in_row(int ***strategy_grid, int row, int num, int grid_size)
{
    int count = 0;
    for (int j = 0; j < grid_size; j++) {
        int k = 0;
        while (k < grid_size && strategy_grid[row][j][k] != 0) {
            if (strategy_grid[row][j][k] == num)
                count++;
            k++;
        }
        if (count > 1)
            return count;
    }
    return count;
}

int strategy_in_col(int ***strategy_grid, int col, int num, int grid_size)
{
    int count = 0;
    for (int i = 0; i < grid_size; i++) {
        int k = 0;
        while (k < grid_size && strategy_grid[i][col][k] != 0) {
            if (strategy_grid[i][col][k] == num)
                count++;
            k++;
        }
        if (count > 1)
            return count;
    }
    return count;
}

int strategy_in_box(int ***strategy_grid, int box_start_row, int box_start_col, int num, int box_size, int grid_size)
{
    int count = 0;
    for (int i = 0; i < box_size; i++)
        for (int j = 0; j < box_size; j++) {
            int k = 0;
            while (k < grid_size && strategy_grid[i + box_start_row][j + box_start_col][k] != 0) {
                if (strategy_grid[i + box_start_row][j + box_start_col][k] == num)
                    count++;
                k++;
            }
            if (count > 1)
                return count;
        }
    return count;
}

void strategy_free(int ***strategy_grid, int grid_size)
{
    for (int i = 0; i < grid_size; i++)
        free(strategy_grid[i]);
    free(strategy_grid);
}

void solve_sudoku(sudoku_t *sudoku)
{
	int row, col;

	if (!find_unassigned_location(sudoku, &row, &col)) {
        print_grid(sudoku);
        return;
    }

	for (int num = 1; num <= sudoku->grid_size; num++) {
        if (is_safe(sudoku, row, col, num)) {
            sudoku->grid[row][col] = num;

            solve_sudoku(sudoku);

            sudoku->grid[row][col] = UNASSIGNED;
        }
	}
}

void print_grid(sudoku_t *sudoku)
{
	for (int row = 0; row < sudoku->grid_size; row++) {
		for (int col = 0; col < sudoku->grid_size; col++) {
            printf("%d ", sudoku->grid[row][col]);
        }

		printf("\n");
	}
}
