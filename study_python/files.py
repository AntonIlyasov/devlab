"файловый ввод вывод"

inf = open('file.txt', 'r')
s1 = inf.readline()
s2 = inf.readline()
inf.close()

with open('file.txt') as inf:
    s1 = inf.readline()
    s2 = inf.readline()

# file.txt уже закрыт
    
s = inf.readline().strip() # '\t abc \n' -> 'abc' убирает все служебные символы в строке

# import os
# os.path.join('.','dirname','filename.txt') # './dirname/filename.txt'

# чтение всех строк из файла
with open('input.txt') as inf:
    for line in inf:
        line = line.strip()
        print(line)

# запись в файл
ouf = open('file.txt','w')
ouf.write('Some text\n')
ouf.write(str(25))
ouf.close()

with open('file.txt') as ouf:
    ouf.write('Some text\n')
    ouf.write(str(25))

# file.txt уже закрыт