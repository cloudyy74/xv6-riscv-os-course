#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc!=3){ 
    fprintf(2,"Usage: hexdump file n\n"); 
    exit(1); 
  }
  int n = atoi(argv[2]); 
  int fd = open(argv[1], O_RDONLY);

  if (fd < 0){ 
    fprintf(2,"hexdump: open fail\n"); 
    exit(1); 
  }
  unsigned char buf[16];
  for (int total = 0, r; total < n && (r = read(fd, buf, (n - total > 16 ? 16 : n-total))) > 0;){
    total += r;
    for (int i = 0; i < r; i++){
      char h[3] = { "0123456789abcdef"[buf[i]>>4],
                    "0123456789abcdef"[buf[i]&0xf], ' ' };
      write(1,h,3);
    }
  }
  write(1,"\n",1);
  close(fd);
  exit(0);
}
