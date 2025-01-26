#include "auxiliar.h"

int sourceInput(std::vector<int> &src, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cin >> src[i];
    }
    return 0;
}

void sourcePrint(std::vector<int> &src, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << src[i] << " ";
    }
    std::cout << std::endl;
}
