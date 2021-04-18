#ifndef SUM
#define SUM

struct SumArgs {
  int *array;
  int begin;
  int end;
};

long long Sum(const struct SumArgs *args);

#endif