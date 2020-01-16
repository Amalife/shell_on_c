#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

t_history *g_history;

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

int     check_his(char *str)
{
    int     k;
    int     size;
    int     t;
    char    *buf;
    int     ind;

    k = 0;
    ind = 0;
    size = 0;
    while (str[k])
    {
        if (str[k] == '!')
        {
            ind = 1;
            t = k;
            k++;
            if (str[k] >= '0' && str[k] <= '9')
            {
                buf = (char*)malloc(sizeof(char));
                while (str[k] >= '0' && str[k] <= '9')
                {
                    size++;
                    buf = realloc(buf, sizeof(char) * (size + 1));
                    buf[size-1] = str[k];
                    k++;
                }
                buf[size] = '\0';
                if (ft_atoi(buf) >= g_history->top && ft_atoi(buf) < g_history->bot)
                {
                    size = 0;
                    free(buf);
                }
                else
                {
                    free(buf);
                    return t;
                }
            }
            else
                return t;
        }
        if (str[k])
            k++;
    }
    if (ind)
        return -1;
    else
        return 0;
}

char    *look_his(char *str)
{
    char    *buf;
    int     k;
    int     size;
    char    *his;
    int     i;
    int     nbr;

    k = 0;
    i = 0;
    size = 0;
    buf = (char*)malloc(sizeof(char));
    while (str[k])
    {
        if (str[k] == '!')
        {
            his = (char*)malloc(sizeof(char));
            k++;
            while (str[k] >= '0' && str[k] <= '9')
            {
                i++;
                his = realloc(his, sizeof(char) * (i + 1));
                his[i-1] = str[k];
                k++;
            }
            his[i] = '\0';
            nbr = ft_atoi(his);
            i = 0;
            free(his);
            nbr -= g_history->top;
            while (g_history->commands[nbr][i])
            {
                size++;
                buf = realloc(buf, sizeof(char) * (size + 1));
                buf[size-1] = g_history->commands[nbr][i];
                i++;
            }
            i = 0;
        }
        else
        {
            size++;
            buf = realloc(buf, sizeof(char) * (size + 1));
            buf[size-1] = str[k];
            k++;
        }
    }
    buf[size] = '\0';
    free(str);
    return buf;
}

char    *err_his(char *str, int pos)
{
    char    *buf;
    int     size;

    size = 0;
    buf = (char*)malloc(sizeof(char) * 2);
    buf[size] = str[pos];
    size++;
    pos++;
    while (str[pos] >= '0' && str[pos] <= '9')
    {
        size++;
        buf = realloc(buf, sizeof(char) * (size + 1));
        buf[size-1] = str[pos];
        pos++;
    }
    buf[size] = '\0';
    free(str);
    return buf;
}

void    add_history(char *buf)
{
    int k;
    int top_save;

    k = 0;
    top_save = 0;
    if (g_history->num < 10)
    {
        g_history->commands = realloc(g_history->commands, sizeof(char*) * (g_history->num + 1));
        g_history->commands[g_history->num % 10] = (char*)malloc(sizeof(char) * (strlen(buf) + 1));
        while (buf[k])
        {
            g_history->commands[g_history->num % 10][k] = buf[k];
            k++;
        }
        g_history->commands[g_history->num % 10][k] = '\0';
        g_history->num++;
    }
    else
    {
        while (top_save != 9)
        {
            g_history->commands[top_save] = realloc(g_history->commands[top_save], sizeof(char) * (strlen(g_history->commands[top_save + 1]) + 1));
            while (g_history->commands[top_save + 1][k])
            {
                g_history->commands[top_save][k] = g_history->commands[top_save + 1][k];
                k++;
            }
            g_history->commands[top_save][k] = '\0';
            top_save++;
            k = 0;
        }
        g_history->commands[top_save] = realloc(g_history->commands[top_save], sizeof(char) * (strlen(buf) + 1));
        while (buf[k])
        {
            g_history->commands[top_save][k] = buf[k];
            k++;
        }
        g_history->commands[top_save][k] = '\0';
        g_history->top++;
    }
        g_history->bot++;
}

void    history_cmd()
{
    int     top_save;
    int     count;
    char    *nbr;

    top_save = 0;
    count = g_history->bot - g_history->top;
    while (count)
    {
        nbr = ft_itoa(g_history->top + top_save);
        ft_putstr(nbr);
        free(nbr);
        ft_putstr("  ");
        ft_putstr(g_history->commands[top_save]);
        ft_putchar('\n');
        top_save++;
        count--;
    }
}
