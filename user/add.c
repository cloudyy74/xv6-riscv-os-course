#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 512

int
main(int argc, char *argv[])
{
  int i = 0, cc;
  char c;
  char buf[BUF_SIZE];
  while (i + 1 < BUF_SIZE) {
    cc = read(0, &c, 1);
    if (cc < 1) {
      fprintf(2, "add: read error\n");
      exit(-1);
    }
    if (c == ' ') {
      break;
    }
    if ('0' > c || '9' < c) {
      fprintf(2, "add: invalid character\n");
      exit(-1);
    }
    buf[i++] = c;
  }

  if (c != ' ') {
    fprintf(2, "add: buffer overflow\n");
    exit(-1);
  }

  buf[i] = '\0';

  printf("|%s ", buf);

  int a = atoi(buf);


  i = 0;

  while (i + 1 < BUF_SIZE) {
    cc = read(0, &c, 1);
    if (cc < 1) {
      fprintf(2, "add: read error\n");
      exit(-1);
    }
    if (c == '\n' || c == '\r') {
      break;
    }
    if ('0' > c || '9' < c) {
      fprintf(2, "add: invalid character\n");
      exit(-1);
    }
    buf[i++] = c;
  }
  if (c != '\n' && c != '\r') {
    fprintf(2, "add: buffer overflow\n");
    exit(-1);
  }
  buf[i] = '\0';

  printf("%s|\n", buf);
  
  int b = atoi(buf);

  printf("%d\n", a + b);
}
