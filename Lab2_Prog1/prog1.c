/*
 ** sigint.c -- grabs SIGINT
 * Source:  http://beej.us/guide/bgipc/output/html/multipage/signals.html
 * Mod. by:  Carolina Ayala
 * compile with -std=gnu99 flag!
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


void sig_handler(int sig);
sig_atomic_t usr1Happened = 0;

/*******************************************************************************
 * main( )
 ******************************************************************************/
int main(void) {
	char s[140];
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGUSR1, &sa, NULL);

	printf("PID = %d : Running...\n", getpid());
    while(usr1Happened != 1){} //Loop until SIGUSR1
    printf("PID = %d : Received USR1\n", getpid());
    printf("PID = %d : Exiting...\n", getpid());

	return 0;
}

void sig_handler(int sig){
	if (sig == SIGUSR1){
    	usr1Happened = 1;
	}
}
