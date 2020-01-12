#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

int     create_proc(t_program *program)
{
    int     j;
    int     pipefd[2];
    int     input;
    int     status;
    pid_t   pid;

    j = 0;
    if (program->num_of_jobs == 1)
    {
        if (check_iternal(program->job[j].name) != -1)
            run_program(program, j);
        else
        {
            pid = fork();
            if (pid == 0)
            {
                run_program(program, j);
                exit(0);
            }
            if (pid > 0)
                waitpid(pid, &status, 0);
        }
    }
    else
    {
        while (j < program->num_of_jobs)
        {
            if (j != program->num_of_jobs - 1)
                pipe(pipefd);
            pid = fork();
            if (pid == 0)
            {
                if (j == 0)
                {
                    dup2(pipefd[1], 1);
                    close(pipefd[1]);
                    close(pipefd[0]);
                    run_program(program, j);
                    exit(0);
                }
                else
                {
                    if (j != program->num_of_jobs - 1)
                    {
                        dup2(pipefd[1], 1);
                        dup2(input, 0);
                        close(pipefd[1]);
                        close(input);
                        close(pipefd[0]);
                        run_program(program, j);
                        exit(0);
                    }
                    else
                    {
                        dup2(input, 0);
                        close(input);
                        run_program(program, j);
                        exit(0);
                    }
                }
            }
            if (pid > 0)
            {
                close(pipefd[1]);
                if (j != program->num_of_jobs - 1)
                {
                    if (j)
                        close(input);
                    input = pipefd[0];
                }
                else
                    close(pipefd[0]);
            }
            waitpid(pid, &status, 0);
            j++;
        }
    }
    return 0;
}
