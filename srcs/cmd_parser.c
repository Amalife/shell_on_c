#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

char    *g_cmd_base[NUM_OF_CMD] = {"pwd", "exit", "cd", "history"};
struct  passwd  *g_user;
char    g_shell_dir[256];
char    **main_argv;
int     main_argc;
int     g_k;

int     check_last(char *str, int t)
{
    while (str[t] && (str[t] == ' ' || str[t] == '\t'))
        t++;
    if (str[t] == '\0')
        return 0;
    else
        return 1;
}

char    *app_var(char *old_buf, char *str, int i)
{
    char    *ch_var;
    int     size;
    int     t;
    char    *nbr;
    char    var[256] = "";

    size = 0;
    t = 0;
    g_k++;
    ch_var = (char*)malloc(sizeof(char));
    if (str[g_k] == '{')
    {
        g_k++;
        while (str[g_k] && str[g_k] != '}')
        {
            size++;
            ch_var = realloc(ch_var, sizeof(char) * (size + 1));
            ch_var[size-1] = str[g_k];
            g_k++;
        }
        ch_var[size] = '\0';
        if (strcmp(ch_var, "USER") == 0)
            memcpy(var, g_user->pw_name, strlen(g_user->pw_name) + 1);
        else if (strcmp(ch_var, "HOME") == 0)
            memcpy(var, g_user->pw_dir, strlen(g_user->pw_dir) + 1);
        else if (strcmp(ch_var, "SHELL") == 0)
            memcpy(var, g_shell_dir, strlen(g_shell_dir) + 1);
        else if (strcmp(ch_var, "UID") == 0)
        {
            nbr = ft_itoa(getuid());
            memcpy(var, nbr, strlen(nbr) + 1);
            free(nbr);
        }
        else if (strcmp(ch_var, "PWD") == 0)
            memcpy(var, getenv("PWD"), strlen(getenv("PWD")) + 1);
        else if (strcmp(ch_var, "PID") == 0)
        {
            nbr = ft_itoa(getpid());
            memcpy(var, nbr, strlen(nbr) + 1);
            free(nbr);
        }
        while (var[t])
        {
            size++;
            old_buf = realloc(old_buf, sizeof(char) * (i + 3));
            old_buf[i] = var[t];
            i++;
            t++;
        }
        if (str[g_k])
            g_k++;
    }
    else if (str[g_k] == '#')
    {
        nbr = ft_itoa(main_argc);
        while (nbr[t])
        {
            old_buf = realloc(old_buf, sizeof(char) * (i + 3));
            old_buf[i] = nbr[t];
            i++;
            t++;
        }
        free(nbr);
        if (str[g_k])
            g_k++;
    }
    else if (str[g_k] && str[g_k] >= '0' && str[g_k] <= '9')
    {
        while (str[g_k] && str[g_k] >= '0' && str[g_k] <= '9')
        {
            size++;
            ch_var = realloc(ch_var, sizeof(char) * (size + 1));
            ch_var[size-1] = str[g_k];
            g_k++;
        }
        ch_var[size] = '\0';
        size = ft_atoi(ch_var);
        if (size < main_argc && size >= 0)
        {
            while (main_argv[size][t])
            {
                old_buf = realloc(old_buf, sizeof(char) * (i + 3));
                old_buf[i] = main_argv[size][t];
                t++;
                i++;
            }
        }
    }
    old_buf[i] = '\0';
    free(ch_var);
    return old_buf;
}

char    *quote_str(char *str, char sym)
{
    char    *buf;
    int     i;
    int     ind_d;

    ind_d = 0;
    i = 0;
    if (sym == '"')
        ind_d = 1;
    buf = (char*)malloc(sizeof(char) * 2);
    g_k++;
    while (str[g_k] != sym && str[g_k])
    {
        if (ind_d == 1 && str[g_k] == '$')
        {
            buf = app_var(buf, str, i);
            while (buf[i])
                i++;
        }
        else
        {
            buf = realloc(buf, sizeof(char) * (i + 3));
            buf[i] = str[g_k];
            i++;
            g_k++;
        }
    }
    buf[i] = '\0';
    return buf;
}

