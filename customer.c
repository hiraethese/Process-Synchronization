// Author: Illia Baturov (xbatur00)
// customer.c

#include "proj2.h"

// Customer process
void customer_process(int customer_id)
{
    // function variable
    int num_queue;

    // started
    sem_wait(mutex_action_counter);
    shared_data->action_counter += 1;
    fprintf(stdout, "%d: Z %d: started\n", shared_data->action_counter, customer_id);
    fflush(stdout);
    sem_post(mutex_action_counter);

    // entering office
    srand(getpid());                                  // seed the random number generator
    usleep(rand() % (max_customer_waiting_time + 1)); // wait to enter the office

    // post office is closed
    sem_wait(mutex_office_closing);

    if (!shared_data->post_office_open)
    {
        sem_post(mutex_office_closing);

        sem_wait(mutex_action_counter);
        shared_data->action_counter += 1;
        fprintf(stdout, "%d: Z %d: going home\n", shared_data->action_counter, customer_id);
        fflush(stdout);
        sem_post(mutex_action_counter);

        exit(0);
    }

    // post office is open
    srand(getpid());              // seed the random number generator
    num_queue = (rand() % 3) + 1; // random action from 1 to 3

    sem_wait(mutex_action_counter);
    shared_data->action_counter += 1;
    fprintf(stdout, "%d: Z %d: entering office for a service %d\n", shared_data->action_counter, customer_id, num_queue);
    fflush(stdout);
    sem_post(mutex_action_counter);

    sem_post(mutex_office_closing);

    // enter the queue
    sem_wait(mutex_queue_counter);

    if (num_queue == 1)
    {
        // enqueue
        shared_data->count1 += 1;

        sem_post(mutex_queue_counter);

        // wait for worker in queue 1
        sem_wait(sem_enter_queue1);
    }
    else if (num_queue == 2)
    {
        // enqueue
        shared_data->count2 += 1;

        sem_post(mutex_queue_counter);

        // wait for worker in queue 2
        sem_wait(sem_enter_queue2);
    }
    else if (num_queue == 3)
    {
        // enqueue
        shared_data->count3 += 1;

        sem_post(mutex_queue_counter);
            
        // wait for worker in queue 3
        sem_wait(sem_enter_queue3);
    }

    // called by officer worker
    sem_wait(mutex_action_counter);
    shared_data->action_counter += 1;
    fprintf(stdout, "%d: Z %d: called by office worker\n", shared_data->action_counter, customer_id);
    fflush(stdout);
    sem_post(mutex_action_counter);

    srand(getpid());     // seed the random number generator
    usleep(rand() % 11); // wait for the end of service

    // going home
    sem_wait(mutex_action_counter);
    shared_data->action_counter += 1;
    fprintf(stdout, "%d: Z %d: going home\n", shared_data->action_counter, customer_id);
    fflush(stdout);
    sem_post(mutex_action_counter);

    exit(0);
}
