/*
 * object.h
 *
 */

#ifndef AWE_OBJECT_H_
#define AWE_OBJECT_H_

#include <common/common.h>

#define OBJECT_DEC \
    volatile int _refs

#define object_init(obj) (obj)->_refs = 1
//#define object_ref_inc(obj) atomic_fetch_add(&(obj)->_refs, 1)
//#define object_ref_dec(obj) atomic_fetch_sub(&(obj)->_refs, 1)

typedef struct awe_object{
    OBJECT_DEC;
}awe_object;

AWE_BEGIN_DECLS

int32_t object_ref_inc(awe_object* o);
int32_t object_ref_dec(awe_object* o);

AWE_END_DECLS

#endif /* AWE_OBJECT_H_ */
