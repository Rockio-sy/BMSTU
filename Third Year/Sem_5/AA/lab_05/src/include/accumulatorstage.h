#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include "stagebase.h"
#include "logger.h"

class AccumulatorStage : public StageBase
{
    public:

        AccumulatorStage(int numThreads_, Logger &logger_)
        : numThreads(numThreads_), logger(logger_) {}

        void process(TaskQueue& inputQueue, TaskQueue& outputQueue) override;

        void shutdown() override;
    
    private:

        int numThreads;
        Logger &logger;

        void accumulateTasks(TaskQueue& inputQueue, TaskQueue& outputQueue);
};

#endif // ACCUMULATOR_H