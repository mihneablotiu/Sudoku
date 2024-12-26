#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "openmp_helpers.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: ./openmp_backtracking <num_threads>\n");
		exit(1);
	}

	int num_threads = atoi(argv[1]);
	int size;
	scanf("%d", &size);
	if (size < 2) {
		fprintf(stderr, "Sudoku minimum size is 2 (4x4)");
		exit(1);
	}

	sudoku_t *sudoku = sudoku_init(size);

	double start, end;
	start = omp_get_wtime();

	omp_set_num_threads(num_threads);

	apply_strategies(sudoku);

	int row, col, num;
	if (!find_unassigned_location(sudoku, &row, &col)) {
		print_grid(sudoku);
		sudoku_free(&sudoku);
	} else {
		#pragma omp parallel
		{
			#pragma omp single
			{
				for (num = 1; num <= sudoku->grid_size; num++) {
					if (is_safe(sudoku, row, col, num)) {
						sudoku_t *new_sudoku = malloc(sizeof(sudoku_t));
						new_sudoku->grid_size = sudoku->grid_size;
						new_sudoku->box_size = sudoku->box_size;

						new_sudoku->grid = malloc(sudoku->grid_size * sizeof(int *));
						for (int i = 0; i < sudoku->grid_size; i++) {
							new_sudoku->grid[i] = malloc(sudoku->grid_size * sizeof(int));
							for (int j = 0; j < sudoku->grid_size; j++)
								new_sudoku->grid[i][j] = sudoku->grid[i][j];
						}
						
						#pragma omp task firstprivate(new_sudoku,row,col,num)
						solve_sudoku_parallel(new_sudoku, row, col, num, 0);
					}
				}
			}
		}

		sudoku_free(&sudoku);
	}

	end = omp_get_wtime();
	printf("Total time: %lf seconds\n", end - start);

	return 0;
}
