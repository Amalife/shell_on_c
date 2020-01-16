#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

int     create_proc(t_program *program)
{
    int     j;
    int     pipefd[2];
    int     input;
    int     status;
    int     in;
    //int     fl_eof;
    int     out;
    pid_t   pid;

    j = 0;
    //fl_eof = 0;
    if (program->num_of_jobs == 1)
    {
        if (check_iternal(program->job[j].name) != -1)
            run_program(program, j);
        else
        {
            pid = fork();
            if (pid == 0)
            {
                if (program->job[j].in_file)
                {
                    if ((in = open(program->job[j].in_file, O_RDONLY)) != -1)
                    {
                        dup2(in, 0);
                        close(in);
                    }
                    else
                    {
                        perror(program->job[j].in_file);
                        exit(0);
                    }
                }
                if (program->job[j].out_file)
                {
                    if (program->job[j].out_type == T_REWRITE)
                        out = open(program->job[j].out_file, 
                            O_WRONLY | O_TRUNC | O_CREAT, 
                                        S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                    else
                        out = open(program->job[j].out_file,
                            O_WRONLY | O_APPEND | O_CREAT,
                                        S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                    dup2(out, 1);
                    close(out);
                }
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
                    if (program->job[j].in_file)
                    {
                        if ((in = open(program->job[j].in_file, O_RDONLY)) != -1)
                        {
                            dup2(in, 0);
                            close(in);
                        }
                        else
                        {
                            perror(program->job[j].in_file);
                            close(pipefd[1]);
                            close(pipefd[0]);
                            exit(0);
                        }
                    }
                    if (program->job[j].out_file)
                    {
                        if (program->job[j].out_type == T_REWRITE)
                            out = open(program->job[j].out_file,
                                O_WRONLY | O_TRUNC | O_CREAT, 
                                        S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                        else
                            out = open(program->job[j].out_file,
                                O_WRONLY | O_APPEND | O_CREAT,
                                        S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                        dup2(out, 1);
                        close(out);
                    }
                    else
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
                        if (program->job[j].in_file)
                        {
                            if ((in = open(program->job[j].in_file, O_RDONLY)) != -1)
                            {
                                dup2(in, 0);
                                close(in);
                            }
                            else
                            {
                                perror(program->job[j].in_file);
                                close(pipefd[1]);
                                close(pipefd[0]);
                                exit(0);
                            }
                        }
                        else
                        {
                            dup2(input, 0);
                            close(input);
                        }
                        if (program->job[j].out_file)
                        {
                            if (program->job[j].out_type == T_REWRITE)
                                out = open(program->job[j].out_file,
                                    O_WRONLY | O_TRUNC | O_CREAT, 
                                        S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                            else
                                out = open(program->job[j].out_file,
                                    O_WRONLY | O_APPEND | O_CREAT,
                                        S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                            dup2(out, 1);
                            close(out);
                        }
                        else
                            dup2(pipefd[1], 1);
                        close(pipefd[1]);
                        close(pipefd[0]);
                        run_program(program, j);
                        exit(0);
                    }
                    else
                    {
                        if (program->job[j].in_file)
                        {
                            if ((in = open(program->job[j].in_file, O_RDONLY)) != -1)
                            {
                                dup2(in, 0);
                                close(in);
                            }
                            else
                            {
                                perror(program->job[j].in_file);
                                close(pipefd[1]);
                                close(pipefd[0]);
                                exit(0);
                            }
                        }
                        else
                        {
                            dup2(input, 0);
                            close(input);
                        }
                        if (program->job[j].out_file)
                        {
                            if (program->job[j].out_type == T_REWRITE)
                                out = open(program->job[j].out_file,
                                    O_WRONLY | O_TRUNC | O_CREAT, 
                                        S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                            else
                                out = open(program->job[j].out_file,
                                    O_WRONLY | O_APPEND | O_CREAT,
                                        S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
                            dup2(out, 1);
                            close(out);
                        }
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
