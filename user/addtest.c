#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) {
   int test1 = add(1, 1);
   printf(test1 == 2   ? "test 1 (1 + 1):     passed\n" : "test 1: failed\n");

   int test2 = add(2, 2);
   printf(test2 == 4   ? "test 2 (2 + 2):     passed\n" : "test 2: failed\n");

   int test3 = add(100, 51);
   printf(test3 == 151 ? "test 3 (100 + 51):  passed\n" : "test 3: failed\n");

   int test4 = add(43, -1);
   printf(test4 == 42  ? "test 4 (43 + (-1)): passed\n" : "test 4: failed\n");

   int test5 = add(39, 0);
   printf(test5 == 39  ? "test 5 (39 + 0):    passed\n" : "test 5: failed\n");
}
