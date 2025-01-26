#ifndef PARSERSTAGE_H
#define PARSERSTAGE_H

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <locale>
#include <stdexcept>
#include <codecvt>
#include <sstream>
#include "stagebase.h"
#include "taskqueue.h"
#include "task.h"
#include "logger.h"

class ParserStage : public StageBase
{
    public:

        explicit ParserStage(int numThreads_, Logger &logger_)
        : numThreads(numThreads_), logger(logger_){}

        void process(TaskQueue& inputQueue, TaskQueue& outputQueue) override;

        void shutdown() override;
    
    private:

        int numThreads;
        Logger &logger;

        void parseTasks(TaskQueue& inputQueue, TaskQueue& outputQueue);

        void parseTask(Task &task);

        Ingredient parseIngredient(const std::string &line);
};

#endif // PARSERSTAGE_H