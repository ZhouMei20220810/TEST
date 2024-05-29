/*
 * atomic.h
 *
 */

#ifndef AWE_ATOMIC_H_
#define AWE_ATOMIC_H_

#if defined(_WIN32)
#include <compat/win32/stdatomic.h>
#else
#include <compat/gcc/stdatomic.h>
#endif

#endif /* AWE_ATOMIC_H_ */
