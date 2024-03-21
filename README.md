# croutine
goroutine, but in C

# Usage
a simple example is the `sleep_sort` algorithm showing in `test.c`. It utlizes nested function extension from gcc, so it may not compile if you are on clang or other compiler, but the idea is the same. 

# supported module
- goroutine
- channel
- waitGroup

# Caution!
goroutine uses a M:N design, where in each kernel thread itself, it performs user space context switch. 

But in C, although it is doable with `ucontext.h` or manually save program counter, registers, etc., it is difficult to have a stable and crossplatform way to do that. I could've use `ucontext.h` and implement a posix only coroutine, but it turns out to be almost impossible to perform work stealing, since it is not possible to change `uc_link` after calling `makecontext`. 

So that croutine just spawn a new kernel thread when all threads in the thread pool is busy working. And it is not really a coroutine, it is just a thread pool that grow automatically. 
