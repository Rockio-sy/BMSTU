#ifndef EFFICIENCYANALYZER_H
#define EFFICIENCYANALYZER_H

#include <iostream>
#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>
#include "auxiliar.h"

class EfficiencyAnalyzer
{
    public:
        EfficiencyAnalyzer() = default;
        EfficiencyAnalyzer(int x);

        int analyzeBruteForce(std::function<std::pair<int, int>(const std::vector<int> &, int )> func, const std::string &algorithmName);
        int analyzeBinSearch(std::function<std::pair<int, int>(const std::vector<int> &, int )> func, const std::string &algorithmName);
        int analyzeComparisions(std::function<std::pair<int, int>(const std::vector<int> &, int )> func, const std::string &algorithmName);

    private:
        int len;
        std::vector<int> src;
        
        void calculateSourceArrayLength(int x);
        void generateSourceArray(int min = -30000, int max = 30000);

};

#endif