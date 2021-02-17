#  Profiling Code Using `clock_gettime`

> **See:** http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
>
> https://stackoverflow.com/questions/6749621/how-to-create-a-high-resolution-timer-in-linux-to-measure-program-performance>
>
> **Author:** [Guy Rutenberg](https://www.guyrutenberg.com/)

After raising the issue of the low resolution problem of the timer provided by `clock()` in [Resolution
Problems in `clock()`](http://www.guyrutenberg.com/2007/09/10/resolution-problems-in-clock/), I’ve
ended the post by mentioning to two more functions that should provide high-resolution timers suitable for
profiling code. In this post I will discuss one of them, `clock_gettime()`.

The `clock_gettime()` provides access to several useful timers with the resolution of nanoseconds. First, the prototype for the function is as follows:

```C++
int clock_gettime(clockid_t clk_id, struct timespect *tp);
```

The `clk_id` allows us to select a specific clock from the several offered by the system, which includes:

* `CLOCK_REALTIME`, a system-wide realtime clock.
* `CLOCK_PROCESS_CPUTIME_ID`, high-resolution timer provided by the CPU for each process.
* `CLOCK_THREAD_CPUTIME_ID`, high-resolution timer provided by the CPU for each of the threads.

Usually, there are more clocks provided, but I find these three the most useful as they allow to get the
execution time spent in the system level, process level and the thread level.

The current clock time, for the chosen clock is stored int the struct provided by the `*tp` pointer. The
`timespec` struct is defined as follows:

```C++
struct timespec {
  time_t tv_sec; /* seconds */
  long tv_nsec; /* nanoseconds */
};
```

To time the processing time some function took, one should call `clock_gettime()` twice. Once before
the function call and once right after it and subtract the returned timings to get the actual runtime.

Getting the difference between two `timespec` structs isn’t very complicated and can be acheived using
the function `diff()` defined below:

```C++
timespec diff(timespec start, timespec end)
{
  timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}
```

Now let’s move to some real example:

```C++
#include <iostream>
#include <time.h>
using namespace std;

timespec diff(timespec start, timespec end);

int main()
{
  timespec time1, time2;
  int temp;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
  for (int i = 0; i< 242000000; i++)
    temp+=temp;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
  cout<<diff(time1,time2).tv_sec<<":"<<diff(time1,time2).tv_nsec<<endl;
  return 0;
}

timespec diff(timespec start, timespec end)
{
  timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}
```
To use `clock_gettime` you need to include `time.h` and to link to `librt.a`. If you use gcc just make
sure you add `-lrt` to your list of arguments.

Play a bit with the length of the for loop. As you can see `clock_gettime` provides much more accurate
results and can register very short processing time too. Just remember that as the case with any profiling
functions, this function adds a little overhead to your program, so make sure you disable the profiling code,
using preprocessor commands for example, in the production release.

### 26/9/2007 – Update

You may want to take a look at The [Revised String Iteration Benchmark](http://www.guyrutenberg.com/2007/09/26/the-revised-string-iteration-benchmark/) post for
another, larger, example of using `clock_gettime` to time performance of code.
