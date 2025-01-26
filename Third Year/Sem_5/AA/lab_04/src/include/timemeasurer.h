#ifndef TIMEMEASURER_H
#define TIMEMEASURER_H

#include <iostream>
#include <chrono>
#include <string>
#include <filesystem>
#include "webscraper.h"

class TimeMeasurer
{
    public:
        TimeMeasurer() = default;

        TimeMeasurer(const std::vector<int> &threadsParam, int iterationsParam) : threads(threadsParam), iterations(iterationsParam) {}

        TimeMeasurer(const std::vector<int> &threadsParam, const std::vector<int> &pagesParam, int linksNParam, int iterationsParam) : threads(threadsParam), pages(pagesParam), linksN(linksNParam), iterations(iterationsParam) {}

        void measureSeriesScraping(const std::string &url, const std::string &filename);

        void measureParallelScraping(const std::string &url, const std::string &filename);

        void measureSeriesScrapingPagesLimit(const std::string &url, const std::string &filename);

        void measureParallelScrapingPagesLimit(const std::string &url);

    private:
        std::vector<int> threads;
        std::vector<int> pages;
        int linksN;
        int iterations;
};

#endif