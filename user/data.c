#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
  
static const uint64 SEC_PER_DAY = 24 * 3600;

void
print(int n)
{
  if (n < 10)
    printf("0");
  printf("%d", n);
}

int
main(int argc, char *argv[])
{
  uint64 rtc = getrtc();
  uint64 total_sec = rtc / 1000000000;
  uint nanosec = (uint)(rtc % 1000000000);

  uint64 days = total_sec / SEC_PER_DAY;
  uint64 sec_of_day = total_sec % SEC_PER_DAY;

  int year = 1970;
  for (;;) {
    uint days_in_year = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ? 366 : 365;
    if (days < days_in_year)
      break;
    days -= days_in_year;
    year++;
  }

  int month_days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    month_days[1] = 29;

  int month = 0;
  while (month < 12 && days >= month_days[month]){
    days -= month_days[month];
    month++;
  }

  int day = (int)days + 1;
  month++;

  int hour   = sec_of_day / 3600;
  int minute = (sec_of_day % 3600) / 60;
  int second = sec_of_day % 60;

  printf("%d-", year);
  print(month);  printf("-");
  print(day);    printf(" ");
  print(hour);   printf(":");
  print(minute); printf(":");
  print(second); printf(".");

  char frac[10];
  for(int i = 8; i >= 0; i--){
    frac[i] = '0' + (nanosec % 10);
    nanosec /= 10;
  }
  frac[9] = '\0';
  printf("%s\n", frac);
  exit(0);
}
