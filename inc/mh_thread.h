#ifndef MHSERV_MH_THREAD_H
#define MHSERV_MH_THREAD_H

#include "mh.h"

/*
 * <mh_tests.h>
 * The libmh threading header.
 *
 * Contains functions for creating threads and exiting threads.
 */

// Code that needs to get executed on a new thread.
typedef mh_ref_t (*mh_thread_action_t)(mh_ref_t);

// Do an action on a new thread.
MH_API_FUNC(void mh_thread_create(mh_thread_action_t action, mh_ref_t args));

// Exit from the current thread.
MH_NORETURN MH_API_FUNC(void mh_thread_exit(mh_ref_t ret));

#endif //MHSERV_MH_THREAD_H
