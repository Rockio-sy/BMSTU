#include "webscraper.h"

std::string WebScraper::getHomeUrl(void) const
{
    return this->homePageUrl;
}

std::vector<std::string> WebScraper::getBaseSectionUrls(void) const
{
    std::vector<std::string> keys;
    for (const auto &entry : baseSections)
    {
        keys.push_back(entry.first);
    }
    return keys;
}

std::unordered_map<std::string, std::vector<std::string>> WebScraper::getSectionRecipes(void) const
{
    return this->baseSections;
}

void WebScraper::addSectionUrl(const std::string &sectionUrl)
{
    baseSections.insert(std::make_pair(sectionUrl, std::vector<std::string>()));
}

void WebScraper::addSectionUrls(const std::vector<std::string> &sectionUrls)
{
    if (!isSectionLimitSet)
    {
        maxSectionN = sectionUrls.size();
    }
    
    int sectionUrlsSize = sectionUrls.size();
    for (int i = 0; i < sectionUrlsSize && i < maxSectionN; i++)
    {
        addSectionUrl(sectionUrls[i]);
    }
}

void WebScraper::mapSectionRecipes(void)
{
    std::string className = "item-bl";
    for (auto &entry : this->baseSections)
    {
        std::vector<std::string> parts = splitUrl(entry.first);
        std::string sectionType = parts[1];
        if (!sectionType.compare("dishes"))
        {
            className = "recipe-small-bl";
        }
        else if (!sectionType.compare("category"))
        {
            className = "item-bl";
        }
        else if (!sectionType.compare("destiny"))
        {
            className = "item-bl";
        }
        std::string content = downloader.fetchContent(entry.first);
        parser.parseHTML(content);
        std::vector<std::string> hrefs = parser.extractRecipes(className);
        filterRecipeLinks(hrefs);
        if (isLinksLimitSet && static_cast<int>(hrefs.size()) > maxLinksPerSectionN)
        {
            hrefs.resize(maxLinksPerSectionN);
        }
        
        baseSections[entry.first] = hrefs;
    }
}

void WebScraper::saveRecipe(const std::string &url, const std::string &directory)
{
    WebPageDownloader localDownloader;
    HTMLParser localParser;
    std::filesystem::path dirPath = std::filesystem::current_path() / "recipes" / directory;
    if (!std::filesystem::exists(dirPath))
    {
        std::filesystem::create_directories(dirPath);
    }
    std::cout << "->processing: " << url << std::endl;
    std::string content = localDownloader.fetchContent(url);
    localParser.parseHTML(content);
    std::string recipeTitle = localParser.extractRecipeTitle();
    std::vector<std::string> rawIngredients = localParser.extractRecipeIngredients("ingredients-bl");
    std::vector<std::string> cleanIngredients = localParser.formatIngredients(rawIngredients);
    std::vector<std::string> rawInstructions = localParser.extractRecipeInstructions();
    std::vector<std::string> cleanInstructions = localParser.formatInstructions(rawInstructions);
    std::string recipeImageUrl = localParser.extractRecipeImageUrl();
    std::vector<std::string> recipeUrlParts = splitUrl(url);
    std::string filename = recipeUrlParts.back() + ".txt";
    std::filesystem::path filePath = dirPath / filename;
    std::ofstream fileStream(filePath);
    fileStream << "title: " << recipeTitle << std::endl;
    fileStream << "url: " << url << std::endl;
    if (fileStream.is_open())
    {
        std::cout << "writting to: " << filePath << std::endl;
        for (const auto& ingredient : cleanIngredients)
        {
            fileStream << "ingredient: " << ingredient << std::endl;
        }
        for (const auto &instruction : cleanInstructions)
        {
            fileStream << "instruction: " << instruction << std::endl;
        }
        fileStream << "image: " << recipeImageUrl << std::endl;
        fileStream.close();
    }
    else
    {
        std::cout << "ERROR: " << filePath << std::endl;
    }
}

void WebScraper::seriesScraping(void)
{
    std::unordered_set<std::string> visitedUrls;
    std::string homePageContent = downloader.fetchContent(this->homePageUrl);
    if (!homePageContent.empty())
    {
        std::cout << "connected" << std::endl;
    }
    parser.parseHTML(homePageContent);
    std::vector<std::string> pageUrls = parser.extractCategoryURLs();
    std::cout << "Fetched: " << pageUrls.size() << std::endl;
    this->addSectionUrls(pageUrls);
    this->mapSectionRecipes();
    std::cout << *(this) << std::endl;

    for (auto &entry : this->baseSections)
    {
        std::cout << "-->" << entry.first << std::endl;
        std::vector<std::string> parts = splitUrl(entry.first);
        std::string dirName = parts.back();
        for (auto &url : entry.second)
        {
            if (visitedUrls.insert(url).second)
            {
                this->saveRecipe(url, dirName);
            }
            else
            {
                std::cout << "URL already processed: " << url << std::endl;
            }
        }
    }
}

