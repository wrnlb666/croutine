#include "croutine.h"


typedef struct task {
    task_fn func;
    void*   args;
} task_t;


static chan_t* free_ch = NULL;
static void* worker(void* ch) {
    pthread_detach(pthread_self());
    
    for (;;) {
        task_t* task = chan_recv(ch);
        if (task == NULL) {
            break;
        }
        task->func(task->args);
        free(task);
        if (free_ch != NULL) {
            chan_send(free_ch, ch);
        } else {
            break;
        }
    }

    // cleanup
    return NULL;
}

static chan_t* task_ch = NULL;
static void* sysmon(void* arg) {
    (void)arg;
    
    task_t*     task;
    chan_t*     target;
    pthread_t   tid;
    
    for (;;) {
        task = chan_recv(task_ch);
        if (task == NULL) {
            break;
        }
        if (chan_try_recv(free_ch, (void**)&target)) {
            chan_send(target, task);
        } else {
            target = chan_init(0);
            pthread_create(&tid, NULL, worker, target);
            chan_send(target, task);
        }
    }

    // cleanup 
    target = NULL;
    while (chan_try_recv(free_ch, (void**)&target) == true) {
        chan_send(target, NULL);
        chan_deinit(target);
    }
    
    return NULL;
}

static pthread_t sysmon_tid;
__attribute__((constructor(101)))
static void croutine_runtime_init(void) {
    task_ch = chan_init(-1);
    free_ch = chan_init(-1);
    pthread_create(&sysmon_tid, NULL, sysmon, NULL);
}

__attribute__((destructor(101)))
static void croutine_runtime_deinit(void) {
    chan_send(task_ch, NULL);
    pthread_join(sysmon_tid, NULL);
    chan_deinit(free_ch);
    free_ch = NULL;
    chan_deinit(task_ch);
    task_ch = NULL;
}

void co(task_fn func, void* args) {
    task_t* task = malloc(sizeof (task_t));
    task->func = func;
    task->args = args;

    chan_send(task_ch, task);
}
