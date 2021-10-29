#include"ctrip_thread_pool.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>



void* ctrip_thread_info::thread_func(void* param){
    ctrip_thread_info* obj = static_cast<ctrip_thread_info*> (param);
    obj->ctrip_thread_routine();
    return NULL;
}
void ctrip_thread_info::ctrip_init_thread_pool(int thread_num){
    if(thread_num <= 0){
        thread_num = 5;
    }

    //初始化条件变量和互斥锁
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);

    
    thread_num = thread_num;
    _thread_running = 1;
    _tasknum = 0;
    _tasks = NULL;
    _threadid = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);

    for (int i = 0;i < thread_num; ++i) {
        pthread_create(&_threadid[i], NULL, ctrip_thread_info::thread_func, this);
    }
}

void ctrip_thread_info::ctrip_destory_thread_pool(){
    _thread_running = 0;
    _thread_running = 0;
    pthread_cond_broadcast(&_cond);

    for (int i = 0;i < _thread_num; ++i) {
        pthread_join(_threadid[i], NULL);
    }

    free(_threadid);
    
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond);

}

void ctrip_thread_info::ctrip_thread_pool_add_task(struct ctrip_task* t){
    if (t == NULL)
        return;
    pthread_mutex_lock(&_mutex);
    struct ctrip_task* head = _tasks;
    if(head == NULL) {
        _tasks = t;
    } else {
        while (head->pNext != NULL) {
            head = head->pNext;
        }
        head->pNext = t;
    }

    ++_tasknum;
    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);
}

struct ctrip_task* ctrip_thread_info::ctrip_thread_pool_retrieve_task(){
    struct ctrip_task* head = _tasks;

    if(head != NULL) {
        _tasks=head->pNext;
        --_tasknum;
        printf("retrieve a task, task value is [%d]\n", head->value);
        return head;
    }
    printf("no task\n");

    return NULL;
}

void ctrip_thread_info::ctrip_thread_pool_do_task(struct ctrip_task* t){
    if (t == NULL){
        return;
    }

    printf("task value is [%d]\n", t->value);
}

void* ctrip_thread_info::ctrip_thread_routine(){
    printf("therad NO.%d start.\n", (int)pthread_self());

    while(_thread_running){
        struct ctrip_task* current = NULL;
        pthread_mutex_lock(&_mutex);
        
        while(_tasknum <= 0) {
            pthread_cond_wait(&_cond,&_mutex);

            if (!_thread_running)
                break;
        }

        current = ctrip_thread_pool_retrieve_task();
        pthread_mutex_unlock(&_mutex);
        ctrip_thread_pool_do_task(current);
    }
    printf("thread NO.%d\n", (int)pthread_self());
}
