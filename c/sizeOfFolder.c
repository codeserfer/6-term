#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


int SizeOfDir ()
{
        int size = 0;
        DIR* d = opendir(".");
        struct dirent *de;
        while ((de = readdir(d))!=0)
        {
            if (de->d_name[0]!='.')
            {
                struct stat st;
                lstat(de->d_name,&st);

                int mode = st.st_mode;

                if (S_ISDIR(mode))
                {
                    chdir(de->d_name);
                    size+=SizeOfDir ();
                    chdir ("..");
                }
                else
                {
                    size+=st.st_size;
                }
            }
        }

        closedir(d);
        return size;
}


int main()
{
    chdir("/home/codeserfer/Dropbox/Code_Blocks/28.03.2015");
    printf("%d\n", SizeOfDir ("/home/codeserfer/Dropbox/Code_Blocks/28.03.2015"));

    //symlink("main.c", "main.c.symlink");
    //link ("main.c", "main.c.link");





    return 0;
}

