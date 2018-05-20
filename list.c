/* list.c
 *
 * List object operations
 *
 * 2016 K.W.E. de Lange
 */
#include <stdlib.h>

#include "object.h"
#include "error.h"


/* Create a new empty list object.
 */
static ListObject *list_alloc(void)
{
	ListObject *list;

	if ((list = calloc(1, sizeof(ListObject))) == NULL)
		error(OutOfMemoryError);

	list->typeobj = &listobject;
	list->type = LIST_T;
	list->refcount = 0;

	list->head = NULL;
	list->tail = NULL;

	return list;
}


/* Free a list object, including all list nodes and referenced objects.
 */
static void list_free(ListObject *list)
{
	Object *obj;

	debug_printf(DEBUGLEVEL3, "\n%p: free %s\n", (void *)list, TYPENAME(list));

	while ((obj = listnode_remove(list, 0)) != NULL)
		obj_decref(obj);

	free(list);
}


static void list_print(ListObject *list)
{
	ListNode *node;

	printf("[");

	for (node = list->head; node; node = node->next) {
		obj_print(node->obj);
		if (node->next)
			printf(",");
	}
	printf("]");
}


/* Create a copy of a list.
 *
 * The new list contains new objects (= deep copy).
 */
static ListObject *list_set(ListObject *dest, ListObject *src)
{
	Object *obj;
	ListNode *node;

	if (dest->head != NULL)
		while ((obj = listnode_remove(dest, 0)) != NULL)
			obj_decref(obj);

	for (node = src->head; node; node = node->next)
		listnode_append(dest, obj_copy(node->obj));

	return dest;
}


static ListObject *list_vset(ListObject *obj, va_list argp)
{
	return list_set(obj, va_arg(argp, ListObject *));
}


/* Create a new empty listnode.
 */
static ListNode *listnode_alloc(void)
{
	ListNode *node;

	if ((node = calloc(1, sizeof(ListNode))) == NULL)
		error(OutOfMemoryError);

	node->typeobj = &listnodeobject;
	node->type = LISTNODE_T;
	node->refcount = 0;

	node->next = NULL;
	node->prev = NULL;
	node->obj = NULL;

	return node;
}


/* Free a listnode, and release the object it references.
 */
static void listnode_free(ListNode *node)
{
	debug_printf(DEBUGLEVEL3, "\n%p: free %s", (void *)node, TYPENAME(node));

	if (node->obj)
		obj_decref(node->obj);

	free(node);
}


static void listnode_print(ListNode *node)
{
	obj_print(node->obj);
}


static ListNode *listnode_set(ListNode *node, Object *obj)
{
	if (node->obj)
		obj_decref(node->obj);

	node->obj = obj;

	return node;
}


static ListNode *listnode_vset(ListNode *node, va_list argp)
{
	return listnode_set(node, va_arg(argp, Object *));
}


/* Count the number of listnodes in a list.
 */
static int_t length(ListObject *list)
{
	ListNode *node;
	int_t i;

	for (i = 0, node = list->head; node; i++, node = node->next)
		;

	return i;
}


Object *list_length(ListObject *list)
{
	return obj_create(INT_T, length(list));
}


/* Create a new list which contains the objects from op1 and op2.
 */
Object *list_concat(ListObject *op1, ListObject *op2)
{
	ListObject *list;
	int i;

	list = (ListObject *)obj_alloc(LIST_T);

	for (i = 0; i < length(op1); i++)
		listnode_append(list, obj_copy(list_item(op1, i)->obj));

	for (i = 0; i < length(op2); i++)
		listnode_append(list, obj_copy(list_item(op2, i)->obj));

	return (Object *)list;
}


/* Create a new list which contains n times an existing list.
 */
Object *list_repeat(Object *op1, Object *op2)
{
	ListObject *list;
	size_t times;
	int i;

	Object *s = TYPE(op1) == LIST_T ? op1 : op2;
	Object *n = TYPE(op1) == LIST_T ? op2 : op1;

	times = obj_as_int(n);

	list = (ListObject *)obj_alloc(LIST_T);

	while (times--)
		for (i = 0; i < length((ListObject *)s); i++)
			listnode_append(list, obj_copy(list_item((ListObject *)s, i)->obj));

	return (Object *)list;
}


/* Retrieve a listnode from a list by index.
 */
