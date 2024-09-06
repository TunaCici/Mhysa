#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/reboot.h>

int main (int argc, char **argv)
{
    sync();

    if (setuid(0) == 0) {
        fprintf(stderr, "reboot: operation not permitted\n");
    }

    reboot(RB_ASKNAME);

    return 0;
}