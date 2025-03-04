#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 512

int
main(int argc, char *argv[])
{
  int i = 0, cc;
  char c;
  char space = 0;
  char buf[BUF_SIZE];
  char* sec;
  while (i + 1 < BUF_SIZE) {
    cc = read(0, &c, 1);
    if (cc < 0) {
      fprintf(2, "add: read error\n");
      exit(1);
    }
    if (cc == 0 || c == '\n' || c == '\r') {
      break;
    }
    if (c == ' ') {
      if (space++ > 0) {
        fprintf(2, "add: wrong input format\n");
        exit(1);
      }
      sec = buf + i + 1;
    }
    else if (('0' > c || '9' < c) && c != ' ') {
      fprintf(2, "add: invalid character\n");
      exit(1);
    }
    buf[i++] = c;
  }

  if (i + 1 == BUF_SIZE) {
    fprintf(2, "add: buffer overflow\n");
    exit(1);
  }

  if (space == 0) {
    fprintf(2, "add: wrong input format\n");
    exit(1);
  }
  
  buf[i] = '\0';

  printf("|%s|\n", buf);

  int a = atoi(buf);
  int b = atoi(sec);
  
  printf("%d\n", a + b);
  
  exit(0);
}
