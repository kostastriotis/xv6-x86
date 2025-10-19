/*
 * user/mycolor.c
 * Print the color passed as a command-line argument.
 * Usage: mycolor <color>
 * Example: mycolor red
 * Output: The color you chose is: red
 * Author: Konstantinos Triotis
 * Description: This program takes a color name as a command-line argument and prints it in a formatted string.
 */

#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf(1, "Usage: mycolor <color>\n");
        exit();
    }
    else if (argc > 2)
    {
        printf(1, "You can only choose one color at a time.\n");
        exit();
    }

    printf(1, "The color you chose is: %s\n", argv[1]);
    exit();
}
