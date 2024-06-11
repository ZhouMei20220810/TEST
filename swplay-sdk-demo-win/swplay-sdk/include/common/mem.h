/*
 * mem.h
 *
 *  Created on: Oct 2, 2017
 *      Author: root
 */

#ifndef AWE_MEM_H_
#define AWE_MEM_H_

#include <common/common.h>

AWE_BEGIN_DECLS

AWE_DECLARE(void*) awe_malloc(size_t size);
AWE_DECLARE(void*) awe_mallocz(size_t size);

AWE_DECLARE(void) awe_free(void* mem);

AWE_END_DECLS

#endif /* AWE_MEM_H_ */
