
// Deadlock-Free Dining Philosophers using Semaphores
// macOS + Linux compatible version using named POSIX semaphores

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define N 5
#define ROUNDS 3

sem_t *chopstick[N];
sem_t *room;

pthread_mutex_t print_lock;

// Wait on semaphore, retry if interrupted
void waitSem(sem_t *s) {
    while (sem_wait(s) == -1) {
        if (errno != EINTR) {
            perror("sem_wait failed");
            exit(1);
        }
    }
}

// Print messages without overlapping
void say(int id, const char *msg) {
    pthread_mutex_lock(&print_lock);
    cout << "Philosopher " << id << " " << msg << endl;
    pthread_mutex_unlock(&print_lock);
}

// Philosopher thread
void *philosopher(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < ROUNDS; i++) {

        say(id, "is thinking");
        sleep(1);

        // Allow maximum N-1 philosophers
        waitSem(room);

        // Pick left chopstick
        waitSem(chopstick[id]);

        // Pick right chopstick
        waitSem(chopstick[(id + 1) % N]);

        // Start eating
        say(id, "is EATING");
        sleep(1);

        // Print completion before releasing chopsticks
        say(id, "finished eating");

        // Put down right chopstick
        sem_post(chopstick[(id + 1) % N]);

        // Put down left chopstick
        sem_post(chopstick[id]);

        // Leave the room
        sem_post(room);
    }

    return NULL;
}

int main() {
    pthread_t t[N];
    int ids[N];

    char room_name[64];
    char chop_names[N][64];

    // Create unique semaphore names for this process
    snprintf(room_name, sizeof(room_name),
             "/dining_room_%d", getpid());

    // Initialize print mutex
    pthread_mutex_init(&print_lock, NULL);

    // Initialize room semaphore (maximum N-1 philosophers)
    sem_unlink(room_name);

    room = sem_open(room_name, O_CREAT | O_EXCL, 0600, N - 1);

    if (room == SEM_FAILED) {
        perror("Room semaphore creation failed");
        return 1;
    }

    // Initialize chopstick semaphores
    for (int i = 0; i < N; i++) {

        snprintf(chop_names[i], sizeof(chop_names[i]),
                 "/dining_chop_%d_%d", getpid(), i);

        sem_unlink(chop_names[i]);

        chopstick[i] = sem_open(
            chop_names[i], O_CREAT | O_EXCL, 0600, 1
        );

        if (chopstick[i] == SEM_FAILED) {
            perror("Chopstick semaphore creation failed");
            return 1;
        }
    }

    // Create five philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;

        if (pthread_create(&t[i], NULL, philosopher, &ids[i]) != 0) {
            cerr << "Thread creation failed\n";
            exit(1);
        }
    }

    // Wait for all philosophers
    for (int i = 0; i < N; i++) {
        pthread_join(t[i], NULL);
    }

    cout << "\nAll philosophers finished. No deadlock!" << endl;

    // Destroy and close synchronization resources
    for (int i = 0; i < N; i++) {
        sem_close(chopstick[i]);
        sem_unlink(chop_names[i]);
    }

    sem_close(room);
    sem_unlink(room_name);

    pthread_mutex_destroy(&print_lock);

    return 0;
}
