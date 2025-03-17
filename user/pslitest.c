#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"

int 
main(int argc, char *argv[]) {
  int ret = ps_listinfo(0, 0);
  printf(ret >= 0 ? "Test 1: passed\n" : "Test 1: failed\n");

  ret = ps_listinfo(0, 15);
  printf(ret >= 0 ? "Test 2: passed\n" : "Test 2: failed\n");

  ret = ps_listinfo(0, -15);
  printf(ret >= 0 ? "Test 3: passed\n" : "Test3: failed\n");

  struct procinfo info[42];
  ret = ps_listinfo(info, -1);
  printf(ret == -1 ? "Test 4: passed\n" : "Test 4: failed\n");

  ret = ps_listinfo(info, 100);
  printf(ret >= 0 ? "Test 5: passed\n" : "Test 5: failed\n");

  ret = ps_listinfo((struct procinfo *)42, 3);
  printf(ret == -2 ? "Test 6: passed\n" : "Test 6: failed\n");

  exit(0);
}
