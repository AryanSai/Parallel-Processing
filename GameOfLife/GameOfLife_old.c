// import the necessary libraries
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// defining the dimensions of the grid as macros
#define N 3

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

void gameOfLife(int **grid, int i, int j)
{
    int count = nbrSum(grid, i, j);

    if (grid[i][j] == 1) // if 1 -- live cell
    {
        if (count < 2 || count > 3)
        {
            grid[i][j] = 0;
        }
        else
        {
            grid[i][j] = 1;
        }
    }
    else // if 0 -- dead cell
    {
        if (count == 3)
        {
            grid[i][j] = 1;
        }
        else
        {
            grid[i][j] = 0;
        }
    }
}

void main()
{
    srand(time(0));
    int i = 2, j = 0;

    // create the grid
    int **grid = randomGrid();
    printGrid(grid);

#pragma omp parallel for num_threads(9)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            gameOfLife(grid, i, j);
        }
    }

    printf("New grid: ");
    printGrid(grid);
}
