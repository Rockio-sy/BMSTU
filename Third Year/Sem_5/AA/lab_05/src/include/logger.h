#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <queue>
#include <mutex>
#include <string>
#include <optional>
#include <thread>
#include <fstream>
#include <condition_variable>

struct LogMessage
{
    std::string message;
    std::string filename;
};

class LogQueue
    {
    public:
        
        LogQueue() = default;

        void enqueue(const LogMessage& log);

        std::optional<LogMessage> dequeue();

        void close();

    private:

        std::queue<LogMessage> queue;
        mutable std::mutex mtx;
        std::condition_variable cv;
        bool finished = false;
};

class Logger
{
    public:

        Logger() : stopLogging(false)
        {
            logThread = std::thread(&Logger::run, this);
        }

        ~Logger()
        {
            stop();
            if (logThread.joinable())
            {
                logThread.join();
            }
        }

        void log(const std::string& message, const std::string& filename);

        void stop();

    private:
    
        LogQueue logQueue;
        std::thread logThread;
        bool stopLogging;

        void run();
    };

#endif // LOGGER_H