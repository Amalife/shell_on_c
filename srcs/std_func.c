#include "std_n_def.h"

void    ft_putchar(char c)
{
    write(STDOUT_FILENO, &c, sizeof(char));
}

void    ft_putstr(char *str)
{
    while (*str)
    {
        write(STDOUT_FILENO, str, sizeof(char));
        str++;
    }
}
