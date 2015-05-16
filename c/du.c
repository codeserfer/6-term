#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h>

int64_t Recursive_count()
{
  int64_t sum = 0;
  DIR           *dir_p;
  struct dirent *dir_entry_p;

  dir_p = opendir(".");

  while( NULL != (dir_entry_p = readdir(dir_p)))
  {
    struct stat stat2;
    stat (dir_entry_p->d_name, &stat2);
    if(dir_entry_p->d_name[0]!='.')
    {
      if(S_ISDIR(stat2.st_mode))
      {
        chdir(dir_entry_p->d_name);
        sum+=Recursive_count();
        chdir("..");
      }
      else
        sum += stat2.st_size;
    }
  }

  closedir(dir_p);

  return sum;
}

int main(int argc, char *argv[])
{
  struct stat stat_p;
  stat (argv[argc-1], &stat_p);

  if (argc<2)
  {
    printf("Error! Input filename.\n");
    return -1;
  }

  if (argc==2)
  {
    if(argv[1][0]=='-')
    {
      if(argv[1][1]=='s')
      {
        printf("%d\n",Recursive_count());
      }
    }
    else
    {
      if(S_ISDIR(stat_p.st_mode))
      {
        chdir(argv[argc-1]);
        printf("%ld\n",Recursive_count());
      }
      else printf("%ld\n",stat_p.st_size);
    }
    return 0;
  }
}
