#include "waitgroup.h"
#include <pthread.h>


typedef struct waitgroup {
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
    size_t              size;
} wg_t;


wg_t* wg_init(void) {
    wg_t* wg = malloc(sizeof (wg_t));
    if (pthread_mutex_init(&wg->mutex, NULL) != 0) {
        free(wg);
        return NULL;
    }
    if (pthread_cond_init(&wg->cond, NULL) != 0) {
        pthread_mutex_destroy(&wg->mutex);
        free(wg);
        return NULL;
    }
    wg->size = 0;
    return wg;
}

void wg_deinit(wg_t* wg) {
    pthread_cond_destroy(&wg->cond);
    pthread_mutex_destroy(&wg->mutex);
    free(wg);
}

void wg_add(wg_t* wg, size_t num) {
    pthread_mutex_lock(&wg->mutex);
    wg->size += num;
    pthread_mutex_unlock(&wg->mutex);
}

void wg_done(wg_t* wg) {
    pthread_mutex_lock(&wg->mutex);
    wg->size -= 1;
    if (wg->size == 0) {
        pthread_cond_signal(&wg->cond);
    }
    pthread_mutex_unlock(&wg->mutex);
}

void wg_wait(wg_t* wg) {
    pthread_mutex_lock(&wg->mutex);
    while (wg->size != 0) {
        pthread_cond_wait(&wg->cond, &wg->mutex);
    }
    pthread_mutex_unlock(&wg->mutex);
}
