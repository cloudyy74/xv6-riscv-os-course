#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) {
  int cnt = ps_listinfo(0, 0);
  if (cnt < 0) {
    fprintf(2, "ps: error getting procces count\n");
    exit(1);
  }
  struct procinfo *plist = malloc(cnt * sizeof(struct procinfo));
  if (!plist) {
    fprintf(2, "ps: malloc failed\n");
    exit(1);
  }
  
  int ret;
  while ((ret = ps_listinfo(plist, cnt)) == -1) {
    cnt *= 2;
    free(plist);
    plist = malloc(cnt * sizeof(struct procinfo));
    if (!plist) {
      fprintf(2, "ps: malloc failed\n");
      exit(1);
    }
  }

  if (ret < 0) {
    free(plist);
    fprintf(2, "ps: syscall ps_listinfo error\n");
    exit(1);
  }

  printf("PID\tNAME\tSTATE    \tPPID \tPNAME\n");

  for (int i = 0; i < cnt; i++) {
    struct procinfo *pi = &plist[i];
    char *pname = "";
    if (pi->ppid != -1) {
      for (int j = 0; j < cnt; j++) {
        if (plist[j].pid == pi->ppid) {
          pname = plist[j].name;
          break;
        }
      }
    }
    char *state;
    switch (pi->state) {
      case unused: state = "UNUSED"; break;
      case used: state = "USED"; break;
      case sleeping: state = "SLEEPING"; break;
      case runnable: state = "RUNNABLE"; break;
      case running: state = "RUNNING"; break;
      case zombie: state = "ZOMBIE"; break;
      default: state = "???";
    }

    printf("%d\t%s\t%s \t%d \t%s\n", pi->pid, pi->name, state, pi->ppid, pname);
  }

  free(plist);
  exit(0);
}
