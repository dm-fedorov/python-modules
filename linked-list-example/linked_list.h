/* (c) Ю.С. Макаров и Д.Ю. Митюра */

#include <Python.h>

typedef struct Node {
	PyObject_HEAD
int value;
struct Node *next;
} Node;

typedef struct linked_list{
	PyObject_HEAD
	Node *first;
	Node *cur;
} linked_list;

static Node* get_last(linked_list*);

static PyObject* show(linked_list*);

static PyObject* add_first(linked_list *self, PyObject *args, PyObject *kwds);

static PyObject* add_last(linked_list *self, PyObject *args, PyObject *kwds);

static PyObject* pop_first(linked_list *self);

static int linked_list_init(linked_list *self, PyObject *args, PyObject *kwds);
