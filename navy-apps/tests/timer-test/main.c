#include <stdio.h>
#include <time.h>
#include <sys/_timeval.h>
int gettimeofday(struct timeval* tv, struct timezone* tz);
int main() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("seconds since 00:00:00, 1 Jan 1970 UTC: %ld \n",  tv.tv_sec);
    sleep(1500);
    printf("seconds since 00:00:00, 1 Jan 1970 UTC: %ld \n",  tv.tv_sec);
}