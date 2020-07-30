/**
 * threadpool_test.c, copyright 2001 Steve Gribble
 *
 * Just a regression test for the threadpool code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <bits/stdc++.h>
#include "Threadpool.h"			//threadpool library

//extern int errno;

void dispatch_to_me(void *arg) {
	int seconds = *((int*) arg);

	fprintf(stdout, "  in dispatch %d\n", seconds);
	sleep(seconds);
	fprintf(stdout, "  done dispatch %d\n", seconds);
}

int main() {
	Threadpool tp(7);			//creating threadpool
//	int i = 3, j = 6, k = 7;
	int sec[12];
	for (int i = 0; i < 12; ++i)
		sec[i] = i+1;

//	sleep(10);
	for (int i = 0; i < 12; ++i) {
		fprintf(stdout, "**main** dispatch %d\n", i+1);
		tp.dispatch(dispatch_to_me, (void *) &sec[i]);		//dispatching tasks
	}

//	fprintf(stdout, "**main** dispatch 3\n");
//	tp.dispatch(dispatch_to_me, (void *) &i);
//	fprintf(stdout, "**main** dispatch 6\n");
//	tp.dispatch(dispatch_to_me, (void *) &j);
//	fprintf(stdout, "**main** dispatch 7\n");
//	tp.dispatch(dispatch_to_me, (void *) &k);
//
//	fprintf(stdout, "**main** done first\n");
//	sleep(10);
//	fprintf(stdout, "\n\n");
//
//	fprintf(stdout, "**main** dispatch 3\n");
//	tp.dispatch(dispatch_to_me, (void *) &i);
//	fprintf(stdout, "**main** dispatch 6\n");
//	tp.dispatch(dispatch_to_me, (void *) &j);
//	fprintf(stdout, "**main** dispatch 7\n");
//	tp.dispatch(dispatch_to_me, (void *) &k);
//
//	fprintf(stdout, "**main done second\n");
	sleep(18);
	cout<<"Finished sleeping"<<endl;
	tp.~Threadpool();			//destroying threadpool
	sleep(2);
	cout<<"Finished"<<endl;
	exit(-1);
}

