#ifndef FINDER_H
#define FINDER_H

#include <iostream>
#include <vector>
#include <utility>

class Finder
{
    public:
        Finder() = default;
        
        std::pair<int, int> bruteForce(const std::vector<int> &src, int x);
        std::pair<int, int> binarySearch(const std::vector<int> &src, int x);
};

#endif // FINDER_H