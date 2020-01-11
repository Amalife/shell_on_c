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

int     ft_nbrlen(int nbr)
{
    int n;

    n = 0;
    if (nbr < 0)
    {
        nbr *= -1;
        n++;
    }
    while (nbr > 0)
    {
        n++;
        nbr /= 10;
    }
    return n;
}

char    *ft_itoa(int nbr)
{
    char    *str;
    int     len;

    len = ft_nbrlen(nbr);
    str = (char*)malloc(sizeof(char) * (len + 1));
    str[len] = '\0';
    len--;
    if (nbr == 0)
    {
        str[0] = '0';
        return str;
    }
    if (nbr < 0)
    {
        str[0] = '-';
        nbr *= -1;
    }
    while (nbr > 0)
    {
        str[len] = (nbr % 10) + '0';
        nbr /= 10;
        len--;

    }
    return str;
}
