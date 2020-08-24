### Пример создания модуля на языке Си

Все действия в этом уроке производятся в `GNU Linux`, поэтому требуется предварительно ее установить.

Скачать `Debian` можно по [ссылке](https://yadi.sk/d/wAN3HIQcjSAxaQ) *3.5 Гб*.

- student, Qwerty12
- root, toor

Рассмотрим пример создания собственного `Python` модуля на языке `С`. Для этого нам понадобится создать файл на языке `С` (лежит по ссылке [`ownmod.c`](https://github.com/dm-fedorov/pm3sem/blob/master/simple_module/ownmod.c)), который является модулем.

Структуры данных модуля представлены в [документации](https://docs.python.org/3/c-api/structures.html).

Небольшой мануальчик для программистов:
- [Extending and Embedding the Python Interpreter](https://docs.python.org/3/extending/index.html)
- [Python/C API Reference Manual](https://docs.python.org/3/c-api/index.html)

Затем формируем файл [`setup.py`](https://github.com/dm-fedorov/pm3sem/blob/master/simple_module/setup.py).

Для создания модулей на языке `С` воспользуемся пакетом [`distutils`](https://docs.python.org/3.8/library/distutils.html), входящим в состав стандартной библиотеки `Python`.

Устанавливаем модуль по [этому мануалу](https://docs.python.org/3/install/), выполняем в командной строке:

```cmd
# python3 setup.py build
```

Выполняем в командной строке с правами администратора:

```cmd
# python3 setup.py install
```

Теперь можем запустить интерпретатор и обратиться к модулю:

```Python
>>> import ownmod
>>> ownmod.echo()
вывод из экспортированного кода!
>>>
```