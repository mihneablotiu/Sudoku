#ifndef __SO_THREADPOOL_H__
#define __SO_THREADPOOL_H__ 1

#include <pthread.h>

typedef struct {
	void *argument;
	void (*task)(void *);
} os_task_t;

typedef struct _node{
	os_task_t *task;
	struct _node *next;
} os_task_queue_t;

typedef struct {
	unsigned int start;

	unsigned int num_threads;
	unsigned int waiting_threads;
	pthread_t *threads;

	os_task_queue_t *tasks;
	pthread_mutex_t taskLock;
	pthread_mutex_t initialMutex;
	pthread_mutex_t print_mutex;

	pthread_cond_t work_started;
	pthread_cond_t more_work;
} os_threadpool_t;


os_task_t *task_create(void *arg, void (*f)(void *));
void add_task_in_queue(os_threadpool_t *tp, os_task_t *t);
os_task_t *get_task(os_threadpool_t *tp);
os_threadpool_t *threadpool_create(unsigned int nThreads);
void *thread_loop_function(void *args);
void threadpool_stop(os_threadpool_t *tp);

#endif
