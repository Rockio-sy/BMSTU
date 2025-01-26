from algorithms import *

def matrixCheck(mat_a, mat_b):
    res = True
    for i in range(0, len(mat_a)):
        for j in range(0, len(mat_a[0])):
            if mat_a[i][j] != mat_b[i][j]:
                res = False
    return res

def caseTest(n, k, m, caseTest):
    print(caseTest)
    print('Parameters: ', n, k, m)
    a, b, res = generateMatrix(n, k, m)

    res_classic = classicMatrixMultiplation(res.copy(), a, b, n, k, m)
    vinogradResult = res.copy()
    res_vinograd_opt = res.copy()

    print('Multiply results: ')
    printMatrix(a)
    printMatrix(b)
    printMatrix(res_classic)
    printMatrix(vinogradResult)
    printMatrix(res_vinograd_opt)

    print("Checking the results: ")
    print(matrixCheck(res_classic, vinogradResult) == matrixCheck(res_classic, res_vinograd_opt) == matrixCheck(res_vinograd_opt, vinogradResult))
    print('===\n')

def testNullCase(n, k, m, case_name):
    print(case_name)
    print('Parameters: ', n, k, m)
    a = [[0 for j in range(0, k)] for i in range(0, n)]
    b = [[rd.randint(-100, 100) for j in range(0, m)] for i in range(0, k)]
    res = [[0 for j in range(0, m)] for i in range(0, n)]

    res_classic = classicMatrixMultiplation(res.copy(), a, b, n, k, m)
    res_vinograd = res.copy()
    res_vinograd_opt = res.copy()

    print('Multiply result: ')
    printMatrix(a)
    printMatrix(b)
    printMatrix(res_classic)
    printMatrix(res_vinograd)
    printMatrix(res_vinograd_opt)

    print("Checking the results: ")
    print(matrixCheck(res_classic, res_vinograd) == matrixCheck(res_classic, res_vinograd_opt) == matrixCheck(res_vinograd_opt, res_vinograd))
    print('===\n')

def testOnesCase(n, k, m, case_name):
    print(case_name)
    print('Parameters: ', n, k, m)
    a = [[rd.randint(-100, 100) for j in range(0, k)] for i in range(0, n)]
    b = [[0 for j in range(0, m)] for i in range(0, k)]
    for i in range(0, len(b)):
        b[i][i] = 1
    res = [[0 for j in range(0, m)] for i in range(0, n)]

    res_classic = classicMatrixMultiplation(res.copy(), a, b, n, k, m)
    vinogradResults = res.copy()
    vinoradOptimizedResults = res.copy()

    print('Multiply results: ')
    printMatrix(a)
    printMatrix(b)
    printMatrix(res_classic)
    printMatrix(vinogradResults)
    printMatrix(vinoradOptimizedResults)

    print("Checking the results: ")
    print(matrixCheck(res_classic, vinogradResults) == matrixCheck(res_classic, vinoradOptimizedResults) == matrixCheck(vinoradOptimizedResults, vinogradResults))
    print('===\n')


caseTest(3, 3, 4, 'Usual matrix')
caseTest(1, 3, 4, 'Checking the vectors')
caseTest(4, 3, 1, 'Checking thev ectors')
testNullCase(3, 3, 4, 'Checking null(zero) matrix')
testOnesCase(3, 3, 4, 'Checking ones matrix')
