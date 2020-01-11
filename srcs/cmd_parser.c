#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

char    *g_cmd_base[NUM_OF_CMD] = {"pwd", "exit", "cd"};
struct  passwd  *g_user;
char    g_shell_dir[256];

int     check_last(char *str, int k)
{
    while (str[k] && (str[k] == ' ' || str[k] == '\t'))
        k++;
    if (str[k] == '\0')
        return 0;
    else
        return 1;
}

char    *app_var(char *old_buf, char *str, int k, int i)
{
    char    *ch_var;
    int     size;
    int     t;
    char    *nbr;
    char    var[256] = "";

    size = 0;
    t = 0;
    k++;
    ch_var = (char*)malloc(sizeof(char));
    if (str[k] == '{')
    {
        k++;
        while (str[k] && str[k] != '}')
        {
            size++;
            ch_var = realloc(ch_var, sizeof(char) * (size + 1));
            ch_var[size-1] = str[k];
            k++;
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
        old_buf[i] = '\0';
    }
    free(ch_var);
        return old_buf;
}

char    *quote_str(char *str, int k, char sym)
{
    char    *buf;
    int     i;
    int     ind_d;

    ind_d = 0;
    i = 0;
    if (sym == '"')
        ind_d = 1;
    buf = (char*)malloc(sizeof(char) * 2);
    buf[i] = str[k];
    i++;
    k++;
    while (str[k] != sym && str[k])
    {
        if (ind_d == 1 && str[k] == '$')
        {
            buf = app_var(buf, str, k, i);
            while (str[k] && str[k] != '}')
                k++;
            if (str[k])
                k++;
            while (buf[i])
                i++;
        }
        else
        {
            buf = realloc(buf, sizeof(char) * (i + 3));
            buf[i] = str[k];
            i++;
            k++;
        }
    }
    buf[i] = str[k];
    i++;
    buf[i] = '\0';
    return buf;
}

char    *sim_str(char *str, int k)
{
    char    *buf;
    int     i;

    i = 0;
    buf = (char*)malloc(sizeof(char) * 2);
    while (str[k] != ' ' && str[k] != '\t' && str[k] && str[k] != '|' && 
                                                                str[k] != ';')
    {
        if (str[k] == '$')
        {
            buf = app_var(buf, str, k, i);
            while (str[k] && str[k] != '}')
                k++;
            if (str[k])
                k++;
            while (buf[i])
                i++;
        }
        else
        {
            if (str[k] == '\\')
                k++;
            buf = realloc(buf, sizeof(char) * (i + 2));
            buf[i] = str[k];
            i++;
            k++;
        }
    }
    buf[i] = '\0';
    return buf;
}

char    *make_str(char *str, int k)
{
    char    *buf;

    if (str[k] == '"')
        buf = quote_str(str, k, '"');
    else if (str[k] == '\'')
        buf = quote_str(str, k, '\'');
    else if (str[k] != ' ' && str[k] != '\t')
        buf = sim_str(str, k);
    return buf;
}

void    make_program(char *str, t_program *program)
{
    int k;
    int i;
    int count;

    k = 0;
    i = 0;
    program->num_of_jobs = 0;
    program->job = (struct s_job*)malloc(sizeof(struct s_job));
    while (str[k] && check_last(str, k))
    {
        count = 0;
        program->job = realloc(program->job, sizeof(struct s_job) * (i + 1));
        program->job[i].num_of_arg = 0;
        while (str[k] && (str[k] == ' ' || str[k] == '\t'))
            k++;
        if (str[k] == '|' || str[k] == ';')
        {
            k++;
            break;
        }
        program->job[i].name = make_str(str, k);
        k += strlen(program->job[i].name);
        program->job[i].arg = malloc(sizeof(char*) * (count + 1));
        while (str[k] && check_last(str, k))
        {
            while (str[k] && (str[k] == ' ' || str[k] == '\t'))
                k++;
            if (str[k] == '|' || str[k] == ';')
            {
                k++;
                break;
            }
            program->job[i].arg = realloc(program->job[i].arg, 
                                                sizeof(char*) * (count + 2));
            program->job[i].arg[count] = make_str(str, k);
            k += strlen(program->job[i].arg[count]);
            count++;
            program->job[i].num_of_arg++;
            if (str[k])
                k++;
        }
        //program->job[i].arg = realloc(program->job[i].arg,
         //                                       sizeof(char*) * (count + 2));
        //program->job[i].arg[count] = NULL;
        program->job[i].in_file = NULL;
        program->job[i].out_file = NULL;
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
