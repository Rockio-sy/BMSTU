#ifndef WEBPAGEDOWNLOADER_H
#define WEBPAGEDOWNLOADER_H

#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <cerrno>
#include <iconv.h>
#include"myhtml/api.h"

class WebPageDownloader
{
    public:
        WebPageDownloader() = default;

        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp);

        std::string fetchContent(const std::string &url);

        void fetchAndSaveContent(const std::string &url, const std::string &filename);

    private:
        std::string convertEncoding(const std::string &input, const std::string &from_enc, const std::string &to_enc);
};

#endif // ENDWEBPAGEDOWNLOADER_H