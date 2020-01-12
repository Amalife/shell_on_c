#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

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
        printf("  output type: %d\n", program->job[i].out_type);
        //printf("  next pipe: %d\n\n\n", program->job[i].next_pipe);
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
