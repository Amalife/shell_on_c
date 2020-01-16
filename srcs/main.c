#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

struct      passwd  *g_user;
char        g_shell_dir[256] = "";
int         g_k;
t_history   *g_history;

int     exter_cmd(struct s_job *job, int i)
{
    char    **argv;
    int     j;

    j = 0;
    argv = (char**)malloc(sizeof(char*) * 2);
    argv[j] = job[i].name;
    j++;
    if (job[i].num_of_arg)
    {    
        while (job[i].arg[j-1])
        {
            argv = realloc(argv, sizeof(char*) * (j + 2));
            argv[j] = job[i].arg[j-1];
            j++;
        }
    }
    argv[j] = NULL;
    if (execvp(job[i].name, argv) == -1)
    {
        ft_putstr(job[i].name);
        ft_putstr(": command not found\n");
    }
    free(argv);
    return 0;
}

int     run_program(t_program *program, int i)
{
    int r;

    r = 0;
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
        case I_HISTORY:
            history_cmd();
            break;
        default:
            //ft_putstr("not an iternal command\n");
            r = exter_cmd(program->job, i);
    }
    return r;
}

int     main(int argc, char **argv)
{
    int             size;
    int             ch;
    int             k;
    int             ind[2] = {0, 0};
    int             com;
    char            *buf;
    t_program       *program;

    program = (t_program*)malloc(sizeof(t_program));
    g_history = (t_history*)malloc(sizeof(t_history));
    g_history->top = 1;
    g_history->bot = 1;
    g_history->num = 0;
    g_history->commands = (char**)malloc(sizeof(char*));
    k = 1;
    com = 0;
    if (argc == 1)
    {
        memcpy(g_shell_dir, getenv("PWD"), strlen(getenv("PWD")));
        size = strlen(getenv("PWD"));
        while (argv[0][k])
        {
            g_shell_dir[size] = argv[0][k];
            size++;
            k++;
        }
        g_shell_dir[size] = '\0';
    }
    else
        return 0;
    program->fl_exit = 1;
    while (program->fl_exit)
    {
        g_k = 0;
        size = 0;
        g_user = getpwuid(getuid());
        ft_putstr(g_user->pw_name);
        ft_putstr("$ ");
        buf = (char*)malloc(sizeof(char));
        do
        {
            while ((ch = getchar()) != '\n' && ch != EOF)
            {
                if (ch == '\'')
                    ind[0] = (ind[0] + 1) % 2;
                if (ch == '"')
                    ind[1] = (ind[1] + 1) % 2;
                if (ind[0] == 0 && ind[1] == 0 && ch == '#' && 
                                                            buf[size-1] != '\\')
                {
                    com = 1;
                    k = size;
                }
                size++;
                buf = realloc(buf, sizeof(char) * (size + 1));
                buf[size-1] = ch;
            }
            buf[size] = '\0';
            if (ch == EOF)
            {
                ft_putchar('\n');
                program->fl_exit = 0;
                break;
            }
        } while (buf[size-1] == '\\');
        if (check_his(buf) == 0 || check_his(buf) == -1)
        {
            if (check_his(buf) == -1)
                buf = look_his(buf);
            add_history(buf);
            if (com && buf[k-1] != '\\' && buf[k-1] == ' ')
            {
                buf = realloc(buf, sizeof(char) * (k + 1));
                buf[k] = '\0';
            }
            while (buf[g_k] && check_last(buf, g_k))
            {
                make_program(buf, program);
                check_program(buf, program);
                create_proc(program);
                free_program(program);
                if (buf[g_k])
                    g_k++;
            }
            free(buf);
        }
        else
        {
            buf = err_his(buf, check_his(buf));
            ft_putstr(buf);
            ft_putstr(": event not found\n");
            free(buf);
        }
    }
    free_history();
    free(program);
    return 0;
}
