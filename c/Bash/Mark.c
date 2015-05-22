//Тут куча нужных библиотек
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
#include <ctype.h>
#include <fcntl.h>
#include <sys/sendfile.h>


#define PROC_FS "/proc" //В этой директории рассполагаются процессы. Это надо для функции ps
#define LINELENGHT 1024 //Максимальная длина строки

//Структура, куда сохраняется информация о текущих активных процессах
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


//Чтение информации об активных процессах
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


struct dirent *entry;


int shellActive = 1; // требуется для команды exit

//  команды оформлены в виде макросов
//  доп. информация http://en.wikipedia.org/wiki/C_preprocessor

#define SHCMD(x) int shcmd_##x (char* cmd, char* params[])
#define SHCMD_EXEC(x) shcmd_##x (params[0], params)
#define IS_CMD(x) strncmp(#x,cmd,strlen(cmd)) == 0

//Вывести адрес текущего каталога
SHCMD(pwd)
{
    //Просто выводим переменную окружения $PWD
	printf("%s\n",getenv("PWD"));
	return 0;
}

//Выход из баша
SHCMD(exit)
{
    //Меняем флажок, отвечающий за активность баша на 0
	shellActive = 0;
	printf("Bye, bye!\n");
	return 0;
}

//Смена текущей директории
SHCMD(cd)
{
    //Сохраняем старую директорию (чтобы работало cd -)
    char* oldDir = getenv("PWD");

    //Подготовка. Если используются команды cd - или cd ~

    //Переход в домашнюю директорию, если cd ~
    if (!params[1] || !strcmp(params[1], "~"))
        params[1] = getenv("HOME");

    //Если cd -, то переходим в старую директорию
    if (!strcmp(params[1], "-"))
        params[1] = getenv("OLDPWD");

    //Непосредственно смена активной директории
    chdir(params[1]);

    //Смена переменных окружения
    char currentDir [200];
    getcwd(currentDir, 200);

    setenv ("PWD", currentDir, 1);
    setenv("OLDPWD", oldDir, 1);

	return 0;
}

//Вывод файла с нумерацией строк
void nl(char* filePath)
{
    //Открываем файл только для чтения
	FILE* file = fopen(filePath, "r");

	//Если файл не найден
	if (!file)
	{
		perror("Can't open file");
		return;
	}


	char str [LINELENGHT];
	int i=1;

	//Считываем файл построчно и выводим его вместе с номером строки
	while(fgets(str,sizeof(str),file))
		printf("%i\t%s",i++,str);

    //закрываем файл
	fclose(file);
}

//Вывод всего файла
void cat(char* filePath)
{
    //Открываем файл только для чтения
	FILE* file = fopen(filePath, "r");

	//Если файл не найден
	if (!file)
	{
		perror("Can't open file");
		return;
	}


	int ch;
	//Считываем файл посимвольно
	while ((ch = fgetc(file)) != EOF)

        //Мы должны выводить только печатаемые символы, поэтому опознаем, можем ли мы напечатать этот символ
		if ((ch > '!' && ch < '~') || ch == '\n')		//readable
			printf("%c",ch);
		else
            //Если не можем, то печатаем пробел
			printf("%c",' ');			//unreadable
	fclose(file);
}


//Подсчет количества слов в файле
int wc_W(char* filePath)	//words
{
	FILE* file = fopen(filePath, "r");
	if (!file)
	{
		perror("Can't open file");
		return -1;
	}

	int count = 0;
	int ch;
	bool state = false;

    //Если встретили разделитель слов (пробел, табуляция, перенос строк), то инкрементируем счетчик
	while ((ch = fgetc(file)) != EOF)
		if (ch == ' ' || ch == '\n' || ch == '\t')		//end of word
			state = false;
		else if (state == false)
		{
			state = true;
			count++;
		}

	fclose(file);
	return count;
}

