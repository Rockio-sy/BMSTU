#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "webpagedownloader.h"
#include "htmlparser.h"
#include "webscraper.h"
#include "timemeasurer.h"

int main(void)
{
    std::string homeUrl = "https://www.povarenok.ru/recipes/cat/";
    // std::string recipe = "https://www.povarenok.ru/recipes/show/143888/";

    // std::vector<int> threads = {16, 32, 64};
    // std::vector<int> pages = {1, 2, 3, 4, 5, 6, 7};
    // int iterations = 15;
    // int linksPerPage = 15;
    // TimeMeasurer timeMeasurer(threads, pages, linksPerPage, iterations);
    // timeMeasurer.measureSeriesScrapingPagesLimit(homeUrl, "seriesPages.csv");
    // timeMeasurer.measureParallelScrapingPagesLimit(homeUrl);

    // TimeMeasurer timeMeasurer(threads, iterations);
    // timeMeasurer.measureSeriesScraping(homeUrl, "seriesScrapingAll.csv");
    // timeMeasurer.measureParallelScraping(homeUrl, "parallelScrapingAll3264.csv");
    

    WebScraperConfig config{.url = homeUrl, .maxSectionN = 5, .maxLinksPerSectionN = 15};
    WebScraper scraper(config);
    scraper.seriesScraping();

    // WebPageDownloader d;
    // HTMLParser p;
    // std::string recipeHtml = d.fetchContent(recipe);
    // std::ofstream recipeFile("recipe.html");
    // recipeFile << recipeHtml;
    // p.parseHTML(recipeHtml);
    // std::string recipeTitle = p.extractRecipeTitle();
    // std::vector<std::string> ingredients = p.extractRecipeIngredients("ingredients-bl");
    // std::vector<std::string> cleanIngredients = p.formatIngredients(ingredients);
    // std::vector<std::string> instructions = p.extractRecipeInstructions();
    // std::vector<std::string> cleanInstructions = p.formatInstructions(instructions);
    // std::string imageUrl = p.extractRecipeImageUrl();
    // std::cout << "title: " << recipeTitle << std::endl;
    // std::cout << "ingredients:" << std::endl;
    // for (auto &ingredient : cleanIngredients)
    // {
    //     std::cout << ingredient << std::endl;
    // }
    // std::cout << "instructions:" << std::endl;
    // for (auto &instruction : cleanInstructions)
    // {
    //     std::cout << instruction << std::endl;
    // }
    // std::cout << imageUrl << std::endl;
    return 0;
}
