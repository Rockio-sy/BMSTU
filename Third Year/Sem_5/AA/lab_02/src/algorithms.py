import random as rd
import time

def generateMatrix(n, k, m):
    a = [[rd.randint(-100, 100) for j in range(k)] for i in range(n)]  # (n x k)
    b = [[ rd.randint(-100, 100) for j in range(m)] for i in range(k)]  # (k x m)
    res = [[0 for j in range(m)] for i in range(n)]
    return a, b, res

def generateMatrixValues(n, k, m, val):
    a = [[val for j in range(0, k)] for i in range(0, n)]
    b = [[val for j in range(0, m)] for i in range(0, k)]
    res = [[0 for j in range(0, m)] for i in range(0, n)]
    return a, b, res


def printMatrix(mat):
    for row in mat:
        print(row)
    print()


def classicMatrixMultiplation(result, mat_a, mat_b, n, k, m):
    for i in range(n):  # Iterate over rows of mat_a
        for j in range(m):  # Iterate over columns of mat_b
            result[i][j] = sum(mat_a[i][l] * mat_b[l][j] for l in range(k))  # Sum the products for each element
    return result


def vinogradMatrixMultiplation(result, mat_a, mat_b, n, k, m):
    # Compute auxiliary arrays
    row_vec = [sum(mat_a[i][j << 1] * mat_a[i][(j << 1) + 1] for j in range(k >> 1)) for i in range(n)]
    col_vec = [sum(mat_b[j << 1][i] * mat_b[(j << 1) + 1][i] for j in range(k >> 1)) for i in range(m)]

    # Main multiplication loop
    for i in range(n):
        for j in range(m):
            result[i][j] = -row_vec[i] - col_vec[j]
            for l in range(k >> 1):  # Use binary shift for division by 2
                result[i][j] += (mat_a[i][(l << 1) + 1] + mat_b[l << 1][j]) * (mat_a[i][l << 1] + mat_b[(l << 1) + 1][j])

    # Handle the case when k is odd
    if k % 2 == 1:
        for i in range(n):
            for j in range(m):
                result[i][j] += mat_a[i][k - 1] * mat_b[k - 1][j]

    return result


def vinogradMatrixOptimized(result, mat_a, mat_b, n, k, m):

    row_vec = [0] * n
    col_vec = [0] * m

    for i in range(n):
        for j in range(0, k - (k % 2), 2):
            row_vec[i] += mat_a[i][j] * mat_a[i][j + 1]

    for i in range(m):
        for j in range(0, k - (k % 2), 2):
            col_vec[i] += mat_b[j][i] * mat_b[j + 1][i]


    k_mod = k - (k % 2)
    for i in range(n):
        for j in range(m):
            temp = -row_vec[i] - col_vec[j]
            for l in range(0, k_mod, 2):
                temp += (mat_a[i][l + 1] + mat_b[l][j]) * (mat_a[i][l] + mat_b[l + 1][j])
            result[i][j] = temp


    if k % 2 == 1:
        for i in range(n):
            for j in range(m):
                result[i][j] += mat_a[i][k - 1] * mat_b[k - 1][j]

    return result

