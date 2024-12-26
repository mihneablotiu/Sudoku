# Sudoku

## Project description:
The purpose of this project is to implement an efficient sudoku solver for any given square-sized sudoku grid.

### Team members:
- Bloțiu Mihnea-Andrei
- Roșu Mihai-Cosmin

### Lab assistant:
- Rareș Folea

## Profiling
For profiling we used *perf* and *pprof* to determine the hotspots in our code. The following commands were used:
> perf record -c 1000 -g *executable*

> pprof -top --functions --cum perf.data

#### The output is saved at the following path: /docs/serial_profiling as a PNG

## Usage

### How to run all tests:
> ./run.sh

### How to run a single test using the serial implementation:
> make run_test_serial TEST=test1

### How to run a single test using the pthread implementation:
> make run_test_pthread THREADS=4 TEST=test1

### How to run a single test using the openmp implementation:
> make run_test_openmp THREADS=4 TEST=test1

## Milestones

* Week 1: 21.11.2023 - Profile data for the serial implementation of the code
* Week 2: 28.11.2023 - Parallel implementation in one technology + more tests
* Week 3: 05.12.2023 - Improving the first implementation (pthreads)
* Week 4: 12.12.2023 - Parallel implementation in second technology (openMP)
* Week 5: 19.12.2023 - Graphics for Serial vs Pthreads vs OpenMP vs Rule Based