void WebScraper::parallelScraping(int threadsN)
{
    HTMLParser localParser;
    WebPageDownloader localDownloader;
    std::vector<std::thread> threads;
    std::mutex queueMutex, visitedMutex, outputMutex;
    std::queue<qTask> taskQueue;
    std::unordered_set<std::string> visitedUrls;
    std::unordered_map<std::thread::id, int> taskCount;

    std::string homePageContent = localDownloader.fetchContent(this->homePageUrl);
    if (!homePageContent.empty())
    {
        std::cout << "connected" << std::endl;
    }

    localParser.parseHTML(homePageContent);
    std::vector<std::string> pageUrls = localParser.extractCategoryURLs();
    this->addSectionUrls(pageUrls);
    std::cout << baseSections.size() << std::endl;
 
    for (auto &entry : this->baseSections)
    {
        qTask initialTask{entry.first, qTaskType::FetchRecipes, ""};
        taskQueue.push(initialTask);
    }

    std::cout << "taskQueue size: " << taskQueue.size() << std::endl;

    auto worker = [&]()
    {
        HTMLParser parserInstance;
        WebPageDownloader downloaderInstance;

        std::thread::id threadId = std::this_thread::get_id();
        taskCount[threadId] = 0;

        while (true)
        {
            qTask task;
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                if (taskQueue.empty())
                {
                    break;
                }
                task = taskQueue.front();
                taskQueue.pop();
            }

            {
                std::lock_guard<std::mutex> lock(visitedMutex);
                if (!visitedUrls.insert(task.url).second)
                {
                    continue;
                }
            }

            {
                std::lock_guard<std::mutex> printLock(outputMutex);
                std::cout << "worker " << threadId << ": processing " << task.url << " ";
            }

            if (task.type == qTaskType::FetchRecipes)
            {
                std::cout << "task type: FetchRecipes" << std::endl;
                std::string className = "item-bl";
                std::vector<std::string> parts = splitUrl(task.url);
                std::string sectionType = parts[1];
                if (!sectionType.compare("dishes"))
                {
                    className = "recipe-small-bl";
                }
                else if (!sectionType.compare("category"))
                {
                    className = "item-bl";
                }
                else if (!sectionType.compare("destiny"))
                {
                    className = "item-bl";
                }
                std::string pageContent = downloaderInstance.fetchContent(task.url);
                parserInstance.parseHTML(pageContent);
                std::vector<std::string> hrefs = parserInstance.extractRecipes(className);
                filterRecipeLinks(hrefs);
                std::string dirName = parts.back();

                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    for (const auto &href : hrefs)
                    {
                        taskQueue.push({href, qTaskType::SaveRecipeDetails, dirName});
                    }
                }
            }
            else if ((task.type == qTaskType::SaveRecipeDetails) && (task.directory != ""))
            {
                std::cout << "task type: SaveRecipeDetails" << std::endl;
                saveRecipe(task.url, task.directory);
            }

            taskCount[threadId]++;

            {
                std::lock_guard<std::mutex> lock(outputMutex);
                std::cout << "Worker " << threadId << ": processed " << task.url << std::endl;
                std::cout << "Queue Size: " << taskQueue.size() << " | Visited: " << visitedUrls.size() << std::endl;
            }
        }

        {
            std::lock_guard<std::mutex> lock(outputMutex);
            std::cout << "Worker " << threadId << " completed " << taskCount[threadId] << " tasks." << std::endl;
        }
    };

    for (int i = 0; i < threadsN; i++)
    {
        threads.emplace_back(worker);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }
}

void WebScraper::filterRecipeLinks(std::vector<std::string>& recipes)
{
    std::regex recipeUrlPattern("https://www.povarenok.ru/recipes/show/[0-9]+/$");
    std::unordered_set<std::string> uniqueRecipes;

    for (const auto& url : recipes)
    {
        if (std::regex_match(url, recipeUrlPattern))
        {
            uniqueRecipes.insert(url);
        }
    }

    recipes.assign(uniqueRecipes.begin(), uniqueRecipes.end());
}

std::vector<std::string> WebScraper::splitUrl(const std::string& url)
{
    std::vector<std::string> parts;
    std::string suburl;

    size_t start = url.find("povarenok.ru/");
    if (start != std::string::npos)
    {
        suburl = url.substr(start + 13);
    }
    else
    {
        suburl = url;
    }

    size_t pos = 0;
    std::string token;
    std::string delimiter = "/";
    while ((pos = suburl.find(delimiter)) != std::string::npos)
    {
        token = suburl.substr(0, pos);
        if (!token.empty())
        {
            parts.push_back(token);
        }
        suburl.erase(0, pos + delimiter.length());
    }

    if (!suburl.empty())
    {
        parts.push_back(suburl);
    }

    return parts;
}

std::ostream& operator<<(std::ostream& os, const WebScraper& scraper)
{
    int totalSections = scraper.baseSections.size();
    int totalLinks = 0;

    for (const auto &entry : scraper.baseSections)
    {
        totalLinks += entry.second.size();
    }

    os << "Sections: " << totalSections << ", Total Links: " << totalLinks << std::endl;

    for (const auto& entry : scraper.baseSections)
    {
        os << entry.first << ": " << entry.second.size() << " links\n";
    }

    return os;
}