# import multiprocessing as mp
# import time
# import math

# res_a = []
# res_b = []
# res_c = []

# def make_calculation_one(numbers):
#     for number in numbers:
#         res_a.append(math.sqrt(number**3))

# def make_calculation_two(numbers):
#     for number in numbers:
#         res_b.append(math.sqrt(number**4))
        
# def make_calculation_three(numbers):
#     for number in numbers:
#         res_c.append(math.sqrt(number**5))

# if __name__ == '__main__':
#     number_list = list(range(5))

#     start = time.time()
#     make_calculation_one(number_list)
#     make_calculation_two(number_list)
#     make_calculation_three(number_list)
#     end = time.time()

#     print(end - start)

#     temp_a = res_a
#     temp_b = res_b
#     temp_c = res_c    

#     res_a = []
#     res_b = []
#     res_c = []

#     print(temp_a)
#     print(res_a)

#     print('with mp')

#     p1 = mp.Process(target=make_calculation_one, args=(number_list,))
#     p2 = mp.Process(target=make_calculation_two, args=(number_list,))
#     p3 = mp.Process(target=make_calculation_three, args=(number_list,))

#     start = time.time()
#     p1.start()
#     p2.start()
#     p3.start()
#     p1.join()
#     p2.join()
#     p3.join()
#     time.sleep(3)
#     end = time.time()
#     print(end-start)
#     print('temp_a')
#     print(temp_a)
#     print('res_a')
#     print(res_a)

#     print (temp_a == res_a)
#     print (temp_b == res_b)
#     print (temp_c == res_c)

################################lesson 2############################################
    
# import multiprocessing as mp
# import time
# import math
# import concurrent.futures

# start = time.perf_counter()

# def do_smth(seconds):
#     print(f'sleeping {seconds} second...')
#     time.sleep(seconds)
#     return f'Done sleeping {seconds}'

# p1 = mp.Process(target=do_smth)
# p2 = mp.Process(target=do_smth)
# p3 = mp.Process(target=do_smth)

# p1.start()
# p2.start()
# p3.start()

# p1.join()
# p2.join()
# p3.join()

# with concurrent.futures.ProcessPoolExecutor() as executor:
#     secs = [4,5,6,7]
#     results = [executor.submit(do_smth, sec) for sec in secs]
    
#     for f in concurrent.futures.as_completed(results):
#         print(f.result())

# print ('here')

# processes = []

# for _ in range(10):
#     p = mp.Process(target=do_smth, args=[5])
#     p.start()
#     processes.append(p)

# for process in processes:
#     process.join()

# finish = time.perf_counter()

# print(f'Finished in {round(finish-start, 2)} seconds(s)')

################################img processing############################################

import time
import concurrent.futures
from PIL import Image, ImageFilter

img_names = [
    'photo-1516117172878-fd2c41f4a759.jpg',
    'photo-1532009324734-20a7a5813719.jpg',
    'photo-1524429656589-6633a470097c.jpg',
    'photo-1530224264768-7ff8c1789d79.jpg',
    'photo-1564135624576-c5c88640f235.jpg',
    'photo-1541698444083-023c97d3f4b6.jpg',
    'photo-1522364723953-452d3431c267.jpg',
    'photo-1513938709626-033611b8cc03.jpg',
    'photo-1507143550189-fed454f93097.jpg',
    'photo-1493976040374-85c8e12f0c0e.jpg',
    'photo-1504198453319-5ce911bafcde.jpg',
    'photo-1530122037265-a5f1f91d3b99.jpg',
    'photo-1516972810927-80185027ca84.jpg',
    'photo-1550439062-609e1531270e.jpg',
    'photo-1549692520-acc6669e2f0c.jpg'
]

t1 = time.perf_counter()
size = (1200, 1200)

def process_image(img_name):
    img = Image.open(img_name)

    img = img.filter(ImageFilter.GaussianBlur(15))

    img.thumbnail(size)
    img.save(f'processed/{img_name}')
    print(f'{img_name} was processed...')

with concurrent.futures.ProcessPoolExecutor() as executor:
    executor.map(process_image, img_names)

t2 = time.perf_counter()
print(f'Finished in {t2-t1} seconds')