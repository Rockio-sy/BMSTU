from algorythms import *
import pandas as pd
import matplotlib.pyplot as plt
import re
from time import process_time

funcs = [[loven_dist_matrix_classic, 'loven_dist_matrix_classic'],
         [loven_dist_matrix_optimized, 'loven_dist_matrix_optimized'],
         [loven_dist_recursion_optimized, 'loven_dist_recursion_optimized'],
         [loven_dist_recursion_classic, 'loven_dist_recursion_classic'],
         [damerau_loven_dist_recursion, 'damerau_loven_dist_recursion']]

strings = [[length, random_string(length), random_string(length)] for length in range(0, 11, 1)] + \
          [[length, random_string(length), random_string(length)] for length in range(20, 201, 10)]

n_repeats = 15
stop_recursive = 10

path = '/home/rockio/github/mine/AA/lab1/report/inc/img/'

time_file = path + 'time.xlsx'
filename_all = path + 'time_all.png'
filename_with = path + 'time_with_matrix.png'
filename_without = path + 'time_rec_without_cash.png'

time_dl_file = path + 'time_dl.xlsx'
filename_dl = path + 'time_dl.png'


def compare_dl():
    df_time = pd.DataFrame({'StringLength': list(range(0, 12, 2))})

    for func, func_name in [[damerau_loven_dist_recursion, 'damerau_loven_dist_recursion'],
                            [loven_dist_recursion_classic, 'loven_dist_recursion_classic']]:
        print(func_name)
        time_measures = []
        two_letters = 'ab'
        two_letters_rev = 'ba'

        for string_length in range(6):
            string1 = two_letters * string_length
            string2 = two_letters_rev * string_length
            start = process_time()
            for i in range(n_repeats):
                answer = func(string1, string2)
            end = process_time()
            time = (end - start) / n_repeats
            time_measures.append(time)
            print(string_length * 2, time)

        df_time[func_name] = time_measures
        print(time_measures)

    df_time.to_excel(time_dl_file)


def compare():
    compare_dl()
    df_time = pd.DataFrame({'StringLength': [x[0] for x in strings]})

    for func, func_name in funcs:
        print(func_name)
        time_measures = []
        for string_length, string1, string2 in strings:
            if re.search('recursion', func_name) and \
                    not re.search('optimized', func_name) and \
                    string_length > stop_recursive:
                time_measures.append(None)
            else:
                start = process_time()
                for i in range(n_repeats):
                    answer = func(string1, string2)
                end = process_time()
                time = (end - start) / n_repeats
                time_measures.append(time)
                print(string_length, time)

        df_time[func_name] = time_measures
        print(time_measures)

    df_time.to_excel(time_file)

    plt.xlim([0, 10])


def draw_plot_all():
    df = pd.read_excel(time_file)

    plt.xlabel('String length')
    plt.ylabel('Taken work time')

    plt.plot(df['StringLength'], df['loven_dist_matrix_classic'], '-.', label='Левенштейн, итерационный')
    plt.plot(df['StringLength'], df['loven_dist_recursion_classic'], '*', label='Левенштейн, рекурсивный без кеша')
    plt.plot(df['StringLength'], df['damerau_loven_dist_recursion'], '--',
             label='Дамерау-Левенштейн, рекурсивный без кеша')
    plt.plot(df['StringLength'], df['loven_dist_recursion_optimized'], ':',
             label='Левенштейн, рекурсивный с кешем')
    plt.legend(loc='best')
    plt.savefig(filename_all)
    plt.gcf().clear()


def draw_plot_rec_without_cash():
    df = pd.read_excel(time_file)
    plt.grid()
    plt.xlabel('String length')
    plt.ylabel('taken work time')
    plt.xlim([0, 10])

    plt.plot(df['StringLength'], df['loven_dist_recursion_classic'], '*', label='Левенштейн, рекурсивный без кеша')
    plt.plot(df['StringLength'], df['damerau_loven_dist_recursion'], '--',
             label='Дамерау-Левенштейн, рекурсивный без кеша')

    plt.legend(loc='best')
    plt.savefig(filename_without)
    plt.gcf().clear()


def draw_plot_with_matrix():
    df = pd.read_excel(time_file)
    plt.grid()
    plt.xlabel('String length')
    plt.ylabel('taken work time')

    plt.plot(df['StringLength'], df['loven_dist_matrix_classic'], '-.', label='Левенштейн, итерационный')
    plt.plot(df['StringLength'], df['loven_dist_recursion_optimized'], ':',
             label='Левенштейн, рекурсивный с кешем')
    plt.legend(loc='best')
    plt.savefig(filename_with)
    plt.gcf().clear()


def draw_plot_dl():
    df = pd.read_excel(time_dl_file)
    plt.grid()
    plt.xlabel('String length')
    plt.ylabel('taken work time')

    plt.plot(df['StringLength'], df['loven_dist_recursion_classic'], '*', label='Левенштейн, рекурсивный без кеша')
    plt.plot(df['StringLength'], df['damerau_loven_dist_recursion'], '--',
             label='Дамерау-Левенштейн, рекурсивный без кеша')

    plt.legend(loc='best')
    plt.savefig(filename_dl)
    plt.gcf().clear()


def draw_plots():
    draw_plot_dl()
    draw_plot_rec_without_cash()
    draw_plot_all()
    draw_plot_with_matrix()


if __name__ == '__main__':
    compare()
    draw_plots()
