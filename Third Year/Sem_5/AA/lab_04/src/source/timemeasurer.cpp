#include "timemeasurer.h"

void TimeMeasurer::measureSeriesScraping(const std::string &url, const std::string &filename)
{
    double totalDuration = 0;
    std::cout << "-->measuring series scrapping" << std::endl;
    for (int i = 0; i < iterations; i++)
    {
        std::cout << "iteration: " << i << std::endl;
        // WebScraperConfig config{.url = url, .maxSectionN = 10, .maxLinksPerSectionN = 15};
        WebScraperConfig config{.url = url};
        WebScraper scraper(config);

        auto start = std::chrono::high_resolution_clock::now();
        scraper.seriesScraping();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        totalDuration += elapsed.count();
        std::filesystem::path recipesDir = std::filesystem::current_path() / "recipes";
        if (std::filesystem::exists(recipesDir))
        {
            std::filesystem::remove_all(recipesDir);
            // std::cout << "Removed " << num_removed << " files and directories." << std::endl;
        }
        std::cout << "series scrapping " << i << ": " << elapsed.count() << std::endl;
    }
    double averageTime = totalDuration / iterations;
    std::cout << "Series sraping avg time: " << averageTime << std::endl;
    std::ofstream outFile(filename);
    outFile << "threads,time\n";
    outFile << "1," << averageTime << "\n";
    outFile.close();
}
        
void TimeMeasurer::measureParallelScraping(const std::string &url, const std::string &filename)
{
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile.is_open())
    {
        std::cerr << "failed openning file: " << filename << std::endl;
        return;
    }
    outFile << "threads,time" << std::endl;
    for (int threadCount : threads)
    {
        double totalDuration = 0;
        std::cout << "----> measuring parallel scraping with: " << threadCount << " threads" << std::endl;
        for (int i = 0; i < iterations; i++)
        {
            std::cout << "->iteration: " << i << std::endl;
            // WebScraperConfig config{.url = url, .maxSectionN = 5, .maxLinksPerSectionN = 10};
            WebScraperConfig config{.url = url};
            WebScraper scraper(config);
            auto start = std::chrono::high_resolution_clock::now();
            scraper.parallelScraping(threadCount);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            totalDuration += elapsed.count();
            std::filesystem::path recipesDir = std::filesystem::current_path() / "recipes";
            std::filesystem::remove_all(recipesDir);
            std::cout << "parallel scrapping: " << i << " (" << threadCount << " threads): " << elapsed.count() << std::endl;
        }
        double averageTime = totalDuration / iterations;
        std::cout << "average time for parallel scrapping (" << threadCount << "): "<< averageTime << std::endl;
        outFile << threadCount << "," << averageTime << std::endl;
    }
    outFile.close();
}

void TimeMeasurer::measureSeriesScrapingPagesLimit(const std::string &url, const std::string &filename)
{
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile.is_open())
    {
        std::cerr << "failed opening file: " << filename << std::endl;
        return;
    }
    outFile << "pages,time" << std::endl;

    for (auto &pageN : pages)
    {
        std::cout << "---> Measures for pages: " << pageN << std::endl;
        double totalDuration = 0;

        for (int i = 0; i < iterations; i++)
        {
            std::cout << "->iteration: " << i << std::endl;
            WebScraperConfig config{.url = url, .maxSectionN = pageN, .maxLinksPerSectionN = linksN};
            WebScraper scraper(config);

            auto start = std::chrono::high_resolution_clock::now();
            scraper.seriesScraping();
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> elapsed = end - start;
            totalDuration += elapsed.count();

            std::filesystem::path recipesDir = std::filesystem::current_path() / "recipes";
            if (std::filesystem::exists(recipesDir))
            {
                std::filesystem::remove_all(recipesDir);
            }
            std::cout << "series scrapping " << i << ": " << elapsed.count() << " ms" << std::endl;
        }

        double averageTime = totalDuration / iterations;
        std::cout << "Series scraping avg time: " << averageTime << " ms" << std::endl;

        if (outFile.is_open())
        {
            std::cout << "WRITING TO FILE: " << pageN << "," << averageTime << std::endl;
            outFile << pageN << "," << averageTime << "\n";
        }
    }

    outFile.close();
}

void TimeMeasurer::measureParallelScrapingPagesLimit(const std::string &url)
{
    for (int threadCount : threads)
    {
        std::string filename = "threads" + std::to_string(threadCount) + ".csv";
        std::ofstream outFile(filename);
        outFile << "pages,time\n";

        for (int pageN : pages)
        {
            std::cout << "---> Measures for " << threadCount << " threads with page limit " << pageN << std::endl;
            double totalDuration = 0;

            for (int i = 0; i < iterations; i++)
            {
                std::cout << "-> iteration: " << i << std::endl;
                WebScraperConfig config{.url = url, .maxSectionN = pageN, .maxLinksPerSectionN = linksN};
                WebScraper scraper(config);

                auto start = std::chrono::high_resolution_clock::now();
                scraper.parallelScraping(threadCount);
                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double, std::milli> elapsed = end - start;
                totalDuration += elapsed.count();

                std::filesystem::path recipesDir = std::filesystem::current_path() / "recipes";
                if (std::filesystem::exists(recipesDir))
                {
                    std::filesystem::remove_all(recipesDir);
                }

                std::cout << "parallel scraping: " << i << " (" << threadCount << " threads): " << elapsed.count() << " ms" << std::endl;
            }

            double averageTime = totalDuration / iterations;
            std::cout << "Parallel scraping avg time: " << averageTime << " ms" << std::endl;

            if (outFile.is_open())
            {
                outFile << pageN << "," << averageTime << std::endl;
            }
        }

        outFile.close();
    }
}
