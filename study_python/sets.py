'множества позволяют хранить некий набор данных и быстро отвечать на '
"запрос присутствует ли элемент "
"хранятся в порядке, не в котором их создаешь"

s = set() # create empty mnozhestvo
basket = {'apple', 'banana', 'pear', 'orange', 'pear'}
print(basket)   # вывод по одному разу всех элементов

print('orange' in basket) # true
print('' in basket) # false

s.add('element')    # если элемент был, то ничего не изменитя
s.remove('element') # если элемента нет, то ошибка
s.discard('element') # удаление с проверкой на наличие элемента
s.clear()
len(s) # число элементов в множестве