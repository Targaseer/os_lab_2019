#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int *Children_pids;
int pnum = -1;

void stop_children(int signum)
{
    printf("Time expired!\n");
    for(int i = 0; i < pnum; i++)
    {
        printf("TERMINATING THIS PROCESS: %d RETURN: %d\n", Children_pids[i], kill(Children_pids[i], SIGTERM));
    }
    exit(0);
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int timeout = -1;
  bool with_files = false;
  const char* filename = "Childs_Data.txt";

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"timeout", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
                printf("seed is a positive number\n");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
                printf("array_size is a positive number\n");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum <= 0) {
                printf("pnum is a positive number\n");
                return 1;
            }
            break;
          case 3:
            timeout = atoi(optarg);
            if (timeout <= 0) {
                printf("timeout is a positive number\n");
                return 1;
            }
            break;
        
          default:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }
    Children_pids = (int*)malloc(sizeof(int) * pnum);
  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;
    int pipefd[2];
    FILE *fp;
    if (with_files)
    {
        fp = fopen(filename, "w");
    }
    else
    {
        if (pipe(pipefd) == -1)
        {
            printf("Pipe failed!\n");
            return 1;
        }
    }
    int part_size = array_size / pnum;
    if (timeout != -1)
    {
        signal(SIGALRM, stop_children);
        signal(SIGCHLD, SIG_IGN);
        alarm(timeout);
    }
  struct timeval start_time;
  gettimeofday(&start_time, NULL);
  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes++;
      if (child_pid == 0) {
        printf("Process %d started\n", getpid());
        // child process
        // parallel somehow
        struct MinMax cur_ans;
        if (i != pnum - 1)
        {
            cur_ans = GetMinMax(array, i*part_size, i*part_size + part_size);
        }
        else
        {
            cur_ans = GetMinMax(array, i*part_size, array_size);
        }
        if (with_files) {
            fprintf(fp, "%d %d\n", cur_ans.min, cur_ans.max);
        } else {
            write(pipefd[1], &cur_ans, sizeof(cur_ans));
        }
        sleep(60);
        printf("Process %d finished\n", getpid());
        return 0;
      }
      else
      {
          Children_pids[i] = child_pid;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    wait(NULL);
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

    if(with_files)
    {
        fclose(fp);
        fp = fopen(filename, "r");
    }
  for (int i = 0; i < pnum; i++) {
    struct MinMax cur_MinMax;

    if (with_files) {
        int tmin, tmax;
        fscanf(fp, "%d %d\n", &tmin, &tmax);
        cur_MinMax.min = tmin;
        cur_MinMax.max = tmax;
    } else {
        read(pipefd[0], &cur_MinMax, sizeof(cur_MinMax));
    }

    if (cur_MinMax.min < min_max.min) min_max.min = cur_MinMax.min;
    if (cur_MinMax.max > min_max.max) min_max.max = cur_MinMax.max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    if(with_files)
    {
        fclose(fp);
        remove(filename);
    }
  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