ListNode *list_item(ListObject *list, int index)
{
	ListNode *node;
	int len, i;

	len = length(list);

	if (index < 0)
		index += len;

	if (index < 0 || index >= len)
		return NULL;  /* IndexError: index out of range */

	for (i = 0, node = list->head; node; i++, node = node->next) {
		if (i == index) {
			break;
		}
	}

	obj_incref(node);

	return node;
}


/* Create a new list from a slice of an existing list.
 *
 * The new list contains new object (= deep copy). Start and end are
 * automatically adjusted to the nearest possible values.
 */
ListObject *list_slice(ListObject *list, int start, int end)
{
	ListObject *slice;
	int len, i;

	len = length(list);

	if (start < 0)
		start += len;

	if (end < 0)
		end += len;

	if (start < 0)
		start = 0;

	if (end >= len)
		end = len;

	slice = (ListObject *)obj_alloc(LIST_T);

	for (i = start; i < end; i++)
		listnode_append(slice, obj_copy(list_item(list, i)->obj));

	return slice;
}


/* Append an object to the end of a list.
 */
void listnode_append(ListObject *list, Object *obj)
{
	ListNode *node, *tail;

	node = (ListNode *)obj_create(LISTNODE_T, obj);

	if (list->head == NULL) {  /* append to empty list */
		list->head = node;
		list->tail = node;
	} else {  /* append to list which has one of more listnodes */
		tail = list->tail;
		node->prev = tail;
		tail->next = node;
		list->tail = node;
	}
}


/* Insert an object before the listnode with number index.
 *
 * Index is automatically adjusted to the nearest possible value.
 * A negative index counts back from the end of the list. Index -1
 * points to the last listnode.
 */
void listnode_insert(ListObject *list, int index, Object *obj)
{
	ListNode *node, *iptr;
	int len;

	node = (ListNode *)obj_create(LISTNODE_T, obj);

	if (list->head == NULL) {  /* insert in empty list */
		list->head = node;
		list->tail = node;
	} else {  /* insert in list which has one or more listnodes */
		len = length(list);

		if (index < 0)
			index += len;

		if (index <= 0) {  /* insert before first listnode */
			node->next = list->head;
			node->next->prev = node;
			node->prev = NULL;
			list->head = node;
		} else if (index >= len) {  /* insert after last listnode */
			iptr = list->tail;
			node->prev = iptr;
			iptr->next = node;
			list->tail = node;
		} else {  /* insert somewhere in the middle */
			for (iptr = list->head; index--; iptr = iptr->next)
				;
			/* insert before iptr */
			node->next = iptr;
			node->prev = iptr->prev;
			node->next->prev = node;
			node->prev->next = node;
		}
	}
}


/* Remove a listnode from a list.
 *
 * Index must exists (nmbering starts at 0).
 * A negative index counts back from the end of the list. Index -1
 * points to the last listnode.
 */
Object *listnode_remove(ListObject *list, int index)
{
	ListNode *node;
	Object *obj = NULL;
	int len, i;

	len = length(list);

	if (index < 0)
		index += len;  /* negative index */

	if (index < 0 || index >= len)
		return NULL;  /* IndexError: index out of range */

	for (i = 0, node = list->head; node; i++, node = node->next) {
		if (i == index) {
			obj = node->obj;
			if (list->head == list->tail) {  /* list contains only 1 node */
				list->head = NULL;
				list->tail = NULL;
			} else if (node->prev == NULL) {  /* at least 2 nodes, remove first */
				list->head = node->next;
				node->next->prev = NULL;
			} else if (node->next == NULL) {  /* at least 2 nodes, remove last */
				list->tail = node->prev;
				node->prev->next = NULL;
			} else {  /* at least 3 nodes, node is not first or last */
				node->prev->next = node->next;
				node->next->prev = node->prev;
			}
			obj_incref(obj);  /* avoid that obj (= return value) is released */
			obj_decref(node);
			break;
		}
	}
	return obj;
}



/* List object API.
 */
TypeObject listobject = {
	"list",
	(Object *(*)())list_alloc,
	(void (*)(Object *))list_free,
	(void (*)(Object *))list_print,
	(Object *(*)())list_set,
	(Object *(*)(Object *, va_list))list_vset
};


/* Listnode object API.
 */
TypeObject listnodeobject = {
	"listnode",
	(Object *(*)())listnode_alloc,
	(void (*)(Object *))listnode_free,
	(void (*)(Object *))listnode_print,
	(Object *(*)())listnode_set,
	(Object *(*)(Object *, va_list))listnode_vset
};
