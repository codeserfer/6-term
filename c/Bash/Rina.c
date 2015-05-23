// Лабораторная работа
// по дисциплине «Программирование в среде UNIX»
// Вариант №1

// Шмелева Е.К.
// ДКО_121б

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <getopt.h>
#include <regex.h>

#define PROC_FS "/proc"
#define CMDLINE_LENGTH 1024
#define CMDS_LENGTH 256
#define PARAMS_LENGTH 256

typedef struct proc_t
{
	int pid;
	char cmd[256];
	unsigned char state;
	int ppid;
	int pgrp;
	int session;
	int tty_nr;
	int tpgid;
	uint flags;
	uint minflt;
	uint cminflt;
	uint majflt;
	uint cmajflt;
	int utime;
	int stime;
	int cutime;
	int cstime;
	int priority;
	uint nice;
	int zero;
	uint itrealvalue;
	int starttime;
	uint vsize;
	uint rss;
	uint rlim;
	uint startcode;
	uint endcode;
	uint startstack;
	uint kstkesp;
	uint kstkeip;
	int signal;
	int blocked;
	int sigignore;
	int sigcatch;
	uint wchan;

	int uid;
	time_t ctime;
} proc_t;

bool active = true;

struct dirent *entry;

// <summary>
// Получение сведений об активных процессах.
// </summary>
void ReadProcStat(struct proc_t *ps, char* fname)
{
	char path[256];

	struct stat st;

	sprintf(path,"/%s/%s/stat", PROC_FS, fname);

	FILE *file = fopen(path,"r");
	fscanf(file,"%d %s %c %d %d %d %d %u %u %u %u %u %d %d %d %d %d %u %d %u %d %u %u %u %u %u %u %u %u %d %d %d %u",
		&ps->pid, ps->cmd, &ps->state, &ps->ppid, &ps->pgrp, &ps->session,
		&ps->tty_nr, &ps->tpgid, &ps->flags, &ps->minflt, &ps->cminflt,
		&ps->majflt, &ps->cmajflt, &ps->utime, &ps->stime, &ps->cutime,
		&ps->cstime, &ps->priority, &ps->nice, &ps->zero, &ps->itrealvalue,
		&ps->starttime, &ps->vsize, &ps->rss, &ps->rlim, &ps->startcode,
		&ps->endcode, &ps->startstack, &ps->kstkesp, &ps->kstkeip, &ps->signal,
		&ps->blocked, &ps->sigignore, &ps->sigcatch, &ps->wchan
	);

	sprintf(path,"/%s/%s", PROC_FS, fname);
	lstat(path,&st);

	ps->uid = st.st_uid;
	ps->ctime = st.st_ctime;
	fclose(file);
}

// <summary>
// Рекурсивный подсчет размера файлов в каталоге.
// </summary>
int64_t RecursiveCount()
{
    int64_t sum = 0;
    DIR *dir_p;
    struct dirent *dir_entry_p;

    dir_p = opendir(".");

    while (dir_entry_p = readdir(dir_p))
    {
        struct stat stat2;
        stat (dir_entry_p->d_name, &stat2);

        if (dir_entry_p->d_name[0]!='.') //текущая/
        {
            if (S_ISDIR(stat2.st_mode))
            {
                chdir(dir_entry_p->d_name);
                sum += RecursiveCount();
                chdir("..");
            }
            else
            {
                sum += stat2.st_size;
            }
        }
    }

    closedir(dir_p);
    return sum;
}

#define SHCMD(x) int shcmd_##x (char* cmd, char* params[])
#define SHCMD_EXEC(x) shcmd_##x (params[0], params)
#define IS_CMD(x) strncmp(#x,cmd,strlen(cmd)) == 0

// <summary>
// Вывод текста, указанного в параметре.
// </summary>
SHCMD (echo)
{
    int np = 0;
    bool nParam = false;

    while (params[np] != NULL)
    {
        np++;
    }

    int c;
    while ((c = getopt(np, params, "n"))!= -1)
	{
        switch (c)
		{
			case 'n':
				nParam = true;
				break;
			default:
				printf ("Error!\n", c);
        }
    }

    while (optind < np)
    {
        printf("%s ", params[optind++]);
    }

    if (!nParam)
        printf("\n");

    return 0;
}

// <summary>
// Переход в каталог, указанный в параметре,
// или в домашний каталог, если параметр не указан.
// </summary>
SHCMD(cd)
{
    char* oldDir = getenv("PWD");

    if (!params[1] || !strcmp(params[1], "~"))
    {
        params[1] = getenv("HOME");
    }

    if (!strcmp(params[1], "-"))
    {
        params[1] = getenv("OLDPWD");
    }

    chdir(params[1]);

    char currentDir [256];
    getcwd(currentDir, 256);

    setenv ("PWD", currentDir, 1);
    setenv("OLDPWD", oldDir, 1);

	return 0;
}

