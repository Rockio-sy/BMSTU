#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "taskqueue.h"
#include "task.h"
#include "stagebase.h"
#include "taskgeneratorstage.h"
#include "readerstage.h"
#include "parserstage.h"
#include "writerstage.h"
#include "accumulatorstage.h"
#include "pipeline.h"
#include "webscraper.h"
#include "databasehandler.h"
#include "logger.h"

const int ERROR_DATABASE_CONNECTION = 1;
const int ERROR_DATABASE_TABLECREATION = 2;

int main()
{
    // std::string homeUrl = "https://www.povarenok.ru/recipes/cat/";
    // WebScraperConfig config{.url = homeUrl, .maxSectionN = 10, .maxLinksPerSectionN = 15};
    // WebScraper s(config);
    // s.seriesScraping();

    DatabaseHandler dh;
    std::string dbStr = "host=localhost dbname=recipes user=postgres password=andresrmlnx15";
    if (!dh.connect(dbStr))
    {
        std::cout << "Not connected" << std::endl;
        return ERROR_DATABASE_CONNECTION;
    }
    if (!dh.createTasksTable())
    {
        std::cout << "Error creating tasks table" << std::endl;
        return ERROR_DATABASE_TABLECREATION;
    }
    TaskQueue dummyQueue;
    TaskQueue taskGeneratorOutputQueue;
    TaskQueue readerOutputQueue;
    TaskQueue parserOutputQueue;
    TaskQueue writerOutputQueue;
    TaskQueue accumulatorOutputQueue;

    Logger logger;
    std::string directoryPath = "/home/andres/Desktop/5Semester/AlgorithmsAnalysis/lab_05/ra22iu62/lab_05/src/recipes/";

    TaskGeneratorStage taskGeneratorStage(directoryPath, 1, logger);
    ReaderStage readerStage(1, logger);
    ParserStage parserStage(1, logger);
    WriterStage writerStage(dbStr, 1, logger);
    AccumulatorStage accumulatorStage(1, logger);

    std::vector<std::thread> pipelineThreads;

    pipelineThreads.emplace_back([&](){
        taskGeneratorStage.process(dummyQueue, taskGeneratorOutputQueue);
    });

    pipelineThreads.emplace_back([&](){
        readerStage.process(taskGeneratorOutputQueue, readerOutputQueue);
    });

    pipelineThreads.emplace_back([&](){
        parserStage.process(readerOutputQueue, parserOutputQueue);
    });

    pipelineThreads.emplace_back([&](){
        writerStage.process(parserOutputQueue, writerOutputQueue);
    });

    pipelineThreads.emplace_back([&](){
        accumulatorStage.process(writerOutputQueue, accumulatorOutputQueue);
    });

    for (auto& t : pipelineThreads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    
    std::vector<Task> taskVector = accumulatorOutputQueue.dequeueAllToVector();
    
    std::cout << std::endl;
    outputOverallStatistics(taskVector);
    std::cout << std::endl;

    taskGeneratorOutputQueue.shutdown();
    readerOutputQueue.shutdown();
    parserOutputQueue.shutdown();
    writerOutputQueue.shutdown();
    accumulatorOutputQueue.shutdown();

    logger.stop();

    return 0;
}
