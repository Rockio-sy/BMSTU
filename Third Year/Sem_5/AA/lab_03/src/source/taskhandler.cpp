#include "taskhandler.h"

void TaskHandler::run(void)
{
    bool running = true;
    while (running)
    {
        displayMenu();
        int choice = getUserChoice();
        switch(choice)
        {
            case 1:
                executeAlgorithmChoice();
                break;
            case 2:
                runEfficiencyAnalyzer();
                break;
            case 3:
                running = false;
                break;
            default:
                std::cout << "Невалидная опция." << std::endl;
                break;
        }
    }
}

void TaskHandler::executeAlgorithmChoice(void)
{
    std::cout << "Выберите алгоритм:" << std::endl;
    std::cout << "1 - Полный перебор" << std::endl;
    std::cout << "2 - Бинарный поиск" << std::endl;
    int algChoice = getUserChoice();
    std::pair<int, int> (Finder::*algorithm)(const std::vector<int> &, int) = nullptr;
    switch (algChoice)
    {
        case 1:
            algorithm = &Finder::bruteForce;
            break;
        case 2:
            algorithm = &Finder::binarySearch;
            break;
        default:
            std::cout << "Невалидная опция.." << std::endl;
            return;
    }
    executeAlgorithm(algorithm);
}

void TaskHandler::executeAlgorithm(std::pair<int, int> (Finder::*algorithm)(const std::vector<int> &, int))
{
    (void) algorithm;
    int n;
    int x;
    std::cout << "Введите размер исходного массива: ";
    std::cin >> n;
    std::vector<int> src(n, 0);
    sourceInput(src, n);

    std::cout << "Исходный массив: ";
    sourcePrint(src, n);

    std::cout << "Введите искомого элемента: ";
    std::cin >> x;

    // std::cout << "Element to find: " << x << std::endl;
    std::pair<int, int> result = (f.*algorithm)(src, x);
    if (result.first == -1)
    {
        std::cout << "Элемент не найден, количество сравнений: " << result.second << std::endl;
    }
    else
    {
        std::cout << "Индекс: " << result.first << ",количество сравнений: " << result.second << std::endl;
    }
}

void TaskHandler::displayMenu(void)
{
    std::cout << std::endl;
    std::cout << "1 - Поиск элемента в массиве\n";
    std::cout << "2 - Анализ эффективности\n";
    std::cout << "3 - Выход\n";
    std::cout << "Введите опцию: ";
}

int TaskHandler::getUserChoice(void)
{
    int choice;
    std::cin >> choice;
    return choice;
}

void TaskHandler::runEfficiencyAnalyzer(void)
{
    ea.analyzeBruteForce([&](const std::vector<int> &src, int x) {
        return f.bruteForce(src, x);
    }, "bruteforce");

    ea.analyzeBinSearch([&](const std::vector<int> &src, int x) {
        return f.binarySearch(src, x);
    }, "binarysearch");

    ea.analyzeComparisions([&](const std::vector<int> &src, int x) {
        return f.binarySearch(src, x);
    }, "binarysearchcompstat");
}