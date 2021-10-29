#include "ctrip_thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    ctrip_thread_info cti;
    cti.ctrip_init_thread_pool(5);
    struct ctrip_task* task = NULL;
    int i;
    for (i = 0; i < 100; ++i)
    {
        task = (struct ctrip_task*)malloc(sizeof(struct ctrip_task));
        task->value = i + 1;
        task->pNext = NULL;
        printf("add task, task value [%d]\n", task->value);
        cti.ctrip_thread_pool_add_task(task);
    }

    sleep(10);

    cti.ctrip_destory_thread_pool();
    
    return 0;
}
