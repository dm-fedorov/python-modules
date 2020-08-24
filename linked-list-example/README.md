
*(c) Ю.С. Макаров и Д.Ю. Митюра*

# Пример лабораторной работы по разработке модуля

Постановка задачи лабораторной работы: научиться писать модули для языка программирования Python на языке С, используя CPython C API.

Теория, которая поможет при выполнении лабораторной работы [ссылка 1](https://docs.python.org/3.8/extending/index.html#extending-index), [ссылка 2](https://docs.python.org/3.8/c-api/)

Документация в отдельном pdf-файле [The Python/C API. Release 3.6.0. Guido van Rossum and the Python development team](https://scicomp.ethz.ch/public/manual/Python/3.6.0/c-api.pdf).
	
### Введение

В этой лабораторной работе Вы научитесь писать расширения для Python на языке программирования С. Зачем это нужно?
	
- Вам нужна скорость и вы знаете, что C быстрее Python.
- Вам нужна конкретная C-библиотека и вы не хотите писать "велосипед" на Python.
- Вам нужен низкоуровневый интерфейс управления ресурсами для работы с памятью и файлами.
- Вы хотите попрактиковаться в совмещении нескольких языков программирования в одном проекте.

**Факт №1**

На самом деле, Python’ов много. Вернее, так: Python - это просто спецификация того, как должен выглядеть язык программирования. Реализовывать эту спецификацию можно на любом другом языке программирования: C, Java, C#, Ruby и т.д.
В этой лабораторной работе используется `CPython`. `CPython` - это реализация Python на языке С. Она чаще всего используется разработчиками (примерно в 90% случаев), и именно её вы использовали в первом семестре во время изучения языка Python. Преимущество `CPython` в том, что он поддерживается создателями Python, и именно его вы скачиваете с сайта python.org.

`CPython` предоставляет API (application programming interface) для разработчиков, желающих написать свой модуль на С.
Стоит заметить, что существуют реализации Python на Java (Jython), C# (IronPython). Зачем они нужны?
Если коротко, то как раз так для того, чтобы совмещать несколько языков в одном проекте, и ускорять программы. [Статья на эту тему](https://www.toptal.com/python/%D0%BF%D0%BE%D1%87%D0%B5%D0%BC%D1%83-%D1%81%D1%83%D1%89%D0%B5%D1%81%D1%82%D0%B2%D1%83%D0%B5%D1%82-%D1%82%D0%B0%D0%BA-%D0%BC%D0%BD%D0%BE%D0%B3%D0%BE-%D0%BF%D0%B8%D1%82%D0%BE%D0%BD%D0%BE%D0%B2/ru).

**Факт №2**

Существует много способов ускорить и расширить Python помимо CPython C API:
- Cython - Python с элементами строгой типизации. Это позволяет писать код быстрее, потому что вы пишете на том же самом Python, но не углубляетесь в C. 	
- SWIG - в этом случае вы пишете код на С (в отдельных С-файлах), который после компиляции и некоторых манипуляций можно вызывать в Python.
- cffi - является просто Python модулем. Импортируем его в Python-файл, затем внутри него пишем код на чистом С. Компилируем, и вызываем его уже в другом Python-файле.
- Numba - по факту, является библиотекой, которую мы просто импортируем в Python, и всюду используем волшебные декораторы из нее. Использует внутри себя [LLVM](https://www.astrosoft.ru/articles/embedded-software/arkhitektura-prilozheniy-s-otkrytym-iskhodnym-kodom-llvm/). Она совместима со стеком Python-библиотек, используемых для научных расчетов (scipy, numpy, и др.).

**Почему мы будем использовать CPython API?**

- Он поддерживается официальными разработчиками.		
- Вы будете использовать объекты, которыми оперирует непосредственно интерпретатор.
- У вас появится глубокое понимание внутреннего устройства интерпретатора и реализации питоновских типов изнутри. 		
По факту, вы можете не знать, как устроен ваш модуль. Его может эффективно написать С-программист, знающий C API.
Однако, стоит отметить следующее: в официальной документации разработчики CPython говорят, что альтернативные пути быстрее по скорости разработки, чем классический CPython C API.

**Работа с CPython C API**

Начнем сразу же с примера. В нем реализован модуль, который содержит только одну функцию. Она складывает 2 числа, которые передаются ей в качестве аргументов.
[Ссылка на github](https://github.com/dm-fedorov/python-and-c-integration/tree/master/Functions-module/plus-function)

```C
#include <Python.h>

static PyObject *py_plus(PyObject *self, PyObject *args){
  double x, y, result;

  if(!PyArg_ParseTuple(args,"dd", &x, &y))
    return NULL;
  
  result = x + y;
  
  return Py_BuildValue("d", result);
}

/* Описывает методы модуля */
static PyMethodDef ownmod_methods[] = {
{ 
    "plus",          // название функции внутри python
     py_plus,        // функция C
     METH_VARARGS,   // макрос, поясняющий, что функция у нас с аргументами
     "plus function" // документация для функции внутри python
},
{ NULL, NULL, 0, NULL } 
// так называемый sentiel. Сколько бы элементов структуры 
// у вас не было, этот нулевой элемент должен быть всегда, и при этом быть последним
};

/* Описываем наш модуль */
static PyModuleDef simple_module = {    
    PyModuleDef_HEAD_INIT,   // обязательный макрос
    "my_plus",               // my_plus.__name__
    "amazing documentation", // my_plus.__doc__
    -1,
    ownmod_methods           // сюда передаем методы модуля
};

// в названии функции обязательно должен быть префикс PyInit
PyMODINIT_FUNC PyInit_my_plus(void) {
      PyObject* m;
      // создаем модуль
      m = PyModule_Create(&simple_module);
      // если все корректно, то эта проверка не проходит
      if (m == NULL)
          return NULL;
      return m;
}
```

**Объяснение примера:**
Будем построчно анализировать код.

В первой строке импортируется заголовочный файл `Python.h`. Он содержит `API CPython`. Его необходимо подключать первым, перед другими модулями, т.к. он содержит в себе директивы препроцессора, которые могут оказать влияние на стандартные заголовочные файлы.

Далее объявляется функция, складывающая два числа. Стоит обратить внимание на тип возвращаемого значения, а также аргументы функции. [`PyObject`](https://docs.python.org/3/c-api/structures.html#c.PyObject) - это С-структура, являющаяся базовой для всех остальных объектов. Все функции в вашем модуле, должны иметь данный тип возвращаемого значения.

см. подробнее про [CPython data structures](https://spyhce.com/blog/cpython-data-structures)

Теперь об аргументах: `self` и `args` - это обязательные аргументы функции.

`self` - указатель на объект самого модуля, если данная функция принадлежит модулю, или указатель на объект, если это функция какого-либо класса.

`args` - это кортеж (`tuple`) аргументов функции, переданный прямиком Python. Например, если вызвать `simple.plus(1, 3)`, то в этом случае `args` будет состоять из чисел `(1, 3)`.

Перейдем к телу функции: объявляются три переменные, которые будут в дальнейшем использованы в функции `PyArg_ParseTuple`. Остановимся на ней подробнее. Она используется для обработки аргументов функции, и позволяет трансформировать типы данных Python в типы данных C. В качестве первого аргумента она принимает `PyObject`, который, по сути, является кортежем переданных функции аргументов, вторым - типы переменных, в которые мы хотим преобразовать наши аргументы. Далее идут переменные, в которые мы сохраняем преобразованные значения. Проверка в строке 6 нужна для того, чтобы проконтролировать, корректно ли отработала функция, и если нет, то сообщить об ошибке.

Несколько примеров использования этой функции:

```C		
    PyArg_ParseTuple(args, "s", &some_string) 
    // проверить, содержит ли массив args один элемент строкового типа, 
    // если да – сохранить это значение в переменную some_string

    PyArg_ParseTuple(args, "i", &some_int) // аналогично для числа
```

Далее мы вычисляем значение переменной `result` и используем функцию `Py_BuildValue`, которая позволяет конвертировать переменные С в переменные соответствующих Python-типов. Примеры работы данной функции:

```C
        Py_BuildValue("")                        None
	Py_BuildValue("i", 123)                  123
	Py_BuildValue("iii", 123, 456, 789)      (123, 456, 789)
	Py_BuildValue("s", "hello")              'hello'
	Py_BuildValue("y", "hello")              b'hello'
	Py_BuildValue("ss", "hello", "world")    ('hello', 'world')
```

Теперь переходим к пояснению массива структур, описывающего методы модуля. 
Такой массив должен обязательно быть у каждого модуля.
```C
static PyMethodDef ownmod_methods[] = {
  {
    "plus", // название функции внутри python
    py_plus, // функция C
    METH_VARARGS, // макрос, поясняющий, что функция у нас с аргументами
    "plus function" // документация для функции внутри python
  },
  { NULL, NULL, 0, NULL } 
  // так называемый sentiel. Сколько бы элементов структуры у вас не было,
  // этот нулевой элемент должен быть всегда, и при этом быть последним.
};
```

Рассмотрим теперь структуру, которая описывает непосредственно сам модуль:
```C
/* Описываем наш модуль */
static PyModuleDef simple_module = {    
    PyModuleDef_HEAD_INIT, // обязательный макрос
    "my_plus",               // my_plus.__name__
    "amazing documentation", // my_plus.__doc__
    -1,
    ownmod_methods           // сюда передаем массив с методами модуля
};
```

И, наконец, функция, в которой мы создаем модуль:

```C
// в названии функции обязательно должен быть префикс PyInit
PyMODINIT_FUNC PyInit_my_plus(void)
{
    PyObject* m;
    // создаем модуль
    m = PyModule_Create(&simple_module);
    // если все корректно, то эта проверка не проходит
    if(m == NULL)
        return NULL;

    return m;
}
```

***Превращаем код на С в Python модуль***
Для этого нам необходимо создать файл `setup.py` в той же директории, где и лежит наш модуль, написанный на C. 

Файл должен иметь следующее содержание:

```Python
from distutils.core import setup, Extension

module1 = Extension(
	'my_plus', # название модуля внутри Python
	 sources = ['plus.c'] # Исходные файлы модуля
)

setup(
	name = 'my_plus',              # название модуля (my_plus.__name__)
	version = '1.1',               # версия модуля
	description = 'Simple module', # описание модуля
	ext_modules= [module1]         # объекты типа Extension (мы объявили его выше)
)
```

Далее, нам необходимо последовательно запустить несколько команд в терминале (консоли):
```shell
sudo python3 setup.py build
sudo python3 setup.py install
```

После этого вы можете запустить Python, импортировать написанную нами библиотеку, и вызвать функцию, которую мы написали:

```Python
import my_plus
my_plus.plus(1,100)
>>> 101.0
```

Для запуска в ОС Windows:
- Запустить консоль
- Перейти в каталог с проектом
- python setup.py install

если будет ошибка: *Unable to find `vcvarsall.bat`*, это значит, что у вас нет нужной версии `visual studio`, нужную версию можно увидеть при запуске Python (правый столбик)

- Visual C++ 4.x                 MSC_VER=1000
- Visual C++ 5                   MSC_VER=1100
- Visual C++ 6                   MSC_VER=1200
- Visual C++ .NET                MSC_VER=1300
- Visual C++ .NET 2003           MSC_VER=1310
- Visual C++ 2005  (8.0)         MSC_VER=1400
- Visual C++ 2008  (9.0)         MSC_VER=1500
- Visual C++ 2010 (10.0)         MSC_VER=1600
- Visual C++ 2012 (11.0)         MSC_VER=1700
- Visual C++ 2013 (12.0)         MSC_VER=1800
- Visual C++ 2015 (14.0)         MSC_VER=1900

### Создаем свой модуль с классами (структурами)

Как вы уже знаете, Python является объектно-ориентированным языком, а С - нет. Однако, это не мешает писать модули, содержащие классы, потому что в С есть замечательная конструкция: структура. Если вы уже программировали на языке С++, то знаете, что отличие между структурой и классом не такое уж и большое. Поэтому, благодаря структурам, мы можем писать модули, содержащие классы.

Рассмотрим написание такого модуля на примере реализации односвязного списка.

Модуль будет состоять из двух файлов: `linked_list.h`, `linked_list.c`.

Ниже приведены листинги кода этих файлов, с комментариями, поясняющими содержимое.

**linked_list.h:**
```C
/* 
linked_list.h - заголовочный файл, здесь объявлены структуры и методы класса 
*/
#include <Python.h>

// элемент списка
typedef struct Node {
        /* 
	   PyObject_HEAD - макрос, общий для всех встроенных объектов, добавляет 
           счетчик количества указателей на объект и указатель на родительский тип объекта
           раскрывается в PyObject ob_base 
	 */
	PyObject_HEAD
        int value;        // значение хранящееся в Node
        struct Node *next; // указатель на следующий элемент
} Node;

// сам список
typedef struct linked_list{
     PyObject_HEAD
     Node *first;   // указатель на первый элемент списка
     Node *cur;     // указатель на буферный элемент
} linked_list;

// возвращает последний элемент списка
static Node* get_last(linked_list*);

// выводит на экран все элементы списка
static PyObject* show(linked_list*);

// добавляет элемент в начало списка
static PyObject* add_first(linked_list *self, PyObject *args, PyObject *kwds);

// добавляет элемент в конец списка
static PyObject* add_last(linked_list *self, PyObject *args, PyObject *kwds);

// удаляет первый элемент и возвращает его значение
static PyObject* pop_first(linked_list *self);

// конструктор
static int linked_list_init(linked_list *self, PyObject *args, PyObject *kwds);
```

**linked_list.c:**

```C
#include <Python.h>
#include "linked_list.h"

static Node* get_last(linked_list *self)
{
    self->cur=self->first;
    if(self->first==NULL)
        return NULL;
	
    while(self->cur->next)
        self->cur=self->cur->next;
    
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

    /* 
    Отсутствие Py_INCREF, может привести к Fatal Python error: deallocating None.
    return Py_None – объект None.
    */

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* add_first(linked_list *self, PyObject *args, PyObject *kwds)
{
    int data;
    Node *node=(Node*)malloc(sizeof(Node));
    
    if(!PyArg_ParseTuple(args, "i", &data))
        return NULL;
    
    node->next = self->first;
    node->value = data;
    self->first = node;
    Py_INCREF(Py_None);
    return Py_None;
}

/*
Все методы, к которым можно обратиться напрямую, должны возвращать PyObject*. 
Например, к методу get_last нельзя обратиться напрямую 
(x.show() – можно, x.get_last() – ошибка).
*/

static PyObject* add_last(linked_list *self, PyObject *args, PyObject *kwds)
{
    int data;
    Node *node=(Node*)malloc(sizeof(Node));
    Node *bufL=(Node*)malloc(sizeof(Node));
   
    if(!PyArg_ParseTuple(args, "i", &data))
        return NULL;
    
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
    // значения возвращаются, только через Py_BuildValue()
    return Py_BuildValue("i", val);
}

/* METH_NOARGS – метод без аргументов, METH_VARARGS – метод с аргументами */
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
    {NULL, NULL},
};

/* в данном примере конструктор не несет практической пользы, он здесь только как пример создания его */
static int linked_list_init(linked_list *self, PyObject *args, PyObject *kwds)
{
    return 0;
}

// деструктор, здесь мы подчищаем память за собой
static void
linked_list_dealloc(linked_list* self)
{
    Py_XDECREF(self->first);
    Py_XDECREF(self->cur);
    Py_TYPE(self)->tp_free((PyObject*)self);
}
 
static PyTypeObject linked_list_Type = {
     /* Everything about object */
     PyVarObject_HEAD_INIT(NULL, 0)
     //tp_name – имя типа
     "linked_list",         	/* tp_name */
     //tp_basicsize – размер объекта, если размер типа не изменяем
      sizeof(linked_list), /* tp_basicsize */
      /*tp_itemsize – если размер типа изменяем, 
      то размер объекта = tp_basicsize + N* tp_itemsize, где N – “длина” объекта.
      */
    	0,                   	/* tp_itemsize */
//tp_dealloc – деструктор
	(destructor)linked_list_dealloc,                   	/* tp_dealloc */
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
        //tp_doc -строка документации
	"List objects",       	/* tp_doc: test.Test.__doc__ */
	0,                   	/* tp_traverse */
	0,                       /* tp_clear */
	0,                   	/* tp_richcompare */
	0,                   	/* tp_weaklistoffset */
	0,                   	/* tp_iter */
	0,                   	/* tp_iternext */
       //tp_methods – указатель на структуру с методами класса
	linked_list_methods,     	 /* tp_methods */
	0,                   	/* tp_members */
	0,                   	/* tp_getset */
	0,                   	/* tp_base */
	0,                   	/* tp_dict */
	0,                   	/* tp_descr_get */
	0,                   	/* tp_descr_set */
	0,                   	/* tp_dictoffset */
       //tp_init – указатель на конструктор
	(initproc)linked_list_init,   /* tp_init */
	0,                   	/* tp_alloc */
	0,                   	/* tp_new */
};

 
static PyModuleDef linked_list_module = {
	/* информация о модуле */
	PyModuleDef_HEAD_INIT,
	"Linked_List", /* __name__ */
	"односвязный список", /* __doc__ */
	-1, /*все методы здесь*/
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
	if(PyType_Ready(&linked_list_Type) < 0)
    	    return NULL;
 
	m = PyModule_Create(&linked_list_module);
	if(m == NULL)
    	    return NULL;
	
	Py_INCREF(&linked_list_Type);
	// x=Linked_List.Create() - так создается объект х класса Linked_List 
	PyModule_AddObject(m, "Create", (PyObject *)&linked_list_Type);
	
	return m;
}
```

Пример работы модуля:

```Python
>>> import Linked_List
>>> a=Linked_List.Create()
>>> a.add_first(2)
>>> a.add_first(4)
>>> a.add_first(3)
>>> a.add_last(22)
>>> a
<Linked_List objectat 0x00E35B90>
>>> a.show()
3 4 2 22
>>> b=a.pop_first()
>>> b
3
>>> a.show()
4 2 22
```
