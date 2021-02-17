//
//  main.c
//  CF.TimingsHighResC00
//
//  Created by Alan Sampson on 2/14/21.
//

#include <stdio.h>
#include <time.h>

#define STR(x)   #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))

#define YY__OSX_AVAILABLE_1012 __OSX_AVAILABLE(10.12)
#define YY__IOS_AVAILABLE_100  __IOS_AVAILABLE(10.0)
#define YY__TVOS_AVAILABLE_100 __TVOS_AVAILABLE(10.0)
#define YY__TVOS_AVAILABLE_30  __WATCHOS_AVAILABLE(3.0)

typedef struct timespec timespec;

inline
static
timespec diff(timespec start, timespec end);
int driver(int argc, const char * argv[]);

/*
 *  MARK: main()
 */
int main(int argc, const char * argv[]) {
  printf("CF.TimingsHighResC00\n");

  SHOW_DEFINE(YY__OSX_AVAILABLE_1012);
  SHOW_DEFINE(YY__IOS_AVAILABLE_100);
  SHOW_DEFINE(YY__TVOS_AVAILABLE_100);
  SHOW_DEFINE(YY__TVOS_AVAILABLE_30);

  for (size_t i_ = 0; i_ < 10; ++i_) {
    driver(argc, argv);
  }

  return 0;
}

int driver(int argc, const char * argv[]) {
  timespec time1, time2;
  int temp = 0;

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
  for (int i = 0; i < 242000000; i++) {
    temp += temp;
  }
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

  printf("%ld:%ld\n", diff(time1, time2).tv_sec, diff(time1, time2).tv_nsec);

  return 0;
}

inline
static
timespec diff(timespec start, timespec end) {
  timespec temp;

  if ((end.tv_nsec - start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
  }
  else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }

  return temp;
}
