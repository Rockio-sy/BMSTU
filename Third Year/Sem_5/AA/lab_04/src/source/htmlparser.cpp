#include "htmlparser.h"

void HTMLParser::parseHTML(const std::string &html)
{
    myhtml_parse(tree, MyENCODING_UTF_8, html.c_str(), html.size());
}

std::vector<std::string> HTMLParser::extractCategoryURLs(void)
{
    std::vector<std::string> urls;
    mystatus_t status;
    std::regex valid_url_regex("^https://www\\.povarenok\\.ru/recipes/[^#]+$", std::regex::icase);
    myhtml_collection_t* a_collection = myhtml_get_nodes_by_tag_id(tree, NULL, MyHTML_TAG_A, &status);

    if (a_collection)
    {
        for (size_t i = 0; i < a_collection->length; i++)
        {
            myhtml_tree_node_t* parent_node = myhtml_node_parent(a_collection->list[i]);
            if (myhtml_node_tag_id(parent_node) == MyHTML_TAG_H2)
            {
                myhtml_tree_attr_t* class_attr = myhtml_attribute_by_key(parent_node, "class", strlen("class"));
                if (class_attr && strstr(myhtml_attribute_value(class_attr, NULL), "t-branch"))
                {
                    myhtml_tree_attr_t* href_attr = myhtml_attribute_by_key(a_collection->list[i], "href", strlen("href"));
                    if (href_attr)
                    {
                        const char* href_value = myhtml_attribute_value(href_attr, NULL);
                        if (href_value && std::regex_match(href_value, valid_url_regex))
                        {
                            urls.emplace_back(href_value);
                        }
                    }
                }
            }
        }
        myhtml_collection_destroy(a_collection);
    }
    return urls;
}

std::vector<std::string> HTMLParser::extractRecipes(const std::string &className)
{
    std::vector<std::string> recipes;
    mystatus_t status;

    myhtml_collection_t* collection = myhtml_get_nodes_by_attribute_value_whitespace_separated(
        tree, NULL, NULL, false, "class", strlen("class"), className.c_str(), className.length(), &status);

    if (collection)
    {
        for (size_t i = 0; i < collection->length; i++)
        {
            std::vector<std::string> foundRecipes = findRecipesInNode(collection->list[i]);
            recipes.insert(recipes.end(), foundRecipes.begin(), foundRecipes.end());
        }
        myhtml_collection_destroy(collection);
    }
    return recipes;
}

std::vector<std::string> HTMLParser::findRecipesInNode(myhtml_tree_node_t* node)
{
    std::vector<std::string> links;
    while (node)
    {
        if (myhtml_node_tag_id(node) == MyHTML_TAG_A)
        {
            myhtml_tree_attr_t* attr = myhtml_attribute_by_key(node, "href", strlen("href"));
            if (attr)
            {
                const char* value = myhtml_attribute_value(attr, NULL);
                if (value)
                {
                    links.push_back(value);
                }
            }
        }
        std::vector<std::string> childLinks = findRecipesInNode(myhtml_node_child(node));
        links.insert(links.end(), childLinks.begin(), childLinks.end());

        node = myhtml_node_next(node);
    }
    return links;
}

std::vector<std::string> HTMLParser::extractRecipeIngredients(const std::string &className)
{
    std::vector<std::string> ingredientsTexts;
    // int ingredientCount = 0;
    mystatus_t status;

    myhtml_collection_t* div_collection = myhtml_get_nodes_by_tag_id(tree, NULL, MyHTML_TAG_DIV, &status);

    if (div_collection && div_collection->length > 0)
    {
        for (size_t i = 0; i < div_collection->length; i++)
        {
            myhtml_tree_attr_t* class_attr = myhtml_attribute_by_key(div_collection->list[i], "class", strlen("class"));
            if (class_attr)
            {
                const char* class_value = myhtml_attribute_value(class_attr, NULL);
                if (class_value && strstr(class_value, className.c_str()))
                {
                    // std::cout << "Found ingredients-bl class" << std::endl;

                    myhtml_tree_node_t* ulNode = myhtml_node_child(div_collection->list[i]);
                    while (ulNode && myhtml_node_tag_id(ulNode) != MyHTML_TAG_UL)
                    {
                        ulNode = myhtml_node_next(ulNode);
                    }

                    if (ulNode)
                    {
                        myhtml_collection_t* li_collection = myhtml_get_nodes_by_tag_id_in_scope(
                            tree, NULL, ulNode, MyHTML_TAG_LI, &status);

                        if (li_collection && li_collection->length > 0)
                        {
                            for (size_t j = 0; j < li_collection->length; j++)
                            {
                                std::string fullText = collectNodeText(li_collection->list[j]);
                                ingredientsTexts.push_back(fullText);
                            }
                            // ingredientCount = li_collection->length;
                            myhtml_collection_destroy(li_collection);
                        }
                    }
                }
            }
        }
        myhtml_collection_destroy(div_collection);
    }

    return ingredientsTexts;
}

