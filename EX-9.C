```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5          // Number of philosophers / forks
#define MEALS 3      // Number of times each philosopher eats

sem_t forks[N];      // One semaphore for each fork
sem_t room;          // Allows at most N-1 philosophers at a time

void think(int id)
{
    printf("Philosopher %d is thinking.\n", id);
    usleep(100000);
}

void eat(int id)
{
    printf("Philosopher %d is eating.\n", id);
    usleep(100000);
}

void *philosopher(void *arg)
{
    int id = *(int *)arg;

    int left = id;
    int right = (id + 1) % N;

    for (int m = 0; m < MEALS; m++)
    {
        think(id);

        // Enter the room
        sem_wait(&room);

        // Pick up left fork
        sem_wait(&forks[left]);
        printf("Philosopher %d picked up left fork %d\n",
               id, left);

        // Pick up right fork
        sem_wait(&forks[right]);
        printf("Philosopher %d picked up right fork %d\n",
               id, right);

        // Eat
        eat(id);

        // Put down right fork
        sem_post(&forks[right]);

        // Put down left fork
        sem_post(&forks[left]);

        printf("Philosopher %d put down both forks.\n", id);

        // Leave the room
        sem_post(&room);
    }

    return NULL;
}

int main()
{
    pthread_t phil[N];
    int ids[N];

    // Initialize each fork
    for (int i = 0; i < N; i++)
    {
        sem_init(&forks[i], 0, 1);
    }

    // Allow only N-1 philosophers to enter
    sem_init(&room, 0, N - 1);

    // Create philosopher threads
    for (int i = 0; i < N; i++)
    {
        ids[i] = i;
        pthread_create(&phil[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all philosophers
    for (int i = 0; i < N; i++)
    {
        pthread_join(phil[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < N; i++)
    {
        sem_destroy(&forks[i]);
    }

    sem_destroy(&room);

    printf("\nAll philosophers have finished eating. ");
    printf("No deadlock occurred.\n");

    return 0;
}
```
