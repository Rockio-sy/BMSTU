#ifndef WEBSCRAPER_H
#define WEBSCRAPER_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <mutex>
#include <queue>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <unordered_set>
#include "webpagedownloader.h"
#include "htmlparser.h"

enum class qTaskType
{
    FetchRecipes,
    SaveRecipeDetails
};

struct qTask
{
    std::string url;
    qTaskType type;
    std::string directory;
};

struct WebScraperConfig
{
    std::string url;
    int maxSectionN = -1;
    int maxLinksPerSectionN = -1;
};

class WebScraper
{
    public:
        WebScraper(const WebScraperConfig &config)
        : homePageUrl(config.url),
        maxSectionN(config.maxSectionN),
        isSectionLimitSet(config.maxSectionN >= 0),
        maxLinksPerSectionN(config.maxLinksPerSectionN),
        isLinksLimitSet(config.maxLinksPerSectionN >= 0) {}

        void addSectionUrl(const std::string &url);

        void addSectionUrls(const std::vector<std::string> &sectionUrls);

        std::string getHomeUrl(void) const;

        std::vector<std::string> getBaseSectionUrls(void) const;

        std::unordered_map<std::string, std::vector<std::string>> getSectionRecipes(void) const;

        void mapSectionRecipes(void);

        void saveRecipe(const std::string &url, const std::string &directory);

        void seriesScraping(void);

        void parallelScraping(int threadsN);

        void filterRecipeLinks(std::vector<std::string> &recipes);

        friend std::ostream &operator<<(std::ostream &os, const WebScraper &scraper);

    private:
        HTMLParser parser;
        WebPageDownloader downloader;

        std::string homePageUrl;
        std::unordered_map<std::string, std::vector<std::string>> baseSections;

        int maxSectionN;
        bool isSectionLimitSet;

        int maxLinksPerSectionN;
        bool isLinksLimitSet;

        std::vector<std::string> splitUrl(const std::string &url);
};

#endif // WEBSCRAPER_H
