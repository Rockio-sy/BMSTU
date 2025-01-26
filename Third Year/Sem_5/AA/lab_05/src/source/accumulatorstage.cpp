#include "accumulatorstage.h"

void AccumulatorStage::process(TaskQueue& inputQueue, TaskQueue& outputQueue)
{
    std::vector<std::thread> workers;
    for (int i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this, &inputQueue, &outputQueue]() {
            this->accumulateTasks(inputQueue, outputQueue);
        });
    }
    for (auto& worker : workers)
    {
        worker.join();
    }
    outputQueue.shutdown();
}

void AccumulatorStage::shutdown()
{
    return;
}

void AccumulatorStage::accumulateTasks(TaskQueue& inputQueue, TaskQueue& outputQueue)
{
    while (!inputQueue.isShutdown() || !inputQueue.isEmpty())
    {
        auto optTask = inputQueue.dequeue();
        if (optTask.has_value())
        {
            Task &task = optTask.value();

            task.markQueueExit(TaskQueueID::WriterOutputQueue);
            task.markStageEntry(PipelineStage::Accumulator);

            std::cout << std::endl;
            std::cout << "Задача " << task.getId() << ": успешно прошла все стадии обработки." << std::endl
                      << "[" << formatTime(task.getCreationTime()) << "] Задача создана." << std::endl
                      << "[" << formatTime(task.getQueueEntryTime(TaskQueueID::TaskGeneratorOutputQueue)) << "] Вошла в очеречь стадии чтения данных." << std::endl
                      << "[" << formatTime(task.getQueueEntryTime(TaskQueueID::ReaderOutputQueue)) << "] Вошла в очеречь стадии обработки данных." << std::endl
                      << "[" << formatTime(task.getQueueEntryTime(TaskQueueID::ParserOutputQueue)) << "] Вошла в очеречь стадии записи данных." << std::endl;
            
            std::cout << "[" << formatTime(task.getStageEntryTime(PipelineStage::Reader)) << "] Начала обрабатываться на стадии чтения данных." << std::endl
                      << "[" << formatTime(task.getStageEntryTime(PipelineStage::Parser)) << "] Начала обрабатываться на стадии обработки данных." <<std::endl
                      << "[" << formatTime(task.getStageEntryTime(PipelineStage::Writer)) << "] Начала обрабатываться на стадии записи данных." << std::endl;

            task.markStageExit(PipelineStage::Accumulator);
            task.markQueueEntry(TaskQueueID::AccumulatorOutputQueue);
            task.markDestruction();

            std::cout << "[" << formatTime(task.getDestructionTime()) << "] Задача уничтожена." << std::endl;
            logger.log("[" + formatTime(task.getDestructionTime()) + "] Задача " +  std::to_string(task.getId()) + " уничтожена.", "writerstage.txt");
            std::cout << std::endl;

            outputQueue.enqueue(std::move(task));
        }
    }
}
