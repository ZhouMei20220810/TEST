/*
 * Utils.h
 *
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

#ifndef DISALLOW_EVIL_CONSTRUCTORS
#define DISALLOW_EVIL_CONSTRUCTORS(name) \
    name(const name &); \
    name &operator=(const name &)
#endif

#ifndef ALIGN4
#define ALIGN4(x) ((x+3)&(~3))
#endif
#ifndef ALIGN16
#define ALIGN16(x)  ((x+15)&~15)
#endif
#ifndef ALIGN64
#define ALIGN64(x)  ((x+63)&~63)
#endif

#endif /* UTILS_H_ */
