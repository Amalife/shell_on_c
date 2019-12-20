#ifndef __FUNC_PRO_H__
# define __FUNC_PRO_H__

#include "global.h"

void    ft_putchar(char c);
void    ft_putstr(char *str);
int     check_last(char *str, int k);
char    *quote_str(char *str, int k, char sym);
char    *sim_str(char *str, int k);
char    *make_str(char *str, int k);
void    make_program(char *str, t_program *program);
void    check_program(char *str, t_program *program);
void    free_program(t_program *program);
int     check_iternal(char *str);
int     run_program(t_program *program);

#endif
