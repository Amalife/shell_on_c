#include "std_n_def.h"
#include "func_pro.h"
#include "global.h"

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
