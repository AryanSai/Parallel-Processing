#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

// function to print the grid
void printGrid(int **grid, int N)
{
    for (int i = 0; i < N; ++i)
    {
        printf("\n");
        for (int j = 0; j < N; ++j)
        {
            printf("%d", grid[i][j]);
        }
    }
    printf("\n");
}

// function to create an initial, valid grid
int **randomGrid(int N, int threads)
{
    int **grid = (int **)malloc(N * sizeof(int *));
#pragma omp parallel num_threads(threads)
    for (int i = 0; i < N; i++)
    {
        grid[i] = (int *)malloc(N * sizeof(int));

        for (int j = 0; j < N; j++)
        {
            grid[i][j] = rand() % 2;
        }
    }

    return grid;
}

// get sum of neighbors of the location (i,j) from the grid
int nbrSum(int **grid, int N, int i, int j, int threads)
{
    int sum = 0, row, col;
#pragma omp parallel for collapse(2) num_threads(threads) reduction(+ : sum)
    for (int rowOffset = -1; rowOffset <= 1; rowOffset++)
    {
        for (int colOffset = -1; colOffset <= 1; colOffset++)
        {
            row = i + rowOffset;
            col = j + colOffset;
            // Skip the center cell and check for valid neighbors
            if ((rowOffset != 0 || colOffset != 0) && row >= 0 && row < N && col >= 0 && col < N)
            {
                sum += grid[row][col];
            }
        }
    }
    return sum;
}

int gameOfLife(int **grid, int **newGrid, int N, int threads, int i, int j)
{
    int count = nbrSum(grid, N, i, j, threads);

    if (grid[i][j] == 1) // if 1 -- live cell
    {
        if (count < 2 || count > 3)
        {
            newGrid[i][j] = 0;
        }
        else
        {
            newGrid[i][j] = 1;
        }
    }
    else // if 0 -- dead cell
    {
        if (count == 3)
        {
            newGrid[i][j] = 1;
        }
        else
        {
            newGrid[i][j] = 0;
        }
    }
}

// Function to deallocate memory for a 2D integer array
void freeGrid(int **grid, int N)
{
    for (int i = 0; i < N; i++)
    {
        free(grid[i]);
    }
    free(grid);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <N> <THREADS>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int threads = atoi(argv[2]);

    srand(time(0));
    double start, end, total = 0.0, startt, endd, start1, end1;

    start = omp_get_wtime();
    // create the grid
    int **grid = randomGrid(N, threads);
    int **newGrid = (int **)malloc(N * sizeof(int *)); // New grid to store the next state

    for (int i = 0; i < N; i++)
    {
        newGrid[i] = (int *)malloc(N * sizeof(int));
    }

    int saturation = 0, threshold = 0;
    start1 = omp_get_wtime();
    while (saturation == 0 && threshold < 100)
    {
        int changes = 0; // Initialize the changes flag for this iteration
#pragma omp parallel for schedule(dynamic, 10) collapse(2) num_threads(threads)
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                startt = omp_get_wtime();
                gameOfLife(grid, newGrid, N, threads, i, j);
                endd = omp_get_wtime();
                total += (endd - startt);
                // Check if the cell's state changed
                if (grid[i][j] != newGrid[i][j])
                {
                    changes = 1;
                }
            }
        }

        if (changes == 0)
        {
            saturation = 1; // No changes occurred, simulation is saturated
        }
        // Update the original grid with the new state
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                grid[i][j] = newGrid[i][j];
            }
        }
        threshold++;
        system("clear"); // Clear the terminal
        // printf("---Game of Life---");
        // printGrid(grid, N);
        usleep(50000);
    }
    end1 = omp_get_wtime();
    end = omp_get_wtime();
    printf("The updation run-time is %lf\n", end1 - start1);
    printf("The parallel run-time is %lf\n", total);
    printf("The run-time is %lf\n", end - start);
    printf("---Game Ended---");

    // Free allocated memory
    freeGrid(grid, N);
    freeGrid(newGrid, N);
    return 0;
}