std::string HTMLParser::extractRecipeTitle()
{
    mystatus_t status;
    std::string title;

    myhtml_collection_t* h1_collection = myhtml_get_nodes_by_tag_id(tree, NULL, MyHTML_TAG_H1, &status);
    if (h1_collection && h1_collection->length > 0)
    {
        myhtml_tree_node_t* h1_node = h1_collection->list[0];
        title = collectNodeText(h1_node);
    }

    if (h1_collection)
    {
        myhtml_collection_destroy(h1_collection);
    }

    return title;
}

std::vector<std::string> HTMLParser::extractRecipeInstructions()
{
    std::vector<std::string> steps;
    mystatus_t status;

    myhtml_collection_t* ulCollection = myhtml_get_nodes_by_attribute_value(
        tree, NULL, NULL, false, "itemprop", strlen("itemprop"), "recipeInstructions", strlen("recipeInstructions"), &status);

    if (ulCollection && ulCollection->length > 0)
    {
        myhtml_tree_node_t* ulNode = ulCollection->list[0];

        myhtml_collection_t* liCollection = myhtml_get_nodes_by_tag_id_in_scope(
            tree, NULL, ulNode, MyHTML_TAG_LI, &status);

        if (liCollection && liCollection->length > 0)
        {
            for (size_t i = 0; i < liCollection->length; i++)
            {
                myhtml_tree_attr_t* class_attr = myhtml_attribute_by_key(liCollection->list[i], "class", strlen("class"));
                if (class_attr)
                {
                    const char* class_value = myhtml_attribute_value(class_attr, NULL);
                    if (class_value && strstr(class_value, "cooking-bl") != NULL)
                    {
                        std::string stepText = collectNodeText(liCollection->list[i]);
                        steps.push_back(stepText);
                    }
                }
            }
            myhtml_collection_destroy(liCollection);
        }
    }
    if (ulCollection)
    {
        myhtml_collection_destroy(ulCollection);
    }
    return steps;
}

std::string HTMLParser::extractRecipeImageUrl()
{
    mystatus_t status;
    std::string imageUrl;

    myhtml_collection_t* divCollection = myhtml_get_nodes_by_attribute_value(
        tree, NULL, NULL, false, "class", strlen("class"), "m-img", strlen("m-img"), &status);

    if (divCollection && divCollection->length > 0)
    {
        myhtml_tree_node_t* imgNode = myhtml_node_child(divCollection->list[0]);

        while (imgNode && myhtml_node_tag_id(imgNode) != MyHTML_TAG_IMG)
        {
            imgNode = myhtml_node_next(imgNode);
        }

        if (imgNode)
        {
            myhtml_tree_attr_t* srcAttr = myhtml_attribute_by_key(imgNode, "src", strlen("src"));
            if (srcAttr)
            {
                const char* srcValue = myhtml_attribute_value(srcAttr, NULL);
                if (srcValue)
                {
                    imageUrl = srcValue;
                }
            }
        }
    }

    if (divCollection)
    {
        myhtml_collection_destroy(divCollection);
    }

    return imageUrl;
}


std::string HTMLParser::collectNodeText(myhtml_tree_node_t* node)
{
    std::string text;
    if (node == NULL)
    {
        return text;
    }

    if (myhtml_node_tag_id(node) == MyHTML_TAG__TEXT)
    {
        const char* node_text = myhtml_node_text(node, NULL);
        if (node_text)
        {
            text += std::string(node_text);
        }
    }

    myhtml_tree_node_t* child = myhtml_node_child(node);
    while (child)
    {
        text += collectNodeText(child);
        child = myhtml_node_next(child);
    }
    return text;
}

std::vector<std::string> HTMLParser::formatIngredients(const std::vector<std::string> &rawIngredients)
{
    std::vector<std::string> formattedIngredients;
    for (const auto& rawIngredient : rawIngredients)
    {
        std::istringstream iss(rawIngredient);
        std::string formattedLine;
        std::string word;
        std::string cleanedIngredient;
        bool firstWord = true;

        while (iss >> word)
        {
            word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
            if (!word.empty())
            {
                if (!firstWord)
                {
                    cleanedIngredient += " ";
                }
                cleanedIngredient += word;
                firstWord = false;
            }
        }
        if (!cleanedIngredient.empty())
        {
            formattedIngredients.push_back(cleanedIngredient);
        }
    }
    return formattedIngredients;
}

std::vector<std::string> HTMLParser::formatInstructions(const std::vector<std::string> &rawInstructions)
{
    std::vector<std::string> formattedInstructions;
    for (const auto& rawInstruction : rawInstructions)
    {
        std::string trimmed = std::regex_replace(rawInstruction, std::regex("\\s+"), " ");
        trimmed = std::regex_replace(trimmed, std::regex("^\\s+|\\s+$"), "");
        if (!trimmed.empty())
        {
            formattedInstructions.push_back(trimmed);
        }
    }
    return formattedInstructions;
}


void HTMLParser::replaceFileExtension(std::string &fileExtension)
{
    std::size_t pos = fileExtension.find(".html");
    if (pos != std::string::npos)
    {
        fileExtension.replace(pos, 5, ".txt");
    }
}
