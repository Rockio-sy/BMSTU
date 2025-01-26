from algorithms import *
import time

iteration = 1
test_sizes = [10, 25, 50, 75, 100, 125, 150]

timeClassic = []
timeVinograd = []
timeVinogradOptimized = []

for test in test_sizes:
    classicTime = 0
    vinogradTime = 0
    vinogradOptimizedTime = 0
    a, b, res = generateMatrixValues(test, test, test, 5)
    for i in range(0, iteration):
        startTime = time.process_time()
        vinogradMatrixOptimized(res, a, b, test, test, test)
        vinogradOptimizedTime += time.process_time() - startTime

        startTime = time.process_time()
        vinogradMatrixMultiplation(res, a, b, test, test, test)
        vinogradTime += time.process_time() - startTime

        startTime = time.process_time()
        classicMatrixMultiplation(res, a, b, test, test, test)
        classicTime += time.process_time() - startTime
        
    classicTime = classicTime / iteration
    vinogradTime = vinogradTime / iteration
    vinogradOptimizedTime = vinogradOptimizedTime / iteration
    timeClassic.append(classicTime)
    timeVinograd.append(vinogradTime)
    timeVinogradOptimized.append(vinogradOptimizedTime)

print(timeClassic, timeVinograd, timeVinogradOptimized)
