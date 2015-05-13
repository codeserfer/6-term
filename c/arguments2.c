/*#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char * argv[])
{

    int opt;


    argc = 3;
    argv[1] = "-i";
    argv[2] =  "55";
    argv[3] = "file.txt";
    printf("argc: %d\nargv: %s\n", argc, argv[1]);

    while ((opt=getopt(argc, argv, "i:0:"))!=-1)
    {
        switch (opt)
        {
            case 'i':
            printf("%s\n", optarg);
            break;
        }
    }
    printf("nonOption: %d\n", optind);

    return 0;
}
*/


#include <stdio.h>     /* для printf */
#include <stdlib.h>    /* для exit */
#include <getopt.h>

int main (int argc, char **argv) {
    int c;
    int digit_optind = 0;

    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        //Если после опции стоит двоеточие, то она требует аргумента
        c = getopt(argc, argv, "abc:d:012p:");
        if (c == -1)
            break;

        switch (c) {
        case 0:
            //printf ("параметр %s", long_options[option_index].name);
            if (optarg)
                printf (" с аргументом %s", optarg);
            printf ("\n");
            break;

        case '0':
        case '1':
        case '2':
            if (digit_optind != 0 && digit_optind != this_option_optind)
              printf ("цифры используются в двух разных элементах argv.\n");
            digit_optind = this_option_optind;
            printf ("параметр  %c\n", c);
            break;

        case 'a':
            printf ("параметр a\n");
            break;

        case 'b':
            printf ("параметр b\n");
            break;

        case 'c':
            printf ("параметр c со значением `%s'\n", optarg);
            break;

        case 'd':
            printf ("параметр d со значением `%s'\n", optarg);
            break;

        case 'p':
            printf ("параметр p со значением `%s'\n", optarg);
            break;

        case '?':
            break;

        default:
            printf ("?? getopt возвратило код символа 0%o ??\n", c);
        }
    }

    if (optind < argc) {
        printf ("элементы ARGV, не параметры: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }

    exit (0);
}

