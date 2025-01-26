#ifndef STAGEBASE_H
#define STAGEBASE_H

#include "taskqueue.h"

class StageBase
{
public:
    virtual ~StageBase() {}

    virtual void process(TaskQueue& inputQueue, TaskQueue& outputQueue) = 0;

    virtual void shutdown() = 0;
};

#endif //STAGEBASE_H
