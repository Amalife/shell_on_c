#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

int     cd_cmd(char *path)
{
    char    buf[256];
    
    if (chdir(path) == 0)
    {
        getcwd(buf, 256);
        setenv("PWD", buf, 1);
    }
    else
    {
        perror(buf);
        return -1;
    }
    return 0;
}

int     exter_cmd(t_program *program)
{
    int     i;
    int     j;
    pid_t   pid;
    char    **argv;

    i = 0;
    j = 0;
    argv = (char**)malloc(sizeof(char*) * (program->job[i].num_of_arg + 2));
    argv[j] = program->job[i].name;
    j++;
    while (j != program->job[i].num_of_arg + 1)
    {
        argv[j] = program->job[i].arg[j-1];
        j++;
    }
    argv[j] = NULL;
    pid = fork();
    if (pid == 0)
    {
        //ft_putstr("It is a child\n");
        if (execvp(program->job[i].name, argv) == -1)
        {
            free(argv);
            perror(program->job[i].name);
            return -1;
        }
        free(argv);
        return 0;
    }
    else
    {
        wait(0);
        free(argv);
        //ft_putstr("It is a parent\n");
        return 0;
    }
}

int     run_program(t_program *program)
{
    int i;
    int r;

    i = 0;
    r = 0;
    while (i != program->num_of_jobs)
    {
        switch (check_iternal(program->job[i].name))
        {
            case I_PWD:
                ft_putstr(getenv("PWD"));
                ft_putchar('\n');
                break;
            case I_EXIT:
                program->fl_exit = 0;
                break;
            case I_CD:
                r = cd_cmd(program->job[i].arg[0]);
                break;
            default:
                //ft_putstr("not an iternal command\n");
                r = exter_cmd(program);
        }
        i++;
    }
    return r;
}

int     main()
{
    int             size;
    int             ch;
    int             r;
    char            *buf;
    struct  passwd  *user;
    t_program       *program;

    program = (t_program*)malloc(sizeof(t_program));
    program->fl_exit = 1;
    while (program->fl_exit)
    {
        size = 0;
        user = getpwuid(getuid());
        ft_putstr(user->pw_name);
        ft_putstr("$ ");
        buf = (char*)malloc(sizeof(char));
        while ((ch = getchar()) != '\n')
        {
            size++;
            buf = realloc(buf, sizeof(char) * (size + 1));
            buf[size-1] = ch;
        }
        buf[size] = '\0';
        make_program(buf, program);
        check_program(buf, program);
        r = run_program(program);
        free(buf);
        free_program(program);
    }
    printf("return check: %d\n", r);
    free(program);
    return 0;
}
