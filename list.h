/* list.h
 *
 * A list contains 0 of more listnodes. The list object is a header which
 * points to the first listnode. The last listnode points to the header.
 * Every listnode points to the object which is stored in the list. In
 * this way the list structure is agnostic of the object type stored.
 *
 * 2016	K.W.E. de Lange
 */
#ifndef _LIST_
#define _LIST_

#include "object.h"

typedef struct listobject {
	OBJ_HEAD;
	struct listnode *head;	/* first node in the list, NULL for empty list */
	struct listnode *tail;	/* last node in the list, NULL for empty list */
} ListObject;

typedef struct listnode {
	OBJ_HEAD;
	struct listnode *next;	/* next node in the list, NULL for last */
	struct listnode *prev;	/* previous node in the list, NULL for first */
	struct object *obj;  	/* object which is stored in the list */
} ListNode;

typedef struct {
	TYPE_HEAD;
	Object *(*length)(ListObject *obj);
	ListNode *(*item)(ListObject *str, int index);
	ListObject *(*slice)(ListObject *obj, int start, int end);
	Object *(*concat)(ListObject *op1, ListObject *op2);
	Object *(*repeat)(Object *op1, Object *op2);
	Object *(*eql)(ListObject *op1, ListObject *op2);
	Object *(*neq)(ListObject *op1, ListObject *op2);
	void (*insert)(ListObject *list, int index, Object *obj);
	void (*append)(ListObject *list, Object *obj);
	Object *(*remove)(ListObject *list, int index);
} ListType;

extern ListType listtype;

typedef struct {
	TYPE_HEAD;
} ListNodeType;

extern ListNodeType listnodetype;

#endif