// <summary>
// Вывод имени текущего каталога.
// </summary>
SHCMD(pwd)
{
	printf("%s\n",getenv("PWD"));
	return 0;
}

// <summary>
// Подсчет размера файлов в каталоге.
// </summary>
SHCMD(du)
{
    int np = 0;
    while (params[np])
    {
        np++;
    }

    struct stat stat_p;
    stat (params[np - 1], &stat_p);

    if (np != 2)
    {
        printf("Error.\n");
        return -1;
    }

    if(params[1][0] == '-')
    {
        if(params[1][1] == 's')
        {
            printf("%d\n", RecursiveCount());
        }
    }
    else
    {
        if(S_ISDIR(stat_p.st_mode))
        {
            chdir(params[np - 1]);
            printf("%ld\n", RecursiveCount());
        }
        else
        {
            printf("%ld\n", stat_p.st_size);
        }
    }

    return 0;
}

// <summary>
// Вывод списка файлов в каталоге.
// </summary>
SHCMD(ls)
{
    bool lParam = false;
    int c, np = 0, intArg;

    while (params[np])
    {
        np++;
    }

    while ((c = getopt(np, params, "l"))!= -1)
    {
        switch (c)
        {
        case 'l':
            lParam = true;
            break;
        default:
            printf ("Error!\n", c);
        }
    }

    if (np == optind)
    {
        params[np++] = ".";
    }

	DIR *D;
    struct dirent *de;
    struct stat st;
    struct passwd *pwd;
    struct group *grp;
    int mode,right_ind;
    static char* rights = "rwxrwxrwx";
    char *nuser,*ngroup;

    while (optind < np)
    {
        printf("%s:\n", params[optind]);
        D = opendir(params[optind++]);

        while (de = readdir(D))
        {
            if (de->d_name[0] != '.')
            {
                lstat(de->d_name, &st);
                mode = st.st_mode;
                if (lParam)
                {
                    S_ISDIR(mode) ? printf("d") : printf("-");
                }

                mode &= 0777;
                right_ind = 0;

                while (right_ind < 9 )
                {
                    if (lParam)
                    {
                        mode&256?printf("%c",rights[right_ind]):printf("-");
                    }
                    mode <<= 1;
                    right_ind++;
                }

                if (pwd=getpwuid(st.st_uid))
                {
                    nuser=pwd->pw_name;
                }
                else
                {
                    sprintf(nuser,"%d",st.st_uid);
                }

                if (grp = getgrgid(st.st_gid))
                {
                    ngroup=grp->gr_name;
                }
                else
                {
                    sprintf(ngroup,"%d",st.st_gid);
                }

                if (lParam)
                {
                    printf("%3d %s\t%s %8d %s\n",st.st_nlink,nuser,ngroup,st.st_size,de->d_name);
                }
                else
                {
                    printf("%s\n", de->d_name);
                }
            }
        }
        closedir(D);
        printf("\n");
    }

    return 0;
}

// <summary>
// Вывод сведений об активных процессах.
// </summary>
SHCMD(ps)
{
    struct proc_t ps;
	DIR *Dir = opendir(PROC_FS);
	char time[256];

	printf("UID\t  PID  PPID  C\tSTIME\tTTY\tTIME\tCMD\n");

	while (entry = readdir(Dir))
    {
		if(!isdigit(*entry->d_name))
			continue;

		ReadProcStat(&ps,entry->d_name);
		printf("%s\t",getpwuid(ps.uid)->pw_name);

		printf("%5d ",ps.pid);
		printf("%5d ",ps.ppid);
		printf("%2d\t",ps.nice);

		strftime(time, sizeof(time), "%H:%M", localtime(&ps.ctime));
		printf("%s\t",time);

		printf("\t");
		printf("\t");
		printf("%s\t",ps.cmd);
		printf("\n");
	}

	closedir(Dir);

    return 0;
}

// <summary>
// Выход из интерпретатора.
// </summary>
SHCMD(exit)
{
	active = false;
	return 0;
}

// <summary>
// Выполнение команды.
// </summary>
// <param name="cmd">Команда с параметрами.</param>
void Execute(char *cmd)
{
    // Параметры команды, разделенные пробелами.
	char *params[PARAMS_LENGTH];
	char *token;
	int np;

	token = strtok(cmd, " ");
	np = 0;
	while (token && np < PARAMS_LENGTH - 1)
	{
		params[np++] = token;
		token = strtok(NULL, " ");
	}
	params[np] = NULL;

	// Выполнение встроенных команд.
    if (IS_CMD(echo))
    {
        SHCMD_EXEC(echo);
    }
    else if (IS_CMD(cd))
    {
        SHCMD_EXEC(cd);
    }
	else if (IS_CMD(pwd))
    {
        SHCMD_EXEC(pwd);
    }
	else if (IS_CMD(du))
    {
        SHCMD_EXEC(du);
    }
    else if (IS_CMD(ls))
    {
        SHCMD_EXEC(ls);
    }
     else if (IS_CMD(ps))
    {
        SHCMD_EXEC(ps);
    }
	else if (IS_CMD(exit))
    {
        SHCMD_EXEC(exit);
    }
    else
	{
	    // Вызов внешних команд.
		execvp(params[0], params);

		// Ошибка запуска.
		perror("exec");
	}
}

