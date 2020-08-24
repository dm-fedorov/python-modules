/* (c) Ю.С. Макаров и Д.Ю. Митюра */

#include <Python.h>
#include "linked_list.h"

static Node* get_last(linked_list *self)
{
	self->cur=self->first;
	if (self->first==NULL)
	{
		return NULL;
	}
	while (self->cur->next)
	{
		self->cur=self->cur->next;
	}
	return self->cur;
}

static PyObject* show(linked_list *self)
{
	self->cur=self->first;
	while(self->cur){
		printf("%d ",self->cur->value);
		self->cur=self->cur->next;
	}
	free(self->cur);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* add_first(linked_list *self, PyObject *args, PyObject *kwds)
{
	int data;
	Node *node=(Node*)malloc(sizeof(Node));
	if (! PyArg_ParseTuple(args, "i", &data)) {
		return NULL;
	}
	node->next = self->first;
	node->value = data;
	self->first = node;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* add_last(linked_list *self, PyObject *args, PyObject *kwds)
{
	int data;
	Node *node=(Node*)malloc(sizeof(Node));
	Node *bufL=(Node*)malloc(sizeof(Node));
	if (! PyArg_ParseTuple(args, "i", &data)) {
		return NULL;
	}
	node=get_last(self);
	bufL->value=data;
	bufL->next=NULL;
	node->next=bufL;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* pop_first(linked_list *self)
{
	int val=0;
	Node *node=(Node*)malloc(sizeof(Node));
	val=self->first->value;
	self->first=self->first->next;
	return Py_BuildValue("i",val);
}

static PyMethodDef linked_list_methods[] = {
	{"get_last", (PyCFunction)get_last, METH_NOARGS,
	PyDoc_STR("pointer to the last element in linked list")},
	{"pop_first", (PyCFunction)pop_first, METH_NOARGS,
	PyDoc_STR("get value and delete the first element")},
	{"add_first", (PyCFunction)add_first, METH_VARARGS,
	PyDoc_STR("add first element")},
	{"add_last", (PyCFunction)add_last, METH_VARARGS,
	PyDoc_STR("add last element")},
	{"show", (PyCFunction)show, METH_NOARGS,
	PyDoc_STR("show all elements of linked list")},
	{NULL,	NULL},
};

static int linked_list_init(linked_list *self, PyObject *args, PyObject *kwds)
{
return 0;
}

static void
linked_list_dealloc(linked_list* self)
{
	Py_XDECREF(self->first);
	Py_XDECREF(self->cur);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyTypeObject linked_list_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"linked_list",
	sizeof(linked_list),
    	0,
	(destructor)linked_list_dealloc,
	0,                   	/* tp_print */
	0,                   	/* tp_getattr */
	0,                   	/* tp_setattr */
	0,                   	/* tp_reserved */
	0,                   	/* tp_repr */
	0,                   	/* tp_as_number */
	0,                	   /* tp_as_sequence */
	0,                   	/* tp_as_mapping */
	0,                   	/* tp_hash */
	0,                   	/* tp_call */
	0,                   	/* tp_str */
	0,                   	/* tp_getattro */
	0,                       /* tp_setattro */
	0,                   	/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,    	/* tp_flags */
	"List objects",       	/* tp_doc: test.Test.__doc__ */
	0,                   	/* tp_traverse */
	0,                       /* tp_clear */
	0,                   	/* tp_richcompare */
	0,                   	/* tp_weaklistoffset */
	0,                   	/* tp_iter */
	0,                   	/* tp_iternext */
	linked_list_methods,     	 /* tp_methods */
	0,                   	/* tp_members */
	0,                   	/* tp_getset */
	0,                   	/* tp_base */
	0,                   	/* tp_dict */
	0,                   	/* tp_descr_get */
	0,                   	/* tp_descr_set */
	0,                   	/* tp_dictoffset */
	(initproc)linked_list_init,   /* tp_init */
	0,                   	/* tp_alloc */
	0,                   	/* tp_new */
};


static PyModuleDef linked_list_module = {
	PyModuleDef_HEAD_INIT,
	"Linked_List", /* __name__ */
	"îäíîñâÿçíûé ñïèñîê", /* __doc__ */
	-1,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

PyMODINIT_FUNC PyInit_Linked_List(void)
{
	PyObject* m;

	linked_list_Type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&linked_list_Type) < 0)
    	return NULL;

	m = PyModule_Create(&linked_list_module);
	if (m == NULL)
    	return NULL;
	Py_INCREF(&linked_list_Type);
	PyModule_AddObject(m, "Create", (PyObject *)&linked_list_Type);
	return m;
}

