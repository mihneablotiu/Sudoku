CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -g

SERIAL_EXECUTABLE=serial_backtracking
SERIAL=$(wildcard ./serial_implementation/*.c)

PTHREAD_EXECUTABLE=pthread_backtracking
PTHREAD=$(wildcard ./pthread_implementation/*.c)
THREADPOOL=$(wildcard ./pthread_implementation/threadpool/*.c)

OPENMP_EXECUTABLE=openmp_backtracking
OPENMP=$(wildcard ./openmp_implementation/*.c)

RULE_BASED_EXECUTABLE=serial_rule_based
RULE_BASED=$(wildcard ./rule_based_implementation/*.c)

TEST=test1
THREADS=12

build: $(SERIAL_EXECUTABLE) $(PTHREAD_EXECUTABLE) $(OPENMP_EXECUTABLE) $(RULE_BASED_EXECUTABLE)

serial_backtracking: $(SERIAL)
	$(CC) $(CFLAGS) $^ -o $(SERIAL_EXECUTABLE)

pthread_backtracking: $(PTHREAD) $(THREADPOOL)
	$(CC) $(CFLAGS) $^ -o $(PTHREAD_EXECUTABLE) -lpthread

openmp_backtracking: $(OPENMP)
	$(CC) $(CFLAGS) $^ -o $(OPENMP_EXECUTABLE) -fopenmp

serial_rule_based: $(RULE_BASED)
	$(CC) $(CFLAGS) $^ -o $(RULE_BASED_EXECUTABLE)

run_test_serial: $(SERIAL_EXECUTABLE)
	./$(SERIAL_EXECUTABLE) < ./tests/in/$(TEST).in > ./tests/out/serial/$(TEST).out

run_test_pthread: $(PTHREAD_EXECUTABLE)
	./$(PTHREAD_EXECUTABLE) $(THREADS) < ./tests/in/$(TEST).in > ./tests/out/pthread/$(TEST).out

run_test_openmp: $(OPENMP_EXECUTABLE)
	./$(OPENMP_EXECUTABLE) $(THREADS) < ./tests/in/$(TEST).in > ./tests/out/openmp/$(TEST).out

run_test_rule_based: $(RULE_BASED_EXECUTABLE)
	./$(RULE_BASED_EXECUTABLE) < ./tests/in/$(TEST).in > ./tests/out/rule_based/$(TEST).out

clean:
	rm $(SERIAL_EXECUTABLE)
	rm $(PTHREAD_EXECUTABLE)
	rm $(OPENMP_EXECUTABLE)
	rm $(RULE_BASED_EXECUTABLE)
