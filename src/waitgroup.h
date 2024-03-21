#ifndef __WAITGROUP_H__
#define __WAITGROUP_H__


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>


typedef struct waitgroup wg_t;


wg_t*   wg_init(void);
void    wg_deinit(wg_t* wg);
void    wg_add(wg_t* wg, size_t num);
void    wg_done(wg_t* wg);
void    wg_wait(wg_t* wg);



#endif  // __WAITGROUP_H__
