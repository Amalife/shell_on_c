#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

char    *g_cmd_base[NUM_OF_CMD] = {"pwd", "exit", "cd"};

int     check_last(char *str, int k)
{
    while (str[k] && (str[k] == ' ' || str[k] == '\t'))
        k++;
    if (str[k] == '\0')
        return 0;
    else
        return 1;
}

char    *quote_str(char *str, int k, char sym)
{
    char    *buf;
    int     i;

    i = 0;
    buf = (char*)malloc(sizeof(char) * 2);
    buf[i] = str[k];
    i++;
    k++;
    while (str[k] != sym && str[k])
    {
        buf = realloc(buf, sizeof(char) * (i + 3));
        buf[i] = str[k];
        i++;
        k++;
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
        buf = realloc(buf, sizeof(char) * (i + 2));
        buf[i] = str[k];
        i++;
        k++;
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

void    check_program(char *str, t_program *program)
{
    int i;
    int j;

    i = 0;
    printf("\n\nraw string: [%s]\n\n", str);
    printf("background: %d\nnumber of jobs: %d\n\n", program->backgrd, 
                                              program->num_of_jobs);
    while (i != program->num_of_jobs)
    {
        printf("the %d job:\n", i + 1);
        printf("  name: %s\n", program->job[i].name);
        printf("  number of arguments: %d\n", program->job[i].num_of_arg);
        if (program->job[i].arg)
        {
            j = 0;
            while (j != program->job[i].num_of_arg)
            {
                printf("\tthe %d argument: %s\n", j + 1, program->job[i].arg[j]);
                j++;
            }
        }
        else
            printf("  no arguments\n");
        if (program->job[i].in_file)
            printf("  input file: %s\n", program->job[i].in_file);
        else
            printf("  no input file\n");
        if (program->job[i].out_file)
            printf("  output file: %s\n", program->job[i].out_file);
        else
            printf("  no output file\n");
        printf("  output type: %d\n\n\n", program->job[i].out_type);
        i++;
    }
}

void    free_program(t_program *program)
{
    int i;
    int j;

    i = 0;
    if (program)
    {
        while (i != program->num_of_jobs)
        {
            j = 0;
            if (program->job[i].name)
                free(program->job[i].name);
            if (program->job[i].in_file)
                free(program->job[i].in_file);
            if (program->job[i].out_file)
                free(program->job[i].out_file);
            if (program->job[i].arg)
            {
                while (j != program->job[i].num_of_arg)
                {
                    free(program->job[i].arg[j]);
                    j++;
                }
                free(program->job[i].arg);
            }
            i++;
        }
        free(program->job);
    }
}
