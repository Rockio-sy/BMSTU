#include "writerstage.h"

void WriterStage::process(TaskQueue& inputQueue, TaskQueue& outputQueue)
{
    std::vector<std::thread> workers;
    for (int i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this, &inputQueue, &outputQueue]() {
            this->writeTasks(inputQueue, outputQueue);
        });
    }
    for (auto& worker : workers)
    {
        worker.join();
    }
    outputQueue.shutdown();
}

void WriterStage::shutdown()
{
}

void WriterStage::writeTasks(TaskQueue& inputQueue, TaskQueue& outputQueue)
{
    DatabaseHandler dbHandler;
    if (!dbHandler.connect(dbConnStr))
    {
        std::cerr << "Failed to connect to the database" << std::endl;
        return;
    }

    while (!inputQueue.isShutdown() || !inputQueue.isEmpty())
    {
        auto optTask = inputQueue.dequeue();
        if (optTask.has_value())
        {
            Task task = std::move(optTask.value());

            task.markQueueExit(TaskQueueID::ParserOutputQueue);
            task.markStageEntry(PipelineStage::Writer);
            logger.log("[" + formatTime(task.getStageEntryTime(PipelineStage::Writer)) + "] Задача " +  std::to_string(task.getId()) + " начала обрабатываться на стадии записи данных.", "writerstage.txt");

            if (dbHandler.insertTask(task, ISSUE_ID))
            {
                task.markStageExit(PipelineStage::Writer);
                task.markQueueEntry(TaskQueueID::WriterOutputQueue);
                
                outputQueue.enqueue(std::move(task));
            }
        }
    }
    dbHandler.close();
}
