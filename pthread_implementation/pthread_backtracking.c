#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pthread_helpers.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: ./pthread_backtracking <num_threads>\n");
		exit(1);
	}

	int num_threads = atoi(argv[1]);
	int size;

	sudoku_t *sudoku;
	os_threadpool_t *tp;

	scanf("%d", &size);
	if (size < 2) {
		fprintf(stderr, "Sudoku minimum size is 2 (4x4)");
		exit(1);
	}

	sudoku = sudoku_init(size);

	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);

	apply_strategies(sudoku);

	int row, col, num;
	int initial_depth = 0;

	if (!find_unassigned_location(sudoku, &row, &col)) {
		for (int row = 0; row < sudoku->grid_size; row++) {
			for (int col = 0; col < sudoku->grid_size; col++) {
				printf("%d ", sudoku->grid[row][col]);
			}

			printf("\n");
		}
		
		sudoku_free(&sudoku);
	} else {
		tp = threadpool_create(num_threads);

		for (num = 1; num <= sudoku->grid_size; num++) {
			if (is_safe(sudoku, row, col, num)) {
				task_arg_t *task_arg = create_task_arg(sudoku, row, col, num, initial_depth, tp);
				add_task_in_queue(tp, task_create(task_arg, solve_sudoku_parallel));
			}
		}

		tp->start = 1;
		pthread_cond_broadcast(&tp->work_started);

		sudoku_free(&sudoku);
		threadpool_stop(tp);
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = finish.tv_sec - start.tv_sec;
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("Total time: %lf seconds\n", elapsed);

	return 0;
}