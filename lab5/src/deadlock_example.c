#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include <pthread.h>

int shared1 = 1;
int shared2 = 2;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *Foo1()
{
    pthread_mutex_lock(&mutex1);
    sleep(1);
    pthread_mutex_lock(&mutex2);
    /*
    while(pthread_mutex_trylock(&mutex2))
    {
        pthread_mutex_unlock(&mutex1);
        sleep(1);
        pthread_mutex_lock(&mutex1);
    }
    */
    shared1 = shared2 * 4 + shared1 * 5;
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

void *Foo2()
{
    pthread_mutex_lock(&mutex2);
    sleep(1);
    pthread_mutex_lock(&mutex1);
    shared2 = shared1 * 4 + shared2 * 5;
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

int main(int argc, char **argv) {
  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, Foo1, NULL))
  {
      printf("Error: pthread_create failed!\n");
      return 1;
  }
  if (pthread_create(&thread2, NULL, Foo2, NULL))
  {
      printf("Error: pthread_create failed!\n");
      return 1;
  }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  printf("shared1 = %d\nshared2 = %d\n", shared1, shared2);
  return 0;
}
