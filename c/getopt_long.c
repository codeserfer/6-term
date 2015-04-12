#include <iostream>
#include <getopt.h>

using namespace std;

int main(int argc, char * argv[])
{

    int opt;

    /*while ((opt=getopt(argc, argv, "i:o:"))!=-1)
    {
        switch (opt)
        {
            case 'i':
            cout << "i" << optarg << endl;
            break;

            case 'o':
            cout << "o" << optarg << endl;
            break;
        }*/

        opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
    while(opt != -1 ) {
        switch( opt ) {
            case 'I':
                globalArgs.noIndex = 1; /* true */
                break;

            case 'l':
                globalArgs.langCode = optarg;
                break;

            case 'o':
                globalArgs.outFileName = optarg;
                break;

            case 'v':
                globalArgs.verbosity++;
                break;

            case 'h':   /* намеренный проход в следующий case-блок */
			case '?':
                display_usage();
                break;

             case 0:     /* длинная опция без короткого эквивалента */
			 if( strcmp( "randomize", longOpts[longIndex].name ) == 0 ) {
                    globalArgs.randomized = 1;
                }
                break;

            default:
                /* сюда попасть невозможно. */
				break;
        }

        opt = getopt_long( argc, argv, optString, longOpts, amp;longIndex );
    }
    }



    return 0;
}

