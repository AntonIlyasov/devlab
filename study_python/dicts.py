'словарь позволяет хранить множество пар ключ-значение'
"и по ключу получать значение"

dictAuto1 = dict() # empty dict
dictAuto1 = {'a':239, 10:48}
print(dictAuto1['a'])
print(dictAuto1[10])

# operations with dict
'a' in dictAuto1 # true
'a' not in dictAuto1 # false
dictAuto1['a'] = 145
print(dictAuto1['a'])        # ошибка, если ключа нет
print(dictAuto1.get('ss'))   # none,   если ключа нет
del dictAuto1['a']
print(dictAuto1)
dictAuto1.clear()
# словари - изменяемы 
# элементы не имеют порядка
# все ключи различны
# ключи неизменяемы ключами могут быть числа строки, но не списки и не другие словари!

# перебор элементов словаря

d = {'C':14, 'A':12, 'T':9, 'G':18}
for key in d:
    print(key, end=' ') # C A T G
print()

for key in d.keys(): # d.keys() возвращает множество ключей
    print(key, end=' ') # C A T G
print()

for value in d.values(): # d.values() возвращает множество values
    print(value, end=' ') # 14 12 9 18
print()

for key, value in d.items(): # d.items() возвращает множество values
    print(key, value, end='; ') # C 14; A 12; T 9; G 18;
print()

# можно для каждого ключа хранить несколько значений
# для этого можем ключам сопостовлять не значение, а список из значений


dictAuto = dict()
dictAuto['number']  = 'X245FG'
dictAuto['year']    = 2018
dictAuto['owners']  = ['Petrov', 'Ivanov', 'Sidorov']
print(dictAuto)

