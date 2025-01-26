#include "readerstage.h"

void ReaderStage::process(TaskQueue& inputQueue, TaskQueue& outputQueue)
{
    std::vector<std::thread> workers;
    for (int i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this, &inputQueue, &outputQueue]() {
            this->readTasks(inputQueue, outputQueue);
        });
    }
    for (auto &worker : workers)
    {
        worker.join();
    }
    outputQueue.shutdown();
}

void ReaderStage::shutdown()
{
    return;
}

void ReaderStage::readTasks(TaskQueue &inputQueue, TaskQueue &outputQueue)
{
    while (!inputQueue.isShutdown() || !inputQueue.isEmpty())
    {
        auto optTask = inputQueue.dequeue();
        if (optTask.has_value())
        {
            optTask->markQueueExit(TaskQueueID::TaskGeneratorOutputQueue);
            optTask->markStageEntry(PipelineStage::Reader);
            logger.log("[" + formatTime(optTask->getStageEntryTime(PipelineStage::Reader)) + "] Задача " +  std::to_string(optTask->getId()) + " начала обрабатываться на стадии чтения данных.", "readerstage.txt");

            std::ifstream fileStream(optTask->getFilePath());
            if (fileStream)
            {
                std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
                optTask->setContent(content);

                optTask->markStageExit(PipelineStage::Reader);
                optTask->markQueueEntry(TaskQueueID::ReaderOutputQueue);

                logger.log("[" + formatTime(optTask->getQueueEntryTime(TaskQueueID::ReaderOutputQueue)) + "] Задача " +  std::to_string(optTask->getId()) + " вошла в очеречь стадии обработки данных.", "parserstage.txt");
                outputQueue.enqueue(std::move(*optTask));
            }
            else
            {
                optTask->markStageExit(PipelineStage::Reader);
            }
        }
        else if (inputQueue.isShutdown())
        {
            break;
        }
    }
}
