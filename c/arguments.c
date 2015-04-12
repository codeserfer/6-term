#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char * argv[])
{

    int opt;

    while ((opt=getopt(argc, argv, "i:0:")))
    {
        switch (opt)
        {
            case 'i':
            printf("%s", optarg);
            break;
        }
    }

    return 0;
}

