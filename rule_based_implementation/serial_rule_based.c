#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rule_based_helpers.h"

int main()
{
	int size;
	sudoku_t *sudoku;

	scanf("%d", &size);
	if (size < 2) {
		fprintf(stderr, "Sudoku minimum size is 2 (4x4)");
		exit(1);
	}

	sudoku = sudoku_init(size);

	clock_t start, end;

	start = clock();

	solve_sudoku(sudoku);

	end = clock();

	sudoku_free(&sudoku);

	printf("Total time: %lf seconds\n", (double) (end - start) / CLOCKS_PER_SEC);

	return 0;
}
