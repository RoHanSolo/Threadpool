#include "Threadpool.h"

Threadpool::Threadpool(int num_threads) {

	max_threads = num_threads > MAXT_IN_POOL ? MAXT_IN_POOL : num_threads;
	available_threads = num_threads < THREAD_MIN ? num_threads : THREAD_MIN;
	dispatched_threads = 0;

	for (int i = 0; i < available_threads; ++i) {
		thread t(&Threadpool::thread_exec, this);
		t.detach();
	}
}

Threadpool::Threadpool() {

	max_threads = available_threads = 20;
	dispatched_threads = 0;

	for (int i = 0; i < available_threads; ++i) {
		thread t(&Threadpool::thread_exec, this);
		t.detach();
	}
}

void poison(void *arg) {
	pthread_exit(arg);
}

void Threadpool::resize(bool inc) {
	int old_size = available_threads;

	if (inc) {
		available_threads = available_threads + THREAD_INC > max_threads ? max_threads : available_threads + THREAD_INC;

		for (int i = old_size; i < available_threads; ++i) {
			thread t(&Threadpool::thread_exec, this);
			t.detach();
		}
	}
	else {
		available_threads = available_threads - THREAD_INC < THREAD_MIN ? THREAD_MIN : available_threads - THREAD_INC;

		for (int i = available_threads; i < old_size; i++) {
			tasks.push(make_pair(&poison, (void *)EXIT_SUCCESS));
			q_not_empty.notify_one();
		}
	}
}

void Threadpool::thread_exec() {

	while (1) {
		m.lock();
		while (tasks.empty())
			q_not_empty.wait(m);

		void (*func)(void *) = tasks.front().first;
		void *arg = tasks.front().second;
		tasks.pop();

		dispatched_threads++;

		if (dispatched_threads + THREAD_INC > available_threads && available_threads != max_threads)
			resize(1);

		m.unlock();

		(func)(arg);

		m.lock();
		dispatched_threads--;
		if (dispatched_threads + THREAD_MIN + THREAD_INC < available_threads && available_threads != THREAD_MIN)
			resize(0);

		m.unlock();
	}
}

void Threadpool::dispatch(dispatch_fn dispatch_to_here, void *arg) {

	m.lock();
	tasks.push(make_pair(dispatch_to_here, arg));
	q_not_empty.notify_one();
	m.unlock();
}

Threadpool::~Threadpool() {

	for (int i = 0; i < available_threads; ++i)
		tasks.push(make_pair(&poison, (void *)EXIT_SUCCESS));
	max_threads = 0;
	available_threads = 0;
	q_not_empty.notify_all();
}
