#include "efficiencyanalyzer.h"

EfficiencyAnalyzer::EfficiencyAnalyzer(int x)
{
    calculateSourceArrayLength(x);
    generateSourceArray();
}

void EfficiencyAnalyzer::calculateSourceArrayLength(int x)
{
    this->len =  x / 8 + ((x >> 2) % 10 == 0 ? x % 1000 : ((x >> 2) % 10 * (x % 10) + (x >> 1) % 10));
}

void EfficiencyAnalyzer::generateSourceArray(int min, int max)
{
    srand(time(nullptr));
    this->src.clear();
    this->src.reserve(this->len);

    std::set<int> seenNumbers;

    while (static_cast<int>(this->src.size()) < this->len)
    {
        int randomValue = min + rand() % (max - min + 1);
        if (seenNumbers.insert(randomValue).second)
        {
            this->src.push_back(randomValue);
        }
    }
}

int EfficiencyAnalyzer::analyzeBruteForce(std::function<std::pair<int, int>(const std::vector<int> &, int )> func, const std::string &algorithmName)
{
    sourcePrint(this->src, this->src.size());

    std::ofstream outFile;
    outFile.open(algorithmName + ".csv");
    outFile << "index,comparisons\n";

    std::vector<std::pair<int, int>> results;
    for (int x : this->src)
    {
        results.push_back(func(this->src, x));
    }
    results.push_back(func(src, std::numeric_limits<int>::min()));

    for (const auto &result : results)
    {
        outFile << result.first << "," << result.second << "\n";
    }

    outFile.close();
    return 0;
}

int EfficiencyAnalyzer::analyzeBinSearch(std::function<std::pair<int, int>(const std::vector<int> &, int )> func, const std::string &algorithmName)
{
    sourcePrint(this->src, this->src.size());
    std::sort(this->src.begin(), this->src.end());

    std::ofstream outFile;
    outFile.open(algorithmName + ".csv");
    outFile << "index,comparisons\n";

    std::vector<std::pair<int, int>> results;
    for (int x : this->src)
    {
        results.push_back(func(this->src, x));
    }
    results.push_back(func(src, std::numeric_limits<int>::min()));

    for (const auto &result : results)
    {
        outFile << result.first << "," << result.second << "\n";
    }

    outFile.close();
    return 0;
}

int EfficiencyAnalyzer::analyzeComparisions(std::function<std::pair<int, int>(const std::vector<int> &, int )> func, const std::string &algorithmName)
{
    sourcePrint(this->src, this->src.size());
    std::sort(this->src.begin(), this->src.end());

    std::ofstream outFile;
    outFile.open(algorithmName + ".csv");
    outFile << "n,comparisons\n";

    std::vector<std::pair<int, int>> results;
    for (int x : this->src)
    {
        results.push_back(func(this->src, x));
    }
    results.push_back(func(src, std::numeric_limits<int>::min()));

    std::sort(results.begin(), results.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b)
    {
        return a.second < b.second;
    });

    for (size_t i = 0; i < results.size(); ++i)
    {
        outFile << i << "," << results[i].second << "\n";
    }

    outFile.close();
    return 0;
}

