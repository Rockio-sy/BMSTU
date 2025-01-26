from algorythms import *


class TestDistance:
    def __init__(self, function, func_name, xchange):
        self.function = function
        self.func_name = func_name
        self.xchange = xchange

    def empty_test(self):
        print(f'testing empty for {self.func_name}')
        assert self.function("", "") == 0
        print('OK\n')

    def equal_test(self):
        print(f'testing equal for {self.func_name}')
        assert self.function("abc", "abc") == 0
        assert self.function("a", "a") == 0
        print('OK\n')

    def diff_test(self):
        print(f'testing different for {self.func_name}')
        assert self.function("a", "") == 1
        assert self.function("", "a") == 1
        assert self.function("b", "c") == 1
        assert self.function("bc", "b") == 1
        assert self.function("bc", "c") == 1
        assert self.function("bc", "ac") == 1
        assert self.function("ab", "cd") == 2
        assert self.function("abc", "cd") == 3
        assert self.function("lol", "lopi") == 2
        print('OK\n')

    def exchange_test(self):
        print(f'testing xchange for {self.func_name}')
        if not self.xchange:
            assert self.function("ac", "ca") == 2
            assert self.function("qac", "lca") == 3
            assert self.function("abc", "cba") == 2
        else:
            assert self.function("ac", "ca") == 1
            assert self.function("qac", "lca") == 2
            assert self.function("abc", "cba") == 2

        print('OK\n')

    def run(self):
        self.empty_test()
        self.equal_test()
        self.diff_test()
        self.exchange_test()


class TestTwoFunctions:
    n = 7
    lenght = 10
    dif = 2

    def __init__(self, f1, f1_name, f2, f2_name):
        self.f1 = f1
        self.f1_name = f1_name
        self.f2 = f2
        self.f2_name = f2_name

    def test_same_len(self):
        print(f'testing same_len for {self.f1_name} and {self.f2_name}')
        for i in range(self.n):
            str1 = random_string(self.lenght)
            str2 = random_string(self.lenght)
            print(f'with str1={str1} and str2={str2}')
            assert self.f1(str1, str2) == self.f2(str1, str2)

    def test_different_len(self):
        print(f'testing different_len for {self.f1_name} and {self.f2_name}')
        for i in range(TestTwoFunctions.n):
            str1 = random_string(self.lenght + self.dif)
            str2 = random_string(self.lenght - self.dif)
            print(f'with str1={str1} and str2={str2}')
            assert self.f1(str1, str2) == self.f2(str1, str2)
            print(f'with str1={str2} and str2={str1}')
            assert self.f1(str2, str1) == self.f2(str2, str1)

    def run(self):
        self.test_same_len()
        self.test_different_len()


if __name__ == '__main__':
    test = TestDistance(loven_dist_matrix_classic, 'loven_dist_matrix_classic', False)
    test.run()
    test = TestDistance(loven_dist_matrix_optimized, 'loven_dist_matrix_optimized', False)
    test.run()
    test = TestDistance(loven_dist_recursion_classic, 'loven_dist_recursion_classic', False)
    test.run()
    test = TestDistance(loven_dist_recursion_optimized, 'loven_dist_recursion_optimized', False)
    test.run()
    test = TestDistance(damerau_loven_dist_recursion, 'damerau_loven_dist_recursion', True)
    test.run()

    ###
    test = TestTwoFunctions(loven_dist_matrix_classic, 'loven_dist_matrix_classic',
                            loven_dist_matrix_optimized, 'loven_dist_matrix_optimized')
    test.run()
    test = TestTwoFunctions(loven_dist_matrix_classic, 'loven_dist_matrix_classic',
                            loven_dist_recursion_classic, 'loven_dist_recursion_classic')
    test.run()
    test = TestTwoFunctions(loven_dist_matrix_classic, 'loven_dist_matrix_classic',
                            loven_dist_recursion_optimized, 'loven_dist_recursion_optimized')
    test.run()

    ###
    test = TestTwoFunctions(loven_dist_matrix_optimized, 'loven_dist_matrix_optimized',
                            loven_dist_recursion_classic, 'loven_dist_recursion_classic')
    test.run()

    test = TestTwoFunctions(loven_dist_matrix_optimized, 'loven_dist_matrix_optimized',
                            loven_dist_recursion_optimized, 'loven_dist_recursion_optimized')
    test.run()

    ##
    test = TestTwoFunctions(loven_dist_recursion_classic, 'loven_dist_recursion_classic',
                            loven_dist_recursion_optimized, 'loven_dist_recursion_optimized')
    test.run()
