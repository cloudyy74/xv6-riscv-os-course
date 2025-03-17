enum proc_state { unused, used, sleeping, runnable, running, zombie };

struct procinfo {
  int pid;
  char name[16];
  enum proc_state state;
  int ppid;
};
