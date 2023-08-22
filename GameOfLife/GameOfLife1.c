//Game of Life Program
//Aryan Sai Arvapelly   Reg.No. 23352   I MTech(CS)

// import the necessary libraries
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

// defining the dimensions of the grid as macros
#define N 30
#define DELAY 500000

// function to print the grid
void printGrid(int **grid)
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
int **randomGrid()
{
    int **grid = (int **)malloc(N * sizeof(int *));
#pragma omp parallel
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

// get sum of neightbors of the location (i,j) from the grid
int nbrSum(int **grid, int i, int j)
{
    int sum = 0, row, col;
#pragma omp parallel for collapse(2)
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

int gameOfLife(int **grid, int **Newgrid, int i, int j)
{
    int count = nbrSum(grid, i, j);
#pragma omp critical
    if (grid[i][j] == 1) // if 1 -- live cell
    {
        if (count < 2 || count > 3)
        {
            Newgrid[i][j] = 0;
        }
        else
        {
            Newgrid[i][j] = 1;
        }
    }
    else // if 0 -- dead cell
    {
        if (count == 3)
        {
            Newgrid[i][j] = 1;
        }
        else
        {
            Newgrid[i][j] = 0;
        }
    }
}

int main()
{
    srand(time(0));
    int i = 0, j = 0;
    clock_t start, stop;
    double d = 0.0;
    // create the grid
    int **grid = randomGrid();
    printGrid(grid);

    int **newGrid = (int **)malloc(N * sizeof(int *)); // New grid to store the next state

    for (int i = 0; i < N; i++)
    {
        newGrid[i] = (int *)malloc(N * sizeof(int));
    }
    
    start = clock();
    int k = 0;
    while (k < 100)
    {
#pragma omp parallel for schedule(dynamic, 10) collapse(2)
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                gameOfLife(grid, newGrid, i, j);
            }
        }

        // Update the original grid with the new state
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                grid[i][j] = newGrid[i][j];
            }
        }
        k++;
        system("clear"); // Clear the terminal
        printf("---Game of Life---");
        printGrid(grid);
        usleep(DELAY);
    }
    return 0;
}
