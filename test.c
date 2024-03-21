#include "src/croutine.h"
#include "defer.h"
#include <unistd.h>

int sort(int n) {
    sleep(n);
    printf("%3d", n);
    return n;
}

void sleep_sort(int size, int arr[static size]) {
    typedef struct {
        int     n;
        wg_t*   wg;
    } wrapper_args;
    void wrapper(void* args) {
        wrapper_args* arg = args;
        defer({
            wg_done(arg->wg);
            free(arg);
        });
        sort(arg->n);
    }
    
    wg_t* wg = wg_init();
    wg_add(wg, size);
    defer(wg_deinit(wg));
    for (int i = 0; i < size; i++) {
        wrapper_args* args = malloc(sizeof (wrapper_args));
        args->n = arr[i];
        args->wg = wg;
        co(wrapper, args);
    }
    wg_wait(wg);
    printf("\n");
}

int main(void) {
    int arr[] = {6,9,4,2,0};
    sleep_sort(sizeof (arr) / sizeof (int), arr);
    return 0;
}

