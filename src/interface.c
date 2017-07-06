
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include "interface.h"

struct termios old;
unsigned short termios_flag = 0;

ssize_t getpass_custom (char **lineptr, size_t *n, FILE *stream)
{
    struct termios new;
    int nread;

    /* Turn echoing off and fail if we can't. */
    if (tcgetattr (fileno (stream), &old) != 0)
        return -1;

    new = old;
    new.c_lflag &= ~ECHO;
    if (tcsetattr (fileno (stream), TCSAFLUSH, &new) != 0)
        return -1;

	termios_flag = 1;

    /* Read the password. */
    nread = getline (lineptr, n, stream);

    /* Restore terminal. */
	restore_termios(stream);

    return nread;
}

void restore_termios(FILE *stream)
{
	if(termios_flag){
	    (void) tcsetattr (fileno (stream), TCSAFLUSH, &old);
		termios_flag = 0;
	}
}

void progress(int count,int total)
{
	int i=0;

	printf("\r[");

	for(; i< (BAR_LEN * count) / total; i++)
		printf("#");	/* printf("■");*/
	
	for(; i < BAR_LEN; i++)
		printf("-");
	
	printf("] %.1f%%",(100.0 * count)/(total));
	fflush(stdout);
}

void progress_m(int count,int total, const char * message)
{
	progress(count,total);
	printf("  %-30s", message);
	fflush(stdout);
}

