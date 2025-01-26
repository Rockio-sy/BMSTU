#ifndef TASKGENERATORSTAGE_H
#define TASKGENERATORSTAGE_H

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "stagebase.h"
#include "logger.h"

class TaskGeneratorStage : public StageBase
{
    public:

        TaskGeneratorStage(const std::string& directoryPath_, int numThreads_, Logger &logger_)
        : directoryPath(directoryPath_), numThreads(numThreads_), threadsCompleted(0), logger(logger_){}

        void process(TaskQueue& inputQueue, TaskQueue& outputQueue) override;

        void shutdown() override;

    private:
    
        std::string directoryPath;
        int numThreads;
        std::atomic<int> threadsCompleted;
        std::mutex mtx;
        std::condition_variable cv;
        Logger &logger;

        void generateTasks(TaskQueue &outputQueue, const std::vector<std::string> &paths, int start, int end);

        std::vector<std::string> collectAllFilePaths(const std::string &directoryPath);
};

#endif // TASKGENERATORSTAGE_H