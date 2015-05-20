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

#define PROC_FS "/proc"
#define LINELENGHT 1024

typedef struct proc_t
{ // http://www.protocols.ru/modules.php?name=News&file=article&sid=57
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
//additional
	int uid;
	time_t ctime;
} proc_t;


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


struct dirent *entry;


int shellActive = 1; // требуется для команды exit

//  команды оформлены в виде макросов
//  доп. информация http://en.wikipedia.org/wiki/C_preprocessor

#define SHCMD(x) int shcmd_##x (char* cmd, char* params[])
#define SHCMD_EXEC(x) shcmd_##x (params[0], params)
#define IS_CMD(x) strncmp(#x,cmd,strlen(cmd)) == 0


SHCMD(pwd)
{
	printf("%s\n",getenv("PWD"));
	return 0;
}

SHCMD(exit)
{
	shellActive = 0;
	printf("Bye, bye!\n");
	return 0;
}

SHCMD(cd)
{
    char* oldDir = getenv("PWD");

    if (!params[1] || !strcmp(params[1], "~"))
        params[1] = getenv("HOME");

    if (!strcmp(params[1], "-"))
        params[1] = getenv("OLDPWD");

    chdir(params[1]);

    char currentDir [200];
    getcwd(currentDir, 200);

    setenv ("PWD", currentDir, 1);
    setenv("OLDPWD", oldDir, 1);

	return 0;
}


SHCMD(ls)
{
    bool lParam = false;
    int c, np = 0, intArg;

    while (params[np]!=NULL)
        np++;

    while ((c = getopt(np, params, "l"))!=-1) {
        switch (c) {

        case 'l':
            lParam = true;
            break;
        default:
            printf ("Error!\n", c);
        }
    }


    if (np == optind) params[np++] = ".";


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

    while( de = readdir(D) )
    {
        if( de->d_name[0] != '.' )
        {
            lstat(de->d_name,&st);
            mode = st.st_mode;
            if (lParam) S_ISDIR(mode)?printf("d"):printf("-");
            mode &= 0777;
            right_ind = 0;
            while( right_ind < 9 )
            {
                if (lParam) mode&256?printf("%c",rights[right_ind]):printf("-");
                mode <<= 1;
                right_ind++;
            }
            if (pwd=getpwuid(st.st_uid))
                nuser=pwd->pw_name;
            else
                sprintf(nuser,"%d",st.st_uid);

            if (grp=getgrgid(st.st_gid))
                ngroup=grp->gr_name;
            else
                sprintf(ngroup,"%d",st.st_gid);

            if (lParam)
                printf("%3d %s\t%s %8d %s\n",st.st_nlink,nuser,ngroup,st.st_size,de->d_name);
            else
                printf("%s\n", de->d_name);
        }
    }
    closedir(D);
    printf("\n");
    }

    return 0;
}

