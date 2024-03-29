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

class ctrip_thread_info{
public:
    static void* thread_func(void*);

    void ctrip_init_thread_pool(int thread_num);

    void ctrip_destory_thread_pool();

    void ctrip_thread_pool_add_task(struct ctrip_task* t);

    struct ctrip_task* ctrip_thread_pool_retrieve_task();

    void ctrip_thread_pool_do_task(struct ctrip_task* t);

    void* ctrip_thread_routine();
private:
    int _thread_running;
    int _thread_num;
    int _tasknum;
    struct ctrip_task* _tasks;
    pthread_t* _threadid;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};


