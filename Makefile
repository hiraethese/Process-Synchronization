CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic

all: proj2

proj2: proj2.o argparse.o customer.o worker.o
	$(CC) $(CFLAGS) $^ -o $@

proj2.o: proj2.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@

argparse.o: argparse.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@

customer.o: customer.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@

worker.o: worker.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@

zip:
	zip proj2.zip Makefile proj2.h proj2.c argparse.c customer.c worker.c

clean:
	rm -f *.o proj2 proj2.out proj2.zip
