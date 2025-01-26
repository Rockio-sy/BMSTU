#include "finder.h"

std::pair<int, int> Finder::bruteForce(const std::vector<int> &src, int x)
{
    int comparisons = 0;
    for (unsigned long int i = 0; i < src.size(); i++)
    {
        comparisons++;
        if (src[i] == x)
        {
            return {i, comparisons};
        }
    }
    return {-1, comparisons};
}

std::pair<int, int> Finder::binarySearch(const std::vector<int> &src, int x)
{
    int left = 0;
    int right = src.size() - 1;
    int comparisons = 0;
    
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        comparisons++;

        if (src[mid] == x)
        {
            return {mid, comparisons};
        }
        else if (src[mid] < x)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return {-1, comparisons};
}
