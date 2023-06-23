// Author: Illia Baturov (xbatur00)
// argparse.c

#include "proj2.h"

// Argument parsing
void parse_args(int argc, char *argv[])
{
    // Not correct number of arguments
    if (argc != 6)
    {
        fprintf(stderr, "Usage: %s NZ NU TZ TU F\n", argv[0]);
        exit(1);
    }

    // All arguments
    num_customers = atoi(argv[1]);
    num_workers = atoi(argv[2]);
    max_customer_waiting_time = atoi(argv[3]);
    max_worker_break_time = atoi(argv[4]);
    max_post_office_hours = atoi(argv[5]);

    // Arguments range check
    if (max_customer_waiting_time < 0 || max_customer_waiting_time > 10000)
    {
        fprintf(stderr, "Error: TZ must be between 0 and 10000\n");
        exit(1);
    }
    if (max_worker_break_time < 0 || max_worker_break_time > 100)
    {
        fprintf(stderr, "Error: TU must be between 0 and 100\n");
        exit(1);
    }
    if (max_post_office_hours < 0 || max_post_office_hours > 10000)
    {
        fprintf(stderr, "Error: F must be between 0 and 10000\n");
        exit(1);
    }

    return;
}
