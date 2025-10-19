#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int favnum = getfavnum();

    printf(1, "System call 'getfavnum' returned: %d\n", favnum);

    exit();
}