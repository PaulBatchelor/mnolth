#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include "light.h"

int mno_light_init(mno_light *ml, const char *portname)
{
    int fd;
    struct termios toptions;

    fd = open(portname, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (fd == -1) {
        fprintf(stderr, "mnolight: unable to open port\n");
        return 1;
    }

    if (tcgetattr(fd, &toptions) < 0) {
        fprintf(stderr, "mnolight: Couldn't get term attributes\n");
        return 2;
    }

    /* Use default baud rate of 9600, or 12 kb/s
     * Note: using sndkit's block size of 64, this should
     * be fast enough. At 44.1kHz this should be about
     * 44100 / 64 bytes / sec, or ~689 bytes.
     */
    cfsetospeed(&toptions, B9600);

    /* 8N1 */

    toptions.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
    toptions.c_cflag |= (CS8 | CLOCAL | CREAD);

    /* no line processing */
    toptions.c_lflag |= ~(ICANON | ECHO | ECHOE | ISIG | IEXTEN);

    /* raw input */
    toptions.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
        INLCR | PARMRK | INPCK | ISTRIP | IXON);

	/* raw output */
    toptions.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
        ONOCR | OFILL | OPOST);


    if (tcsetattr(fd, TCSANOW, &toptions) < 0) {
        close(ml->fd);
        fprintf(stderr, "mnolight: Couldn't set term attributes\b");
        return 3;
    }

    ml->fd = fd;
    return 0;
}

void mno_light_close(mno_light *ml)
{
    close(ml->fd);
}

void mno_light_set(mno_light *ml, float amt)
{
    unsigned char msg[1];
    unsigned char val;
    /* clamp range to be 0-1 */
    if (amt < 0) amt = 0;
    else if (amt > 1) amt = 1;


    val = floor(amt * 0xFF);
    msg[0] = val;

    write(ml->fd, msg, 1);
}