// <summary>
// Конвейер.
// </summary>
// <param name="cmds">Команды.</param>
// <param name="count">Количество команд.</param>
// <param name="current">Обрабатываемая в настоящий момент команда.</param>
int ExecutePipeline(char *cmds[], int count, int current)
{
	int fd[2];

	if (pipe(fd) < 0)
	{
		printf("Cannot create a pipe\n");
		return 1;
	}

    // Команды с первой по (count - 2).
	if (count > 1 && current < count - 2)
	{
	    // Исполнение команды.
		if (fork() == 0)
		{
			dup2(fd[1], 1);

			close(fd[0]);
			close(fd[1]);

			Execute(cmds[current]);

			exit(0);
		}

        // Рекурсивный вызов конвейера.
		if(fork() == 0)
		{
			dup2(fd[0], 0);

			close(fd[0]);
			close(fd[1]);

			ExecutePipeline(cmds, count, ++current);

			exit(0);
		}
	}
	// Последние две команды или первая команда, если она является единственной.
	else
	{
	    // Для команды exit без вызова fork.
		if (count == 1 && (!strcmp(cmds[0],"exit") || !strncmp(cmds[0],"cd", 2)))
		{
			close(fd[0]);
			close(fd[1]);

			Execute(cmds[current]);

			return 0;
		}

		if(fork() == 0)
		{
			if (count > 1)
            {
                dup2(fd[1], 1);
            }

			close(fd[0]);
			close(fd[1]);

			Execute(cmds[current]);

			exit(0);
		}

		if(count > 1 && fork() == 0)
		{
			dup2(fd[0], 0);

			close(fd[0]);
			close(fd[1]);

			Execute(cmds[current+1]);

			exit(0);
		}
	}

	close(fd[0]);
	close(fd[1]);

    // Ожидание завершения всех порожденных процессов.
    int i;
	for (i = 0 ; i < count; i++)
    {
        wait(0);
    }

	return 0;
}

void PreExecute(char* command)
{
    char *p, *token;
    int cmd_cnt;
    char *cmds[CMDS_LENGTH];

    if((p = strstr(command, "\n")) != NULL)
    {
        p[0] = 0;
    }

    token = strtok(command, "|");

    int cmdcount;
	for (cmdcount = 0; token && cmdcount < CMDS_LENGTH; cmdcount++)
    {
        cmds[cmdcount] = strdup(token);
        token = strtok(NULL, "|");
    }
	cmds[cmdcount] = NULL;

	if (cmdcount > 0)
	{
		ExecutePipeline(cmds, cmdcount, 0);
	}
}

// <summary>
// Обработка переменных окружения.
// </summary>
char* ReplaceVariables(char *str)
{
    char *env;
    char buf[256];
    regex_t preg;
    regmatch_t pm;
    char *newstr = strdup (str);
    int len;

    regcomp( &preg, "\\$\\w\\+\\b", REG_ICASE );



    while ( regexec(&preg, str, 1, &pm, REG_NOTBOL) == 0 )
    {
        memset(buf, '\0', 256);
        strncpy(buf,&str[pm.rm_so]+1,(pm.rm_eo-pm.rm_so-1 < 256)?pm.rm_eo-pm.rm_so-1:254);
        env = getenv(buf);

        if (!env)
        {
            printf("Variable didn't found!");
            return NULL;
        }
        len = strlen(str)-strlen(buf)+strlen(env);
        newstr=malloc(len);
        memset(newstr, '\0', len);
        strncpy(newstr,str,pm.rm_so);
        strncat(newstr,env,strlen(env));
        strncat(newstr,&str[pm.rm_eo],strlen(str)-pm.rm_eo);
        str = newstr;
    }
    regfree(&preg);

    return newstr;
}

// <summary>
// Чтение строки и запуск конвейера.
// </summary>
int main(int argc, char** argv)
{
    if (argc > 1)
    {
        FILE* file = fopen(argv[1], "r");
        if (!file)
        {
            printf("Can't open file!\n");
            return 0;
        }

        char str[CMDLINE_LENGTH];

        while (fgets(str, sizeof(str), file))
        {
            PreExecute(ReplaceVariables(str));
        }
    }
    else
    {
        while(active)
        {
            if (getenv("PS1"))
            {
                printf("%s", getenv("PS1"));
            }
            else
            {
                printf("[%s]# ",getenv("PWD"));
            }

            fflush(stdout);

            char cmdline[CMDLINE_LENGTH];
            fgets(cmdline,CMDLINE_LENGTH,stdin);

            PreExecute(ReplaceVariables(cmdline));
            //PreExecute((cmdline));
        }
    }

	return 0;
}

