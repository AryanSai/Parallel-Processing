#include <stdio.h>
#include <omp.h>

void do_work(int task_id) {
    printf("Task %d is starting...\n", task_id);
    // Simulate some work
    for (int i = 0; i < 100000; i++) {
        // Do some computations
    }
    printf("Task %d is done.\n", task_id);
}

int main() {
    // Enable OpenMP parallelism
    #pragma omp parallel
    {
        // Each thread will execute a task
        #pragma omp single
        {
            // Create three tasks
            #pragma omp task
            do_work(1);

            #pragma omp task
            do_work(2);

            #pragma omp task
            do_work(3);

            // Allow other tasks to execute while waiting for the tasks to complete
            #pragma omp taskyield
            printf("Main thread is yielding to other tasks...\n");

            // More tasks can be added here or other work can be done

            // Implicit taskwait after the single region, waits for all tasks to complete
        }
    }

    return 0;
}
