#include "logger.h"

void LogQueue::enqueue(const LogMessage& log)
{
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(log);
    cv.notify_one();
}

std::optional<LogMessage> LogQueue::dequeue()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]{ return !queue.empty() || finished; });
    if (queue.empty()) {
        return {};
    }
    LogMessage log = std::move(queue.front());
    queue.pop();
    return log;
}

void LogQueue::close()
{
    std::lock_guard<std::mutex> lock(mtx);
    finished = true;
    cv.notify_all();
}

void Logger::log(const std::string& message, const std::string& filename)
{
    logQueue.enqueue({message, filename});
}

void Logger::stop()
{
    logQueue.close();
    stopLogging = true;
}

 void Logger::run()
 {
    while (true)
    {
        auto logOpt = logQueue.dequeue();
        if (logOpt)
        {
            auto& log = *logOpt;
            std::cout << log.message << std::endl;
            std::ofstream file(log.filename, std::ios::out | std::ios::app);
            if (file.is_open())
            {
                file << log.message << std::endl;
                file.close();
            }
        }
        else
        {
            break;
        }
    }
}