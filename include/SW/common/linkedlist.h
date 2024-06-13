/*
 * linkedlist.h
 *
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdint.h>

#include <common/dlist.h>
#include <common/common.h>
#include <common/mem.h>
#include <common/object.h>

AWE_BEGIN_DECLS

typedef void object_autorelease_proc(awe_object **e);

typedef struct linkedlist{
    struct list_head _linkedhead; //list head

    object_autorelease_proc *_release_proc;

    int32_t _size;
}linkedlist;

typedef struct linked_obj{
    struct list_head _head; //list entry
    awe_object *_object;
}linked_obj;

status_t linkedlist_init(linkedlist *list, object_autorelease_proc *proc);

status_t linkedlist_add(linkedlist *list, awe_object *obj);

awe_object* linkedlist_removeFirst(linkedlist *list);
awe_object* linkedlist_remove(linkedlist *list,
        bool (*equals)(awe_object *e, void* context), void* context);

status_t linkedlist_del(linkedlist *list, awe_object *obj);

status_t linkedlist_del2(linkedlist *list, struct list_head *pos);

status_t linkedlist_del3(linkedlist *list,
        bool (*equals)(awe_object *e, void* context), void* context);

bool linkedlist_contains(linkedlist *list, awe_object *obj);

void linkedlist_clear(linkedlist *list);

int32_t linkedlist_size(linkedlist *list);

awe_object* linkedlist_getFirst(linkedlist *list);
awe_object* linkedlist_getLast(linkedlist *list);

awe_object* linkedlist_get(linkedlist *list,
        bool (*equals)(awe_object *e, void* context), void* context);

awe_object* linkedlist_getByIndex(linkedlist *list, int index);

void linkedlist_forEach(linkedlist *list,
        bool (*callback)(struct list_head *pos, awe_object *e, void* context),
        void* context);

#define linkedlist_for(pos, list)        \
    struct list_head* _##pos = NULL;                \
    struct list_head* _##pos##tmp = NULL;            \
    list_for_each_safe(_##pos, _##pos##tmp, &(list)->_linkedhead)

//not safe
#define linkedlist_for_prev(pos, list)        \
        struct list_head* _##pos = NULL;        \
        list_for_each_prev(_##pos, &(list)->_linkedhead)

#define linkedlist_entry(pos)                    \
    (list_entry(_##pos, awe_linked_obj, _head)->_object)

#define linkedlist_del_pos(list, pos)    \
        linkedlist_del2(list, _##pos)

//---------------------------------------------------------------------
bool linkedlist_item_equals(awe_object *obj, void* context);


AWE_END_DECLS

#endif /* LINKEDLIST_H_ */
