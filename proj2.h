// Author: Illia Baturov (xbatur00)
// proj2.h

#ifndef PROJECT_TWO_H
#define PROJECT_TWO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

// Macro definition
#define FILE_TO_OPEN "proj2.out"

// Structure
typedef struct {
    bool post_office_open;
    int action_counter;
    int count1;
    int count2;
    int count3;
} shared_memory_t;

// Function prototype declarations
void parse_args(int argc, char *argv[]);
void worker_process(int worker_id);
void customer_process(int customer_id);

// Global variables
extern int num_customers;
extern int num_workers;
extern int max_customer_waiting_time;
extern int max_worker_break_time;
extern int max_post_office_hours;
extern shared_memory_t *shared_data;
extern FILE *file_pointer;
extern pid_t first_process_id;
extern pid_t second_process_id;
extern sem_t *mutex_action_counter; // mutex for action counter
extern sem_t *mutex_queue_counter;  // mutex for queue counter
extern sem_t *mutex_office_closing; // mutex for office check
extern sem_t *sem_enter_queue1;     // semaphore for entering the queue 1
extern sem_t *sem_enter_queue2;     // semaphore for entering the queue 2
extern sem_t *sem_enter_queue3;     // semaphore for entering the queue 3

#endif
