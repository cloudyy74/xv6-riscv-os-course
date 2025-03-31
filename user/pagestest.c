#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char* argv[]) {
  pages(0, 0, 0);
  exit(0);
}
