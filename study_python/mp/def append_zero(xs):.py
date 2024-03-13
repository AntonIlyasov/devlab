def append_zero(xs):
    xs.append(0)
    xs = 100

a = []
append_zero(a)
print(a)

def calc_square(numbers):
    print('calc square')
    for n in numbers:
        # time.sleep(0.2)
        print(n * n)
        square_res.append(n * n)

square_res = []
arr = list(range(5))

calc_square(arr)

print(square_res)