#ifndef TASKMANAGER_H
#define TASKMANAFER_H

#include <iostream>
#include <vector>
#include <utility>
#include "finder.h"
#include "efficiencyanalyzer.h"
#include "auxiliar.h"

#define ERROR_ARRAY_LEN 10

class TaskHandler
{
    public:
        TaskHandler() = default;
        void run(void);
    
    private:
        Finder f;
        EfficiencyAnalyzer ea{8220};

        void displayMenu(void);
        int getUserChoice(void);
        void executeAlgorithm(std::pair<int, int> (Finder::*algorithm)(const std::vector<int> &, int));
        void runEfficiencyAnalyzer(void);
        void executeAlgorithmChoice(void);
};

#endif // TASKMANAGER_H