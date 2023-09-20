// // #include <stdio.h>
// // int main()
// // {
// //     int matrix[3][3] = {{0, 9, 8}, {1, 4, 2}, {3, 6, 8}};
// //     int i = 1, j = 2;
// //     int nbrs[8];
// //     // printf("%d", *matrix[i, j]);
// //     nbrs[1] = *matrix[i, j];
// //     printf("%d", nbrs[1]);
// // }



// // // int getNeighbors(int **grid, int i, int j)
// // // {
// // //     int count = grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1] + grid[i][j - 1] + grid[i][j + 1] + grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];
// // //     return count;
// // // }

// void main()
// {
//     srand(time(0));
//     int i = 2, j = 0;

//     // create the grid
//     int **grid = randomGrid();
//     int **newGrid = (int **)malloc(N * sizeof(int *)); // New grid to store the next state

//     for (int i = 0; i < N; i++)
//     {
//         newGrid[i] = (int *)malloc(N * sizeof(int));
//     }

//     printGrid(grid);

// #pragma omp parallel for num_threads(9)
//     for (int i = 0; i < N; i++)
//     {
//         for (int j = 0; j < N; j++)
//         {
//             gameOfLife(grid, newGrid, i, j); // Pass the newGrid to store next state
//         }
//     }

//     // Update the original grid with the new state
//     for (int i = 0; i < N; i++)
//     {
//         for (int j = 0; j < N; j++)
//         {
//             grid[i][j] = newGrid[i][j];
//         }
//     }

//     // Free the memory used by newGrid
//     for (int i = 0; i < N; i++)
//     {
//         free(newGrid[i]);
//     }
//     free(newGrid);

//     printf("New grid: ");
//     printGrid(grid);
// }

// void gameOfLife(int **grid, int **newGrid, int i, int j)
// {
//     int count = nbrSum(grid, i, j);
    
//     if (grid[i][j] == 1) // if 1 -- live cell
//     {
//         if (count < 2 || count > 3)
//         {
//             newGrid[i][j] = 0;
//         }
//         else
//         {
//             newGrid[i][j] = 1;
//         }
//     }
//     else // if 0 -- dead cell
//     {
//         if (count == 3)
//         {
//             newGrid[i][j] = 1;
//         }
//         else
//         {
//             newGrid[i][j] = 0;
//         }
//     }
// }
// #include <stdio.h>
// #include <omp.h>

// int main() {
//     #pragma omp parallel num_threads(4)
//     {
//         int thread_id = omp_get_thread_num();
//         printf("Thread %d before the barrier.\n", thread_id);
//         #pragma omp barrier
//         printf("Thread %d after the barrier.\n", thread_id);
//     }

//     return 0;
// }
