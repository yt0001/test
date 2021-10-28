#pragma once
#include<pthread.h>

#ifndef NULL
#define NULL 0
#endif

#define PUBLIC

PUBLIC struct ctrip_task {
    struct ctrip_task* pNext;
    int value;
};

struct ctrip_thread_info{
    int thread_running;
    int thread_num;
    int tasknum;
    struct ctrip_task* tasks;
    pthread_t* threadid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

 void ctrip_init_thread_pool(int thread_num);

 void ctrip_destory_thread_pool();

 void ctrip_thread_pool_add_task(struct ctrip_task* t);

struct ctrip_task* ctrip_thread_pool_retrieve_task();

 void ctrip_thread_pool_do_task(struct ctrip_task* t);

void* ctrip_thread_routine(void* thread_param);
