#include"ctrip_thread_pool.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

struct ctrip_thread_info g_threadinfo;
int thread_running = 0;

void ctrip_init_thread_pool(int thread_num){
    if(thread_num <= 0){
        thread_num = 5;
    }

    //初始化条件变量和互斥锁
    pthread_mutex_init(&g_threadinfo.mutex, NULL);
    pthread_cond_init(&g_threadinfo.cond, NULL);

    
    g_threadinfo.thread_num = thread_num;
    g_threadinfo.thread_running = 1;
    g_threadinfo.tasknum = 0;
    g_threadinfo.tasks = NULL;
    thread_running = 1;
    g_threadinfo.threadid = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);

    for (int i = 0;i < thread_num; ++i) {
        pthread_create(&g_threadinfo.threadid[i], NULL, ctrip_thread_routine, NULL);
    }
}

void ctrip_destory_thread_pool(){
    g_threadinfo.thread_running = 0;
    thread_running = 0;
    pthread_cond_broadcast(&g_threadinfo.cond);

    for (int i = 0;i < g_threadinfo.thread_num; ++i) {
        pthread_join(g_threadinfo.threadid[i], NULL);
    }

    free(g_threadinfo.threadid);
    
    pthread_mutex_destroy(&g_threadinfo.mutex);
    pthread_cond_destroy(&g_threadinfo.cond);

}

void ctrip_thread_pool_add_task(struct ctrip_task* t){
    if (t == NULL)
        return;
    pthread_mutex_lock(&g_threadinfo.mutex);
    struct ctrip_task* head = g_threadinfo.tasks;
    if(head == NULL) {
        g_threadinfo.tasks = t;
    } else {
        while (head->pNext != NULL) {
            head = head->pNext;
        }
        head->pNext = t;
    }

    ++g_threadinfo.tasknum;
    pthread_cond_signal(&g_threadinfo.cond);
    pthread_mutex_unlock(&g_threadinfo.mutex);
}

struct ctrip_task* ctrip_thread_pool_retrieve_task(){
    struct ctrip_task* head = g_threadinfo.tasks;

    if(head != NULL) {
        g_threadinfo.tasks=head->pNext;
        --g_threadinfo.tasknum;
        printf("retrieve a task, task value is [%d]\n", head->value);
        return head;
    }
    printf("no task\n");

    return NULL;
}

void ctrip_thread_pool_do_task(struct ctrip_task* t){
    if (t == NULL){
        return;
    }

    printf("task value is [%d]\n", t->value);
}

void* ctrip_thread_routine(void* thread_param){
    printf("therad NO.%d start.\n", (int)pthread_self());

    while(thread_running){
        struct ctrip_task* current = NULL;
        pthread_mutex_lock(&g_threadinfo.mutex);
        
        while(g_threadinfo.tasknum <= 0) {
            pthread_cond_wait(&g_threadinfo.cond,&g_threadinfo.mutex);

            if (!g_threadinfo.thread_running)
                break;
        }

        current = ctrip_thread_pool_retrieve_task();
        pthread_mutex_unlock(&g_threadinfo.mutex);
        ctrip_thread_pool_do_task(current);
    }
    printf("thread NO.%d\n", (int)pthread_self());
}