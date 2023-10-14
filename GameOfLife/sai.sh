#!/bin/bash

# Define grid sizes and thread counts
grid_sizes=(50 100 200)
thread_counts=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)

# Output CSV file
output_file="output.csv"

# Clear the output file and add the column titles
echo "Grid Size, Thread Count, Total Time, Sequential Time, Parallel Time" > "$output_file"

# Compile the C code
gcc -o game_of_life GameOfLifecopy.c -fopenmp

# Loop through grid sizes and thread counts
for grid_size in "${grid_sizes[@]}"; do
  for threads in "${thread_counts[@]}"; do
    echo "Running with grid size $grid_size and $threads threads..."
    # Run the program and store the output in a temporary file
    ./game_of_life "$grid_size" "$threads" > temp_output.txt
    # Extract the relevant information from the temporary output file
    total_time=$(grep "The total run-time is" temp_output.txt | awk '{print $5}')
    seq_time=$(grep "The sequential run-time is" temp_output.txt | awk '{print $5}')
    parallel_time=$(grep "The parallel run-time is" temp_output.txt | awk '{print $5}')
    # Append the information to the output CSV file
    echo "$grid_size, $threads, $total_time, $seq_time, $parallel_time" >> "$output_file"
    # Remove the temporary output file
    rm temp_output.txt
  done
done

# Clean up and exit
rm game_of_life
echo "Script execution complete. Results are stored in $output_file"
