// Author: Illia Baturov (xbatur00)
// proj2.c

#include "proj2.h"

// Global variables
int num_customers;
int num_workers;
int max_customer_waiting_time;
int max_worker_break_time;
int max_post_office_hours;
shared_memory_t *shared_data;
FILE *file_pointer;
pid_t first_process_id;
pid_t second_process_id;
sem_t *mutex_action_counter; // mutex for action counter
sem_t *mutex_queue_counter;  // mutex for queue counter
sem_t *mutex_office_closing; // mutex for office check
sem_t *sem_enter_queue1;     // semaphore for entering the queue 1
sem_t *sem_enter_queue2;     // semaphore for entering the queue 2
sem_t *sem_enter_queue3;     // semaphore for entering the queue 3

// Main function
int main(int argc, char *argv[])
{
    // Parse arguments
    parse_args(argc, argv);

    // Open a file
    file_pointer = freopen(FILE_TO_OPEN, "w", stdout);
    if (file_pointer == NULL)
    {
        fprintf(stderr, "Error: freopen\n");
        exit(1);
    }

    // Create the shared memory
    shared_data = mmap(NULL, sizeof(*shared_data), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shared_data->post_office_open = true;
    shared_data->action_counter = 0;

    // Initialize mutex
    sem_unlink("/mutex_action_counter");
    mutex_action_counter = sem_open("/mutex_action_counter", O_CREAT | O_EXCL, 0666, 1);
    if (mutex_action_counter == SEM_FAILED)
    {
        fprintf(stderr, "Error: unable to create semaphore\n");
        exit(1);
    }

    sem_unlink("/mutex_queue_counter");
    mutex_queue_counter = sem_open("/mutex_queue_counter", O_CREAT | O_EXCL, 0666, 1);
    if (mutex_queue_counter == SEM_FAILED)
    {
        fprintf(stderr, "Error: unable to create semaphore\n");
        exit(1);
    }

    sem_unlink("/mutex_office_closing");
    mutex_office_closing = sem_open("/mutex_office_closing", O_CREAT | O_EXCL, 0666, 1);
    if (mutex_office_closing == SEM_FAILED)
    {
        fprintf(stderr, "Error: unable to create semaphore\n");
        exit(1);
    }

    // Initialize semaphore
    sem_enter_queue1 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_enter_queue2 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_enter_queue3 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_init(sem_enter_queue1, 1, 0);
    sem_init(sem_enter_queue2, 1, 0);
    sem_init(sem_enter_queue3, 1, 0);

    // Create child processes for customers
    for (int id = 1; id <= num_customers; id++)
    {
        second_process_id = fork();
        if (second_process_id == 0)
        {
            customer_process(id);
        }
    }

    // Create child processes for workers
    for (int id = 1; id <= num_workers; id++)
    {
        first_process_id = fork();
        if (first_process_id == 0)
        {
            worker_process(id);
        }
    }

    // Wait for post office closing
    srand(time(NULL)); // seed the random number generator with the current time
    usleep((rand() % (max_post_office_hours / 2)) + (max_post_office_hours / 2));

    // Close the post office
    sem_wait(mutex_office_closing);

    shared_data->post_office_open = false; // closing

    sem_wait(mutex_action_counter);
    shared_data->action_counter += 1;
    fprintf(stdout, "%d: closing\n", shared_data->action_counter);
    fflush(stdout);
    sem_post(mutex_action_counter);

    sem_post(mutex_office_closing);

    // Wait for all child processes
    while (true)
    {
        if (wait(NULL) < 0)
        {
            break;
        }
    }

    // End of parent process
    sem_close(sem_enter_queue3);
    sem_close(sem_enter_queue2);
    sem_close(sem_enter_queue1);
    sem_close(mutex_office_closing);
    sem_close(mutex_queue_counter);
    sem_close(mutex_action_counter);
    sem_unlink("/mutex_office_closing");
    sem_unlink("/mutex_queue_counter");
    sem_unlink("/mutex_action_counter");
    munmap(shared_data, sizeof *shared_data);
    fclose(file_pointer); // close a file in parent

    return 0;
}
