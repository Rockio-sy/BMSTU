from algorythms import *
import time

n = 100000
str1 = 'tikokat'
str2 = 'sopl1234'


def main_function1():
    start = time.time()
    for i in range(n):
        loven_dist_matrix_classic(str1, str2)
    end = time.time()
    print(f'time:{end - start}')

    start = time.thread_time()
    for i in range(n):
        loven_dist_matrix_classic(str1, str2)
    end = time.thread_time()
    print(f'thread_time:{end - start}')

    start = time.process_time()
    for i in range(n):
        loven_dist_matrix_classic(str1, str2)
    end = time.process_time()
    print(f'process_time():{end - start}')


def main_function2():
    start = time.thread_time()
    for i in range(n):
        loven_dist_matrix_classic(str1, str2)
    end = time.thread_time()
    print(f'thread_time:{end - start}')

    start = time.process_time()
    for i in range(n):
        loven_dist_matrix_classic(str1, str2)
    end = time.process_time()
    print(f'process_time():{end - start}')

    start = time.time()
    for i in range(n):
        loven_dist_matrix_classic(str1, str2)
    end = time.time()
    print(f'time:{end - start}')


def main_function3():
    start = time.process_time()
    for i in range(n):
        loven_dist_matrix_classic(str1, str2)
    end = time.process_time()
    print(f'process_time():{end - start}')

    start = time.time()
    for i in range(n):
        loven_dist_matrix_classic(str1, str2)
    end = time.time()
    print(f'time:{end - start}')

    start = time.thread_time()
    for i in range(n):
        loven_dist_matrix_classic(str1, str2)
    end = time.thread_time()
    print(f'thread_time:{end - start}')


if __name__ == '__main__':
    main_function1()
    print()
    main_function2()
    print()
    main_function3()
