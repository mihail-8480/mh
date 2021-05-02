#ifndef MHSERV_MH_THREAD_H
#define MHSERV_MH_THREAD_H

#include <mh.h>

// Code that needs to get executed on a new thread
typedef void *(*mh_thread_action_t)(void *);

// Do an action on a new thread
MH_API_FUNC(void mh_thread_create(mh_thread_action_t action, void *args));

// Exit from the current thread
MH_NORETURN MH_API_FUNC(void mh_thread_exit(void *ret));

#endif //MHSERV_MH_THREAD_H
