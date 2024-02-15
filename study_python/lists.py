students = ['s2','s1','s3']
teachers = ['t1','t2']
students_ordered = sorted(students)
print(students_ordered)
empty_list = []
for student in students:
    print('hello ' + student)

print(len(students))
print(students[:2])
print(students[::-1])
students += ['s4']
print(students + teachers)

a = 5
a = 3
print(a)
if 's2' in students:
    students.remove('s2')
ind = students.index('s3')
print(students)
del students[ind]
print(students)

# input_list = [int(i) for i in input('input digits\n').split()]      #чтение списка из ввода
# print(input_list)

# двумерные списки
a = [[1,2,3],[4,5,6],[7,8,9]]
print(a)
aa = [[0 for i in range(5)] for j in range(5)]
aa[2][2] = 5
print(aa)