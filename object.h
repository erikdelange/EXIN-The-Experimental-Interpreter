/*	object.h
 *
 * 	1994	K.W.E. de Lange
 */
#if !defined (_OBJECT_)
#define _OBJECT_

#include <stdarg.h>
#include "config.h"

typedef enum { UNDEFINED, CHAR_T, INT_T, FLOAT_T, STR_T,
			   LIST_T, LISTNODE_T, POSITION_T, NONE_T } object_t;


#define OBJ_HEAD	int refcount; 	\
					object_t type; 	\
					struct typeobject *typeobj

typedef struct object {
	OBJ_HEAD;
} Object;


#define TYPE_HEAD	char *name; \
					Object *(*alloc)(void); \
					void (*free)(Object *obj); \
					void (*print)(Object *obj); \
					Object *(*set)();  /* undefined argument to suppress compiler warnings */ \
					Object *(*vset)(Object *obj, va_list argp)

typedef struct typeobject {
	TYPE_HEAD;
} TypeObject;


#define TYPE(obj)		(((Object *)(obj))->type)
#define TYPEOBJ(obj)	(((Object *)(obj))->typeobj)
#define TYPENAME(obj)	(((Object *)(obj))->typeobj->name)

#define isFunction(obj)	(TYPE(obj) == POSITION_T)
#define isNumber(obj)	(TYPE(obj) == CHAR_T || TYPE(obj) == INT_T || TYPE(obj) == FLOAT_T)
#define isString(obj)	(TYPE(obj) == STR_T)
#define isList(obj)		(TYPE(obj) == LIST_T)
#define isSequence(obj)	(TYPE(obj) == LIST_T || TYPE(obj) == STR_T)
#define isListNode(obj)	(TYPE(obj) == LISTNODE_T)

#define obj_from_listnode(o)	(((ListNode *)o)->obj)

#define obj_incref(obj)	(((Object *)(obj))->refcount++)

#define obj_decref(obj)	do { 										\
							if (--((Object *)(obj))->refcount <= 0)	\
								obj_free((Object *)obj); 			\
						} while (0)

#include "list.h"

extern Object *obj_alloc(object_t type);
extern Object *obj_create(object_t type, ...);
extern void obj_free(Object *obj);
extern Object *obj_scan(object_t objtype);
extern void	obj_print(Object *a);

extern void	obj_assign(Object *a, Object *b);
extern Object *obj_copy(Object *a);

extern Object *obj_add(Object *op1, Object *op2);
extern Object *obj_sub(Object *op1, Object *op2);
extern Object *obj_mult(Object *op1, Object *op2);
extern Object *obj_divs(Object *op1, Object *op2);
extern Object *obj_mod(Object *op1, Object *op2);
extern Object *obj_eql(Object *op1, Object *op2);

extern Object *obj_neq(Object *op1, Object *op2);
extern Object *obj_lss(Object *op1, Object *op2);
extern Object *obj_leq(Object *op1, Object *op2);
extern Object *obj_gtr(Object *op1, Object *op2);
extern Object *obj_geq(Object *op1, Object *op2);
extern Object *obj_or(Object *op1, Object *op2);
extern Object *obj_and(Object *op1, Object *op2);

extern Object *obj_negate(Object *op1);
extern Object *obj_invert(Object *op1);

extern Object *obj_item(Object *sequence, int index);
extern Object *obj_slice(Object *sequence, int start, int end);

extern char_t obj_as_char(Object *op1);
extern int_t obj_as_int(Object *op1);
extern float_t obj_as_float(Object *op1);
extern char *obj_as_str(Object *op1);
extern ListObject *obj_as_list(Object *op1);
extern int obj_as_bool(Object *a);

extern char_t str_to_char(char *s);
extern int_t str_to_int(char *s);
extern float_t str_to_float(char *s);

extern Object *obj_to_strobj(Object *obj);

#endif
