/*	list.h
 *
 * 	A list contains 0 of more listnodes. The list object is a header which
 *	points to the first listnode. The last listnode points to the header.
 *	Every listnode points to the object which is stored in the list.
 *
 * 	2016	K.W.E. de Lange
 */
#if !defined (_LIST_)
#define _LIST_

#include "object.h"

typedef struct listobject {
	OBJ_HEAD;
	struct listnode *head;	/* first node in the list, NULL for empty list */
	struct listnode *tail;	/* last node in the list, NULL for lege list */
} ListObject;

typedef struct listnode {
	OBJ_HEAD;
	struct listnode *next;	/* next node in the list, NULL for last */
	struct listnode *prev;	/* previous node in the list, NULL for first */
	struct object *obj;  	/* opgeslagen object */
} ListNode;

extern TypeObject listobject;
extern TypeObject listnodeobject;

extern Object *list_length(ListObject *list);
extern ListNode *list_item(ListObject *list, int index);
extern ListObject *list_slice(ListObject *list, int start, int end);
extern Object *list_concat(ListObject *op1, ListObject *op2);
extern Object *list_repeat(Object *op1, Object *op2);
extern void listnode_insert(ListObject *list, int index, Object *obj);
extern void listnode_append(ListObject *list, Object *obj);
extern Object *listnode_remove(ListObject *list, int index);

#endif
