#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>



int main()
{
    struct sysinfo myinfo;
    if (sysinfo (&myinfo)<0)
    {
        printf("error!");
        return 0;
    }
    printf("%s\t\t%s\t\t%s", "total", "used", "free");
    printf("%s", "\n");
    printf("Memory:\n");
    printf("%ul\t%ul\t%ul", myinfo.totalram, myinfo.totalram - myinfo.freeram, myinfo.freeram);
    printf("\nSwap:\n");
    printf("%ul\t%ul\t%ul", myinfo.totalswap, myinfo.totalswap - myinfo.freeswap, myinfo.freeswap);



    return 0;
}

