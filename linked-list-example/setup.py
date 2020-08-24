# (c) Ю.С. Макаров и Д.Ю. Митюра 

from distutils.core import setup, Extension

module1 = Extension(
	'Linked_List',
	sources = ['linked_list.c']
)

setup(
	name = 'linked_list',
	version = '1.1',
	description = 'Simple module',
	ext_modules= [module1]
)
