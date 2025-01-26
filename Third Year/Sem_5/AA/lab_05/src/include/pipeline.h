#ifndef PIPELINE_H
#define PIPELINE_H

#include <vector>
#include <thread>
#include "taskqueue.h"
#include "stagebase.h"

class Pipeline
{
    public:
        Pipeline() = default;
        void addStage(StageBase *stage, TaskQueue *inputQueue, TaskQueue *outputQueue);
        void start();
        void shutdown();

    private:
        std::vector<StageBase *> stages;
        std::vector<TaskQueue *> queues;
        std::vector<std::thread> threads;
};

#endif // PIPELINE_H