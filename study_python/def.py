def min2(a, b):
    if a <= b:
        return a
    else:
        return b
    
m = min2(42, 30)
print(m)

'произвольное число параметров min(5), min([5,3,1])'
def min(*a):
    min = a[0]
    for i in a:
        if min > i:
            min = i
    return min

mm = min(1,2,3, -100)
print(mm)
#область видимости
def init_val():
    a = 100

a = 0
init_val()
print(a) # a = 0
#область видимости
def init_val(a):
    a = 100
b = 0
init_val(b)
print(b) # b = 0
#область видимости
def append_zero(xs):
    xs.append(0)
    xs = 100

a = []
append_zero(a)
print(a)
# глобальные переменные
def print_value():
    print(a)

print_value()

def f(x):
    if x <= -2:
        return 1 - (x + 2)*(x + 2)
    if -2 < x <= 2:
        return -x / 2
    else:
        return (x - 2)*(x - 2) + 1
    

def modify_list(a):
    


lst = [1, 2, 3, 4, 5, 6]