//Подсчет количества символов в файле
int wc_M(char* filePath)	//symbols
{
	FILE* file = fopen(filePath, "r");
	if (!file)
	{
		perror("Can't open file");
		return -1;
	}

	int count = 0;

	while (fgetc(file) != EOF)
		count++;

	fclose(file);
	return count;
}

//Подсчет количества строк в файле
int wc_L(char* filePath)	//strings
{
	FILE* file = fopen(filePath, "r");
	if (!file)
	{
		perror("Can't open file");
		return -1;
	}

	int count = 1;
	int ch;
	bool state = false;

	while ((ch = fgetc(file)) != EOF)
	{
		if (state == true)
		{
			count++;
			state = false;
		}
		if (ch == '\n')            //end of line
			state = true;
	}

	fclose(file);
	return count;
}


//Выводит в зависимости от параметров или количество символов, или количество слов, или количество строк в файле
void wc(char* filePath, char key)
{
	switch (key)
	{
        //В зависимости от переданного параметра вызываем нужную функцию
		case 'w':
			printf("%i", wc_W(filePath));
			break;
		case 'm':
			printf("%i", wc_M(filePath));
			break;
		case 'l':
			printf("%i", wc_L(filePath));
			break;
		default:
			printf("%i %i %i", wc_L(filePath),wc_W(filePath),wc_M(filePath));
			break;
	}
	printf(" %s\n", filePath);
}

//Создание ссылки. Просто Используем системную функцию
void linkM(char* path1, char* path2)
{
	if (link(path1, path2) != 0)
		perror("Couldn't make a link");
}

//Грубо говоря в нашем случае поиск подстроки в файле
//Если подстрока найдена, то выводим на консоль всю строку
void grep(char* filePath, char* string)
{
	FILE* file = fopen(filePath, "r");
	if (!file)
	{
		perror("Can't open file");
		return;
	}

	char str [LINELENGHT];
	while(fgets(str,sizeof(str),file))
		if (strstr(str,string))
			printf("%s",str);

	fclose(file);
}

/*
*   Далее идет вызов выше написанных функций в ответ на соответствующие команды
*   Этот страннный SHCMD () черт знает как работает, это макросы в сишке
*   Иванов сам написал это, я лишь делал по образцу. Вряд ли он будет что-то спрашивать тут
*/

//Вывод всего файла
//Пример вызова:
// cat file
SHCMD(cat)
{
    //Здесь и везде тут происходит
    //Считание количества переданных параметров
    int np = 0;
    while (params[np]!=NULL)
        np++;

    //Теперь для каждого переданного параметра вызываем уже написанную функцию
    while (optind < np)
    {
        cat (params[optind++]);
    }

    return 0;
}

//Подсчет количества символов, слов и строк в файле
//Пример вызова:
//wc file
//wc -m file
//wc -l file
SHCMD(wc)
{
    int np = 0;
    while (params[np]!=NULL)
        np++;

    //Здесь и везде таким образом проиводится разбор переданных ключей
    // и зависимости от них вызываетс нужная функция
    int c;
    while ((c = getopt(np, params, "ml"))!=-1) {
        switch (c) {

        case 'm':
        case 'l':
            while (optind < np)
            {
                wc (params[optind++], c);
            }

            return 0;
        default:
            printf ("Error!\n", c);
            return 0;
        }
    }

    //Если ключей не было передано
    while (optind < np)
        wc (params[optind++], 0);

    return 0;
}

//Вывод файла с нумерацей строк
//Тут все стандартно
//Пример вызова:
//nl file
SHCMD(nl)
{
    int np = 0;
    while (params[np]!=NULL)
        np++;

    while (optind < np)
        nl (params[optind++]);

    return 0;
}

//Создание ссылки
//Пример вызова:
//link file linkfile
SHCMD(link)
{
    linkM (params[1], params[2]);

    return 0;
}

//Поиск подстроки во всех переданных файлах
//Пример вызова:
//grep somestring file1 file2 file3
SHCMD(grep)
{
    int np = 0;
    while (params[np]!=NULL)
        np++;


    int str = optind++;
    while (optind < np)
    {
        if (np>3) printf ("%s: ", params[optind]);
        grep(params[optind++], params[str]);
    }

    return 0;
}

