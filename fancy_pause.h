#pragma once

#ifdef linux

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int _kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

#endif

#if defined(_WIN32) || defined(WIN32)

#include <conio.h>

#endif

void fancy_pause(char *msg)
{
    printf("%s", msg);
    fflush(stdout);
    while (!_kbhit())
        ;

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}
