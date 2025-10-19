#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int count = getcount(28); // 28 is the syscall number for getfavnum
    printf(1, "System call 'getcount' returned: %d\n", count);

    getfavnum();
    getfavnum();
    getfavnum();

    count = getcount(28); // 28 is the syscall number for getfavnum
    printf(1, "System call 'getcount' after calling 'getfavnum' 3 times returned: %d\n", count);

    exit();
}