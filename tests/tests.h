#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include <assert.h>
#include <time.h>

#include <functional>

#define assert_are_equal(x,y)    (assert(x == y))
#define assert_are_equal_t(x,y,t)  (assert(abs(x-y)<t))
#define assert_are_not_equal(x,y)  (assert(x != y))
#define assert_is_true(x)      (assert(x != 0))
#define assert_is_false(x)      (assert(x == 0))

#define TEST_REPORT_DEPTH 2

void time_function (std::function<void()> x, const char* fname, const int L)
{
  clock_t T;

  if (L<=TEST_REPORT_DEPTH)
  {
    int La=L;
    while (La--)
      fprintf (stdout, " ");

    // FIXME: %-45-Ls
    fprintf(stdout, "%-44s", fname);
    T=clock();
  }

  x();

  if (L<=TEST_REPORT_DEPTH)
  {
    int La = L;
    while (La--)
      fprintf (stdout, " ");

    fprintf(stdout,"[%0.4fs]\n", (double)(clock()-T)/CLOCKS_PER_SEC);
  }
}
void time_function (std::function<void()> x, const char* fname, const char* group, const int L)
{
  clock_t T;

  if (L<TEST_REPORT_DEPTH)
  {
    int La=L;
    while (La--)
      fprintf (stdout, " ");
    fprintf(stdout, "%s\n", fname);
  }

  if (L<=TEST_REPORT_DEPTH)
    T=clock();

  x();
  
  if (L<=TEST_REPORT_DEPTH)
  {
    int La=L;
    while (La--)
      fprintf (stdout, " ");

    // FIXME: %(-40-L)s 
    fprintf(stdout, "%-40s %s [%0.4fs]\n", fname, group, (double)(clock()-T)/CLOCKS_PER_SEC);
  }
}
#define TEST(x)        {time_function (x, #x, 2);}
#define TEST_GROUP(x)  {time_function (x, #x, "GROUP", 1);}
#define TEST_SUITE(x)  {time_function (x, #x, "SUITE", 0);}