import time
import threading

def calc_square(numbers):
    print('calc square')
    for n in numbers:
        time.sleep(0.2)
        square_res.append(n * n)
        print(n * n)

def calc_cube(numbers):
    print('calc cube')
    for n in numbers:
        time.sleep(0.2)
        n = n * n * n
        print(n)

square_res = []

arr = list(range(50))

start = time.time()

t1 = threading.Thread(target=calc_square,   args=(arr,))
t2 = threading.Thread(target=calc_cube,     args=(arr,))

t1.start()
t2.start()

t1.join()
print('here')
t2.join()

# calc_square(arr)
# calc_cube(arr)

end = time.time()

print(f'Finished in {end - start} seconds')
print(f'arr: {arr}')
print(f'square_res: {square_res}')