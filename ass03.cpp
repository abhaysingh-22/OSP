// Producer-Consumer problem using counting semaphores and mutex
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

#define SIZE 5                 // buffer size
#define ITEMS 5                // items per producer / consumer

int buffer[SIZE], in = 0, out = 0, item = 1;
sem_t empty_slots;             // counts empty slots (starts at SIZE)
sem_t full_slots;              // counts filled slots (starts at 0)
pthread_mutex_t mtx;           // mutual exclusion for buffer access

void *producer(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < ITEMS; i++) {
        sem_wait(&empty_slots);            // wait for an empty slot
        pthread_mutex_lock(&mtx);          // enter critical section
        buffer[in] = item;
        cout << "Producer " << id << " produced " << item << endl;
        item++;
        in = (in + 1) % SIZE;
        pthread_mutex_unlock(&mtx);        // leave critical section
        sem_post(&full_slots);             // one more filled slot
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < ITEMS; i++) {
        sem_wait(&full_slots);             // wait for a filled slot
        pthread_mutex_lock(&mtx);          // enter critical section
        int x = buffer[out];
        cout << "Consumer " << id << " consumed " << x << endl;
        out = (out + 1) % SIZE;
        pthread_mutex_unlock(&mtx);        // leave critical section
        sem_post(&empty_slots);            // one more empty slot
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t p[2], c[2];
    sem_init(&empty_slots, 0, SIZE);
    sem_init(&full_slots, 0, 0);
    pthread_mutex_init(&mtx, NULL);

    for (long i = 0; i < 2; i++) {
        pthread_create(&p[i], NULL, producer, (void *)(i + 1));
        pthread_create(&c[i], NULL, consumer, (void *)(i + 1));
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(p[i], NULL);
        pthread_join(c[i], NULL);
    }

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&mtx);
    return 0;
}