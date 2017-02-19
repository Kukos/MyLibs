#include <getch.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

/* set global static struct to avoid resizing stack every time in getch */
static struct termios old_attr;
static struct termios new_attr;

int getch()
{
    int ch; /* getchar return int instead of char !!! */

	/* get current settings */
    if (tcgetattr(STDIN_FILENO, &old_attr) == -1)
		return -1;

	if (memcpy((void *)&new_attr, (void *)&old_attr,
			sizeof(struct termios)) == NULL)
		return (int)-1;

	/* reset waiting for enter */
    new_attr.c_lflag &= ~(ICANON | ECHO);

	/* set new settings */
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_attr) == -1)
		return (int)-1;

    ch = getchar();

	/* restore old */
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old_attr) == -1)
		return (int)-1;

    return ch;
}
