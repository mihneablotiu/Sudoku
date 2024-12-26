#!/bin/bash

# Define the input and output folders
input_folder="./tests/in"
pattern="*.in"
output_folder="./tests/out"
THREADS=4

# Run make build
make build

# Iterate over each test file in the input folder
for test_file in "$input_folder"/$pattern; do
    if [ -f "$test_file" ]; then
        # Extract the test name from the file path
        test_name=$(basename "$test_file" .in)

        # Initialize total time for each test
        total_time=0

        # Run the executable with the test file as input for the first 10 runs
        for run in {1..10}; do
            # Capture the start time
            start_time=$(date +%s.%N)

            # Run the executable with the test file as input and capture the output
            output=$(./serial_backtracking < "$test_file" | tail -n 1)

            # Capture the end time
            end_time=$(date +%s.%N)

            # Calculate and accumulate the elapsed time
            elapsed_time=$(echo "$end_time - $start_time" | bc -l)
            total_time=$(echo "$total_time + $elapsed_time" | bc -l)

            # Print the output for each run
            echo "Total time for '$test_name' - Run $run: $elapsed_time seconds"
        done

        # Calculate the average time for the first 10 runs
        average_time=$(echo "$total_time / 10" | bc -l)
        echo "Average time for '$test_name': $average_time seconds" > "tests/out/serial/${test_name}.out.average"

        # Print the completion message
        echo "Serial - Test '$test_name' completed."
    fi
done

# Iterate over each test file in the input folder
for test_file in "$input_folder"/$pattern; do
   if [ -f "$test_file" ]; then
       # Extract the test name from the file path
       test_name=$(basename "$test_file" .in)

       # Initialize total time for each test
       total_time=0

       # Run the executable with the test file as input for the first 10 runs
       for run in {1..10}; do
           # Capture the start time
           start_time=$(date +%s.%N)

           # Run the executable with the test file as input and capture the output
           output=$(./pthread_backtracking ${THREADS} < "$test_file" | tail -n 1)

           # Capture the end time
           end_time=$(date +%s.%N)

           # Calculate and accumulate the elapsed time
           elapsed_time=$(echo "$end_time - $start_time" | bc -l)
           total_time=$(echo "$total_time + $elapsed_time" | bc -l)

           # Print the output for each run
           echo "Total time for '$test_name' - Run $run: $elapsed_time seconds"
       done

       # Calculate the average time for the first 10 runs
       average_time=$(echo "$total_time / 10" | bc -l)
       echo "Average time for '$test_name': $average_time seconds" > "tests/out/pthread/${test_name}.out.${THREADS}.average"

       # Print the completion message
       echo "Pthread - Test '$test_name' completed - ${THREADS} THREADS"
   fi
done

# Iterate over each test file in the input folder
# for test_file in "$input_folder"/$pattern; do
#      if [ -f "$test_file" ]; then
#          # Extract the test name from the file path
#          test_name=$(basename "$test_file" .in)

#          # Initialize total time for each test
#          total_time=0

#          # Run the executable with the test file as input for the first 10 runs
#          for run in {1..10}; do
#              # Capture the start time
#              start_time=$(date +%s.%N)

#              # Run the executable with the test file as input and capture the output
#              output=$(./openmp_backtracking ${THREADS} < "$test_file" | tail -n 1)

#              # Capture the end time
#              end_time=$(date +%s.%N)

#              # Calculate and accumulate the elapsed time
#              elapsed_time=$(echo "$end_time - $start_time" | bc -l)
#              total_time=$(echo "$total_time + $elapsed_time" | bc -l)

#              # Print the output for each run
#              echo "Total time for '$test_name' - Run $run: $elapsed_time seconds"
#          done

#          # Calculate the average time for the first 10 runs
#          average_time=$(echo "$total_time / 10" | bc -l)
#          echo "Average time for '$test_name': $average_time seconds" > "tests/out/openmp/${test_name}.out.${THREADS}.average"

#          # Print the completion message
#          echo "OpenMP - Test '$test_name' completed - ${THREADS} THREADS"
#     fi
# done

# Iterate over each test file in the input folder
for test_file in "$input_folder"/$pattern; do
    if [ -f "$test_file" ]; then
        # Extract the test name from the file path
        test_name=$(basename "$test_file" .in)

        # Initialize total time for each test
        total_time=0

        # Run the executable with the test file as input for the first 10 runs
        for run in {1..10}; do
            # Capture the start time
            start_time=$(date +%s.%N)

            # Run the executable with the test file as input and capture the output
            output=$(./serial_rule_based < "$test_file" | tail -n 1)

            # Capture the end time
            end_time=$(date +%s.%N)

            # Calculate and accumulate the elapsed time
            elapsed_time=$(echo "$end_time - $start_time" | bc -l)
            total_time=$(echo "$total_time + $elapsed_time" | bc -l)

            # Print the output for each run
            echo "Total time for '$test_name' - Run $run: $elapsed_time seconds"
        done

        # Calculate the average time for the first 10 runs
        average_time=$(echo "$total_time / 10" | bc -l)
        echo "Average time for '$test_name': $average_time seconds" > "tests/out/rule_based/${test_name}.out.average"

        # Print the completion message
        echo "Rule Based - Test '$test_name' completed."
    fi
done

# Iterate over each test file in the input folder
for test_file in "$input_folder"/$pattern; do
    if [ -f "$test_file" ]; then
        # Extract the test name from the file path
        test_name=$(basename "$test_file" .in)

        # Initialize total time for each test
        total_time=0

        # Run the executable with the test file as input for the first 10 runs
        for run in {1..10}; do
            # Capture the start time
            start_time=$(date +%s.%N)

            # Run the executable with the test file as input and capture the output
            output=$(./serial_rule_based < "$test_file" | tail -n 1)

            # Capture the end time
            end_time=$(date +%s.%N)

            # Calculate and accumulate the elapsed time
            elapsed_time=$(echo "$end_time - $start_time" | bc -l)
            total_time=$(echo "$total_time + $elapsed_time" | bc -l)

            # Print the output for each run
            echo "Total time for '$test_name' - Run $run: $elapsed_time seconds"
        done

        # Calculate the average time for the first 10 runs
        average_time=$(echo "$total_time / 10" | bc -l)
        echo "Average time for '$test_name': $average_time seconds" > "tests/out/rule_based/${test_name}.out.average"

        # Print the completion message
        echo "Rule Based - Test '$test_name' completed."
    fi
done

# Run make clean
make clean
