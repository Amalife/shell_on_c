#ifndef __GLOBAL_H__
# define __GLOBAL_H__

enum    e_inter_cmd
{
    I_PWD,
    I_EXIT,
    I_CD,
    I_HISTORY
};

enum    e_out_type
{
    T_NONE,
    T_REWRITE,
    T_APPEND
};

struct  s_job
{
    char    *name;
    int     num_of_arg;
    char    **arg;
    char    *in_file;
    char    *out_file;
    int     out_type;
};

typedef struct  s_program
{
    int     backgrd;
    struct  s_job *job;
    int     num_of_jobs;
    int     fl_exit;
}               t_program;

typedef struct  s_history
{
    int     top;
    int     bot;
    int     num;
    char    **commands;
}               t_history;

#endif
