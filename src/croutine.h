#ifndef __CROUTINE_H__
#define __CROUTINE_H__


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "channel.h"
#include "waitgroup.h"


typedef void (*task_fn)(void*);

void    co(task_fn func, void* args);


#endif  // __CROUTINE_H__
