'модуль содержит функции и данные в отдельном файле'
"объекты из модуля можно импортировать в другие модули"
"имя файла = имя модуля + .py"

# my_module.py
def foo():
    pass

# other .py
import my_module
my_module.foo()