//Вывод всех файлов в папке
//Пример вызова:
//ls
//ls / (в корне)
//ls /home (в папке /home)
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

    //Если проиводится вызов функции без параметров, то выводим текущую диреторию
    if (np == optind) params[np++] = ".";


    //Всякие структуры для вывода информации о файлах
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

    //Считываем каждый файл и выводим о нем информацию
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

//Вывод текущих активных процессов
SHCMD(ps)
{
    //Структура для хранения информации
    struct proc_t ps;

    //Открываем папку с процессами
	DIR *Dir = opendir(PROC_FS);

	char time[256];

    //И выводим информацию о каждом процессе
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




// выполнение команды с параметрами
void MyExec(char *cmd)
{

	char *params[256];  //параметры команды, разделенные пробелами
	char *token;
	int np;

	token = strtok(cmd, " "); //Разделение на подстроки
	np = 0;

	//отделяем параметры друг от друга
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
    if( IS_CMD(cat) )
		SHCMD_EXEC(cat);
    else
    if( IS_CMD(wc) )
		SHCMD_EXEC(wc);
    else
    if( IS_CMD(ps) )
		SHCMD_EXEC(ps);
    else
    if( IS_CMD(nl) )
		SHCMD_EXEC(nl);
    else
    if( IS_CMD(link) )
		SHCMD_EXEC(link);
    else
    if( IS_CMD(grep) )
		SHCMD_EXEC(grep);
    else
	{
	// иначе вызов внешней команды
		execvp(params[0], params);
		perror("exec"); // если возникла ошибка при запуске
	}
}
// рекурсивная функция обработки конвейера
// параметры: строка команды, количество команд в конвейере, текущая (начинается с 0 )
//Это все он писал сам, воспринимай это As is!
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

//сюда поступает строка с командами в ее первоначальном виде
//Происходит разделение на отдельные команды, если присутствует конвейер (|)
void Do (char* command)
{
    char *p, *token;
    int cmd_cnt;
    char *cmds[256];

    //Если в команде откуда-то перенос строки, то "обрубаем" команду на нем
    if( (p = strstr(command,"\n")) != NULL ) p[0] = 0;

    //Раздение строки на команды по конвейеру
    //И занесение отдельных команд в массив
    token = strtok(command, "|");
	for(cmd_cnt = 0; token && cmd_cnt < 256; cmd_cnt++ )
	{
		cmds[cmd_cnt] = strdup(token);
		token = strtok(NULL, "|");
	}
	cmds[cmd_cnt] = NULL;

    //Исполнение каждой команды из массива
	if( cmd_cnt > 0 )
	{
		ExecConv(cmds,cmd_cnt,0);
	}
}

// главная функция, цикл ввода строк
int main(int argc, char** argv)
{
    //По усмолчанию мы в домашнем каталоге
    chdir(getenv ("HOME"));
    setenv ("PWD", getenv ("HOME"), 1);

    //Если команды считываются из файла
    //Как это происходит!
    //Например, исполнимый файл (уже скомпилированный) нашего баша называется mybash
    //А список команд у нас в файле commands
    //Тогда мы просто вызываем mybash comands
    //И исполняются все команды из файла
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

    //Если мы все-таки не из файла команды берем, а вводим их вручную
    else
    {
        //Пока не введено exit ( exit делает shellActive = false )
        while( shellActive )
        {

            //Если в системе задано приветствие, то используем его
            if (getenv("PS1"))
                printf("%s", getenv("PS1"));
            //Если нет, то используем свое
            else
                printf("[%s]# ",getenv("PWD"));


            fflush(stdout);

            char cmdline[LINELENGHT];
            //Считываем строку
            fgets(cmdline,LINELENGHT,stdin);

            //Выполняем считанные команды
            Do (cmdline);
        }
    }

	return 0;
}
