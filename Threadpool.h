#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <bits/stdc++.h>
using namespace std;

#define MAXT_IN_POOL 200
#define THREAD_MIN 20
#define THREAD_INC 10

typedef void (*dispatch_fn)(void *);

class Threadpool {
private:
	int max_threads, available_threads, dispatched_threads;
	queue<pair<dispatch_fn, void*>> tasks;
	mutex m;
	condition_variable_any q_not_empty;

	void thread_exec();
	void resize(bool);
public:
	Threadpool();
	Threadpool(int);
	void dispatch(dispatch_fn, void*);
	void destroy_threadpool();
	virtual ~Threadpool();
};

#endif /* THREADPOOL_H_ */
