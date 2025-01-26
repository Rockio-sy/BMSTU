#include "webpagedownloader.h"

size_t WebPageDownloader::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
{
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string WebPageDownloader::fetchContent(const std::string &url)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    long http_code = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else
        {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (http_code != 200)
            {
                std::cerr << "error: " << http_code << std::endl;
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return (http_code == 200) ? convertEncoding(readBuffer, "Windows-1251", "UTF-8") : "";
}

void WebPageDownloader::fetchAndSaveContent(const std::string &url,const std::string &filename)
{
    std::string content = this->fetchContent(url);
    if (!content.empty())
    {
        std::ofstream outFile(filename);
        if (outFile.is_open())
        {
            outFile << content;
            outFile.close();
        }
    }
}

std::string WebPageDownloader::convertEncoding(const std::string &input, const std::string &from_enc, const std::string &to_enc)
{
    size_t in_size = input.size();
    size_t out_size = in_size * 2;
    std::unique_ptr<char[]> out_buf(new char[out_size]);

    char *in_ptr = const_cast<char*>(input.data());
    char *out_ptr = out_buf.get();

    iconv_t cd = iconv_open(to_enc.c_str(), from_enc.c_str());
    if (cd == (iconv_t)-1)
    {
        perror("iconv_open");
        return "";
    }

    if (iconv(cd, &in_ptr, &in_size, &out_ptr, &out_size) == (size_t)-1)
    {
        perror("iconv");
        iconv_close(cd);
        return "";
    }

    iconv_close(cd);
    return std::string(out_buf.get(), out_ptr - out_buf.get());
}