SHCMD(ps)
{

    struct proc_t ps;

	DIR *Dir = opendir(PROC_FS);

	char time[256];

	printf("UID\t  PID  PPID  C\tSTIME\tTTY\tTIME\tCMD\n");
	while (entry = readdir(Dir)) {
		if(!isdigit(*entry->d_name))
			continue;
		ReadProcStat(&ps,entry->d_name);
		printf("%s\t",getpwuid(ps.uid)->pw_name);
		printf("%5d ",ps.pid);
		printf("%5d ",ps.ppid);
		printf("%2d\t",ps.nice);//?

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

SHCMD (echo)
{
    int np = 0;
    bool nParam = false;

    while (params[np]!=NULL)
        np++;

    int c;
    while ((c = getopt(np, params, "n"))!=-1) {
        switch (c) {

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

SHCMD(du)
{

    int np = 0;
    while (params[np]!=NULL)
        np++;

    struct stat stat_p;
    stat (params[np-1], &stat_p);

    if (np<2)
    {
        printf("Error! Input filename.\n");
        return -1;
    }

    if (np==2)
    {
        if(params[1][0]=='-')
        {
            if(params[1][1]=='s')
            {
                printf("%d\n",Recursive_count());
            }
        }
        else
        {
            if(S_ISDIR(stat_p.st_mode))
            {
                chdir(params[np-1]);
                printf("%ld\n",Recursive_count());
            }
            else
                printf("%ld\n",stat_p.st_size);
        }
    return 0;
    }
}

// выполнение команды с параметрами
void MyExec(char *cmd)
{
	char *params[256];  //параметры команды, разделенные пробелами
	char *token;
	int np;

	token = strtok(cmd, " "); //Разделение на подстроки
	np = 0;
	while( token && np < 255 )
	{
		params[np++] = token;
		token = strtok(NULL, " ");
	}
	params[np] = NULL;

	// выполнение встроенных команд
	if( IS_CMD(pwd) )
		SHCMD_EXEC(pwd);
	else
	if( IS_CMD(exit) )
		SHCMD_EXEC(exit);
    else
    if( IS_CMD(cd) )
		SHCMD_EXEC(cd);
    else
    if( IS_CMD(ls) )
		SHCMD_EXEC(ls);
    else
    if( IS_CMD(ps) )
		SHCMD_EXEC(ps);
    else
    if( IS_CMD(echo) )
		SHCMD_EXEC(echo);
    else
    if( IS_CMD(du) )
		SHCMD_EXEC(du);
    else
	{
	// иначе вызов внешней команды
		execvp(params[0], params);
		perror("exec"); // если возникла ошибка при запуске
	}
}
// рекурсивная функция обработки конвейера
// параметры: строка команды, количество команд в конвейере, текущая (начинается с 0 )
int ExecConv(char *cmds[], int n, int curr)
{
	int fd[2],i;
	if( pipe(fd) < 0 )
	{
		printf("Cannot create pipe\n");
		return 1;
	}

	if( n > 1 && curr < n - 2 )
	{ // first n-2 cmds
		if( fork() == 0 )
		{
			dup2(fd[1], 1);
			close(fd[0]);
			close(fd[1]);
			MyExec(cmds[curr]);
			exit(0);
		}
		if( fork() == 0 )
		{
			dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
			ExecConv(cmds,n,++curr);
			exit(0);
		}
	}
	else
	{ // 2 last cmds or if only 1 cmd
		if(n == 1 && (!strcmp(cmds[0],"exit") ||  !strncmp(cmds[0],"cd", 2)) )
		{ // для случая команды exit обходимся без fork, иначе не сможем завершиться
			close(fd[0]);
			close(fd[1]);
			MyExec(cmds[curr]);
			return 0;
		}
		if( fork() == 0 )
		{
			if( n > 1 ) // if more then 1 cmd
				dup2(fd[1], 1);
			close(fd[0]);
			close(fd[1]);
			MyExec(cmds[curr]);
			exit(0);
		}
		if( n > 1 && fork()==0 )
		{
			dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
			MyExec(cmds[curr+1]);
			exit(0);
		}
	}
	close(fd[0]);
	close(fd[1]);

	for( i = 0 ; i < n ; i ++ ) // ожидание завершения всех дочерних процессов
		wait(0);

	return 0;
}

void Do (char* command)
{
    char *p, *token;
    int cmd_cnt;
    char *cmds[256];

    if( (p = strstr(command,"\n")) != NULL ) p[0] = 0;

    token = strtok(command, "|");
	for(cmd_cnt = 0; token && cmd_cnt < 256; cmd_cnt++ )
	{
		cmds[cmd_cnt] = strdup(token);
		token = strtok(NULL, "|");
	}
	cmds[cmd_cnt] = NULL;

	if( cmd_cnt > 0 )
	{
		ExecConv(cmds,cmd_cnt,0);
	}
}


// главная функция, цикл ввода строк (разбивка конвейера, запуск команды)
int main(int argc, char** argv)
{

    if (argc>1)
    {
        FILE* file = fopen(argv[1], "r");
        if (!file)
        {
            printf("Can't open file!\n");
            return 0;
        }

        char str [LINELENGHT];

        while(fgets(str,sizeof(str),file))
        {
            Do (str);
        }
    }
    else
    {
        while( shellActive )
        {

            if (getenv("PS1"))
                printf("%s", getenv("PS1"));
            else
                printf("[%s]# ",getenv("PWD"));

            fflush(stdout);

            char cmdline[1024];
            fgets(cmdline,1024,stdin);

            Do (cmdline);
        }
    }

	return 0;
}

