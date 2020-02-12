#ifndef __FUNC_PRO_H__
# define __FUNC_PRO_H__

#include "global.h"

void    ft_putchar(char c);
void    ft_putstr(char *str);
char    *ft_itoa(int nbr);
int     ft_nbrlen(int nbr);
int     ft_atoi(char *str);

void    check_program(char *str, t_program *program);
void    free_program(t_program *program);
void    free_history();

int     check_last(char *str, int t);
char    *quote_str(char *str, char sym);
char    *sim_str(t_program *program, char *str, int j);
char    *make_str(t_program *program, char *str, int i);
char    *app_var(char *old_buf, char *str, int i);
void    finout(t_program *program, struct s_job *job, char *str, int i);
void    make_program(char *str, t_program *program);

int     run_program(t_program *program, int i);
int     create_proc(t_program *program);
int     exter_cmd(struct s_job *job, int i);

int     check_iternal(char *str);
int     cd_cmd(char *path);
void    add_history(char *buf);
void    history_cmd();
int     check_his(char *str);
char    *look_his(char *str);
char    *err_his(char *str, int pos);

#endif
