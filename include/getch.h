#ifndef GETCH_H
#define GETCH_H

/*
	Author: Michal Kukowski
	email: michalkukowski10@gmail.com

	LICENCE: GPL 3.0
*/

/*
	Function similar to getch from conio.h on windows

	Get char but without waiting for enter

	PARAMS
	@NO PARAMS

	RETURN:
	char from stdout iff success
    -1 iff failure
*/
int getch(void);

#endif
