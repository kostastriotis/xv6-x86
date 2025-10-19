#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    printf(1, "Shutting down...\n");
    halt();
    printf(2, "Error: Halt failed! The system is still running.\n");
    exit();
}