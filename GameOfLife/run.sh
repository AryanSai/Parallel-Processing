#!/bin/bash

# Define the values for N and THREADS
N_VALUES=(10 50)
THREADS_VALUES=(1 4)

# Create a text file to store the results
OUTPUT_FILE="results.txt"
echo "N Threads Execution Time" > $OUTPUT_FILE

# Loop through N and THREADS values
for N in "${N_VALUES[@]}"; do
    for THREADS in "${THREADS_VALUES[@]}"; do
        # Compile the C program
        gcc -o game_of_life GameOfLife.c -fopenmp -DNUM_THREADS=$THREADS

        # Run the program and record execution time
        EXECUTION_TIME=$(./game_of_life $N $THREADS | grep "The run-time is" | awk '{print $NF}')

        # Append the results to the output file
        echo "$N $THREADS $EXECUTION_TIME" >> $OUTPUT_FILE
    done
done

echo "Execution completed. Results saved in $OUTPUT_FILE"
