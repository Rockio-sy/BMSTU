#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <iostream>
#include <myhtml/api.h>
#include <vector>
#include <string>
#include <regex>

class HTMLParser
{
    public:
        HTMLParser()
        {
            myhtml = myhtml_create();
            myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
            tree = myhtml_tree_create();
            myhtml_tree_init(tree, myhtml);
        }

        ~HTMLParser()
        {
            myhtml_tree_destroy(tree);
            myhtml_destroy(myhtml);
        }

        void parseHTML(const std::string &html);

        std::vector<std::string> extractCategoryURLs(void);

        std::vector<std::string> extractRecipes(const std::string &className);

        std::vector<std::string> extractRecipeIngredients(const std::string &className);

        std::string extractRecipeTitle();

        std::vector<std::string> extractRecipeInstructions();

        void replaceFileExtension(std::string &fileExtension);

        std::vector<std::string> formatIngredients(const std::vector<std::string> &rawIngredients);

        std::vector<std::string> formatInstructions(const std::vector<std::string> &rawInstructions);

        std::string extractRecipeImageUrl();

    private:
        myhtml_t* myhtml;
        myhtml_tree_t* tree;

        std::vector<std::string> findRecipesInNode(myhtml_tree_node_t* node);

        std::string collectNodeText(myhtml_tree_node_t* node);
};

#endif // HTMLPARSER_H