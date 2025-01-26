#ifndef WRITERSTAGE_H
#define WRITERSTAGE_H

#include "stagebase.h"
#include "databasehandler.h"
#include "logger.h"

const int ISSUE_ID = 9273;

class WriterStage : public StageBase
{
    public:

        WriterStage(const std::string &dbConnStr_, int numThreads_, Logger &logger_)
        : dbConnStr(dbConnStr_), numThreads(numThreads_), logger(logger_) {}

        void process(TaskQueue& inputQueue, TaskQueue& outputQueue) override;

        void shutdown() override;
    
    private:
        std::string dbConnStr;
        int numThreads;
        Logger &logger;

        void writeTasks(TaskQueue& inputQueue, TaskQueue& outputQueue);
};

#endif