#ifndef READERSTAGE_H
#define READERSTAGE_H

#include <iostream>
#include <fstream>
#include "task.h"
#include "stagebase.h"
#include "taskqueue.h"
#include "logger.h"

class ReaderStage : public StageBase
{
    public:

        explicit ReaderStage(int numThreads_, Logger &logger_)
        : numThreads(numThreads_), logger(logger_) {}
        
        void process(TaskQueue& inputQueue, TaskQueue& outputQueue) override;

        void shutdown() override;
    
    private:

        int numThreads;
        Logger &logger;

        void readTasks(TaskQueue &inputQueue, TaskQueue &outputQueue);
};

#endif // READERSTAGE_H
