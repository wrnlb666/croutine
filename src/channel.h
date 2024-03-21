#ifndef __CHANNEL_H__
#define __CHANNEL_H__


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct chan chan_t;
typedef struct chan_select {
    chan_t* ch;
    bool    is_send;
    union {
        void*   data;
        void**  result;
    };
} chan_select_t;

// constructor and destructor
chan_t* chan_init(size_t buf_size);
void    chan_deinit(chan_t* ch);

// `len()` and `cap()` like what go's chan has
size_t  chan_len(chan_t* ch);
size_t  chan_cap(chan_t* ch);

// API for basic send and recv
void    chan_send(chan_t* ch, void* data);
void*   chan_recv(chan_t* ch);

// API for checking if the speficied channel may block
bool    chan_send_blocked(chan_t* ch);
bool    chan_recv_blocked(chan_t* ch);
bool    chan_send_will_block(chan_t* ch);
bool    chan_recv_will_block(chan_t* ch);

// API for non-blocking channel operation
bool    chan_try_send(chan_t* ch, void* data);
bool    chan_try_recv(chan_t* ch, void** result);
int     chan_select(size_t num, chan_select_t* chs, bool has_default);


#endif  // __CHANNEL_H__
