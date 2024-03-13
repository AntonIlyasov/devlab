import time
import multiprocessing as mp

def calc_square(numbers, square_res, value):
    value.value = 5.67
    print('calc square')
    for idx, n in enumerate(numbers):
        # time.sleep(0.2)
        square_res[idx] = n * n

def calc_cube(numbers):
    print('calc cube')
    for n in numbers:
        # time.sleep(0.2)
        n = n * n * n
        print(n)    

square_res = mp.Array('i', 5)
value = mp.Value('d', 0.0)
arr = list(range(5))

start = time.time()

p1 = mp.Process(target=calc_square, args=(arr, square_res, value))
p2 = mp.Process(target=calc_cube, args=(arr,))

p1.start()
p2.start()

p1.join()
p2.join()

# calc_square(arr)
# calc_cube(arr)

end = time.time()

print(f'Finished in {end - start} seconds')
print(f'arr: {arr}')
print(f'square_res: {square_res[:]}')
print(f'value: {value.value}')