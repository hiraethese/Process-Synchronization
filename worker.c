// Author: Illia Baturov (xbatur00)
// worker.c

#include "proj2.h"

// Worker process
void worker_process(int worker_id)
{
    // function variable
    int num_queue;

    // started
    sem_wait(mutex_action_counter);
    shared_data->action_counter += 1;
    fprintf(stdout, "%d: U %d: started\n", shared_data->action_counter, worker_id);
    fflush(stdout);
    sem_post(mutex_action_counter);

    // cycle start
    while (true)
    {
        sem_wait(mutex_queue_counter);

        if (shared_data->count1 == 0 && shared_data->count2 == 0 && shared_data->count3 == 0)
        {
            // office is closed and queues are empty
            sem_post(mutex_queue_counter);

            sem_wait(mutex_office_closing);

            if (!shared_data->post_office_open)
            {
                sem_post(mutex_office_closing);
                
                // going home
                sem_wait(mutex_action_counter);
                shared_data->action_counter += 1;
                fprintf(stdout, "%d: U %d: going home\n", shared_data->action_counter, worker_id);
                fflush(stdout);
                sem_post(mutex_action_counter);

                exit(0);
            }

            // office is open and queues are empty
            sem_wait(mutex_action_counter);
            shared_data->action_counter += 1;
            fprintf(stdout, "%d: U %d: taking break\n", shared_data->action_counter, worker_id);
            fflush(stdout);
            sem_post(mutex_action_counter);

            sem_post(mutex_office_closing);

            srand(getpid());                              // seed the random number generator
            usleep(rand() % (max_worker_break_time + 1)); // take a break

            sem_wait(mutex_action_counter);
            shared_data->action_counter += 1;
            fprintf(stdout, "%d: U %d: break finished\n", shared_data->action_counter, worker_id);
            fflush(stdout);
            sem_post(mutex_action_counter);

            continue;
        }

        // search for non-empty queue
        if (shared_data->count1 != 0) {
            num_queue = 1;
        } else if (shared_data->count2 != 0) {
            num_queue = 2;
        } else if (shared_data->count3 != 0) {
            num_queue = 3;
        }

        // call a customer
        if (num_queue == 1) {
            // dequeue
            shared_data->count1 -= 1;
            sem_post(sem_enter_queue1); // queue 1
        } else if (num_queue == 2) {
            // dequeue
            shared_data->count2 -= 1;
            sem_post(sem_enter_queue2); // queue 2
        } else if (num_queue == 3) {
            // dequeue
            shared_data->count3 -= 1;
            sem_post(sem_enter_queue3); // queue 3
        }

        sem_post(mutex_queue_counter);

        // serve a customer in queue X=num_queue
        sem_wait(mutex_action_counter);
        shared_data->action_counter += 1;
        fprintf(stdout, "%d: U %d: serving a service of type %d\n", shared_data->action_counter, worker_id, num_queue);
        fflush(stdout);
        sem_post(mutex_action_counter);

        srand(getpid());     // seed the random number generator
        usleep(rand() % 11); // wait for the end of service

        // service finished
        sem_wait(mutex_action_counter);
        shared_data->action_counter += 1;
        fprintf(stdout, "%d: U %d: service finished\n", shared_data->action_counter, worker_id);
        fflush(stdout);
        sem_post(mutex_action_counter);

        continue;
    }
}
