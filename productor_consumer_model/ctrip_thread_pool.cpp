#include"ctrip_thread_pool.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<thread>

void ctrip_thread_info::ctrip_init_thread_pool(int thread_num){
    if(thread_num <= 0){
        thread_num = 5;
    }
   
    _thread_num = thread_num;
    _thread_running = 1;
    _tasknum = 0;
    _tasks = NULL;
    _threads = new std::thread[_thread_num];
    for (int i = 0;i < _thread_num; ++i) {
        _threads[i] = std::move(std::thread(&ctrip_thread_info::ctrip_thread_routine, this));
    }
}

void ctrip_thread_info::ctrip_destory_thread_pool(){
    _thread_running = 0;
    _cond.notify_all();
     for (int i =0 ;i < _thread_num; ++i)
        _threads[i].join();
}

void ctrip_thread_info::ctrip_thread_pool_add_task(struct ctrip_task* t){
    if (t == NULL)
        return;
    std::unique_lock<std::mutex> locker(_mutex);
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
    _cond.notify_one();
    locker.unlock();
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
    printf("therad NO.%lu start.\n", std::this_thread::get_id());

    while(_thread_running){
        struct ctrip_task* current = NULL;
        std::unique_lock<std::mutex> locker(_mutex);
        while(_tasknum <= 0) {
            _cond.wait(locker);
            if (!_thread_running)
                break;
        }

        current = ctrip_thread_pool_retrieve_task();
        _mutex.unlock();
        ctrip_thread_pool_do_task(current);
    }
    printf("thread NO.%lu\n", std::this_thread::get_id());
}
