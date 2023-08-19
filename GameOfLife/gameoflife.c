#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Size of the grid
#define GRID_SIZE 10
// Number of generations to simulate
#define NUM_GENERATIONS 100
// Delay in microseconds between generations
#define DELAY 500000

// Function to initialize the grid randomly
void initializeGrid(int grid[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j] = rand() % 2;
        }
    }
}

// Function to update the grid based on the rules of the game
void updateGrid(int grid[GRID_SIZE][GRID_SIZE])
{
    int newGrid[GRID_SIZE][GRID_SIZE];

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            int aliveNeighbors = 0;

            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    if (x == 0 && y == 0)
                        continue;

                    int ni = i + x;
                    int nj = j + y;

                    if (ni >= 0 && ni < GRID_SIZE && nj >= 0 && nj < GRID_SIZE)
                    {
                        aliveNeighbors += grid[ni][nj];
                    }
                }
            }

            if (grid[i][j] == 1)
            {
                newGrid[i][j] = (aliveNeighbors == 2 || aliveNeighbors == 3) ? 1 : 0;
            }
            else
            {
                newGrid[i][j] = (aliveNeighbors == 3) ? 1 : 0;
            }
        }
    }

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j] = newGrid[i][j];
        }
    }
}

// Function to display the grid
void displayGrid(int grid[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            printf(grid[i][j] ? "1 " : "0 ");
        }
        printf("\n");
    }
}

int main()
{
    int grid[GRID_SIZE][GRID_SIZE];

    srand(time(NULL)); // Seed the random number generator
    initializeGrid(grid);

    for (int generation = 0; generation < NUM_GENERATIONS; generation++)
    {
        system("clear"); // Clear the terminal (UNIX-like systems)
        displayGrid(grid);
        updateGrid(grid);
        usleep(DELAY);
    }

    return 0;
}