char    *sim_str(t_program *program, char *str, int j)
{
    char    *buf;
    int     i;
    int     t;

    i = 0;
    buf = (char*)malloc(sizeof(char) * 2);
    while (str[g_k] != ' ' && str[g_k] != '\t' && str[g_k] && str[g_k] != ';'
                                                            && str[g_k] != '|')
    {
        if (str[g_k] == '$')
        {
            t = g_k;
            buf = app_var(buf, str, i);
            if (str[t+1] == '{')
            {
                while (str[g_k] && str[g_k] != '}')
                    g_k++;
            }
            if (str[g_k])
                g_k++;
            while (buf[i])
                i++;
        }
        else if (str[g_k] == '<' || str[g_k] == '>' || 
                                    (str[g_k] == '>' && str[g_k+1] == '>'))
            finout(program, program->job, str, j);
        else
        {
            if (str[g_k] == '\\')
                g_k++;
            buf = realloc(buf, sizeof(char) * (i + 2));
            buf[i] = str[g_k];
            i++;
            g_k++;
        }
    }
    buf[i] = '\0';
    return buf;
}

char    *make_str(t_program *program, char *str, int i)
{
    char    *buf;

    if (str[g_k] == '"')
        buf = quote_str(str, '"');
    else if (str[g_k] == '\'')
        buf = quote_str(str, '\'');
    else if (str[g_k] != ' ' && str[g_k] != '\t')
        buf = sim_str(program, str, i);
    return buf;
}

void    finout(t_program *program, struct s_job *job, char *str, int i)
{
    //int t;

    //t = g_k;
    if (str[g_k] == '<')
    {
        g_k++;
        while (str[g_k] && (str[g_k] == ' ' || str[g_k] == '\t'))
            g_k++;
        job[i].in_file = make_str(program, str, i);
    }
    else if (str[g_k] == '>' && str[g_k+1] == '>')
    {
        job[i].out_type = T_APPEND;
        g_k += 2;
        while (str[g_k] && (str[g_k] == ' ' || str[g_k] == '\t'))
            g_k++;
        job[i].out_file = make_str(program, str, i);
    }
    else if (str[g_k] == '>')
    {
        job[i].out_type = T_REWRITE;
        g_k++;
        while (str[g_k] && (str[g_k] == ' ' || str[g_k] == '\t'))
            g_k++;
        job[i].out_file = make_str(program, str, i);
    }
}

void    make_program(char *str, t_program *program)
{
    int i;
    int count;

    i = 0;
    program->num_of_jobs = 0;
    program->job = (struct s_job*)malloc(sizeof(struct s_job));
    while (str[g_k] && check_last(str, g_k))
    {
        count = 0;
        program->job = realloc(program->job, sizeof(struct s_job) * (i + 1));
        program->job[i].in_file = NULL;
        program->job[i].out_file = NULL;
        program->job[i].num_of_arg = 0;
        while (str[g_k] && (str[g_k] == ' ' || str[g_k] == '\t'))
            g_k++;
        if (str[g_k] == '|')
        {
            g_k++;
            break;
        }
        if (str[g_k] == ';')
            break;
        else if (str[g_k] == '<' || str[g_k] == '>' || 
                                    (str[g_k] == '>' && str[g_k+1] == '>'))
            finout(program, program->job, str, i);
        program->job[i].name = make_str(program, str, i);
        program->job[i].arg = malloc(sizeof(char*) * (count + 1));
        while (str[g_k] && check_last(str, g_k) && str[g_k] != ';')
        {
            while (str[g_k] && (str[g_k] == ' ' || str[g_k] == '\t'))
                g_k++;
            if (str[g_k] == '|')
            {
                g_k++;
                break;
            }
            if (str[g_k] == ';')
                break;
            else if (str[g_k] == '<' || str[g_k] == '>' || 
                                        (str[g_k] == '>' && str[g_k+1] == '>'))
                finout(program, program->job, str, i);
            if (check_last(str, g_k) && str[g_k] != ' ' && str[g_k] != '\t'
                                                            && str[g_k] != ';')
            {
                program->job[i].arg = realloc(program->job[i].arg, 
                                                sizeof(char*) * (count + 2));
                program->job[i].arg[count] = make_str(program, str, i);
                count++;
                program->job[i].num_of_arg++;
            }
            if (str[g_k] && str[g_k] != ';' && str[g_k] != '|')
                g_k++;
        }
        program->job[i].arg = realloc(program->job[i].arg,
                                                sizeof(char*) * (count + 2));
        program->job[i].arg[count] = NULL;
        if (program->job[i].out_file == NULL)
            program->job[i].out_type = T_NONE;
        i++;
        program->num_of_jobs++;
    }
    program->backgrd = 0;
}

int     check_iternal(char *str)
{
    int i;

    i = 0;
    while (i < NUM_OF_CMD)
    {
        if (strcmp(str, g_cmd_base[i]) == 0)
            return i;
        i++;
    }
    return -1;
}
