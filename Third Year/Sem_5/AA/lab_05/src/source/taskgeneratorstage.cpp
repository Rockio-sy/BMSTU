#include "taskgeneratorstage.h"

void TaskGeneratorStage::process(TaskQueue& inputQueue, TaskQueue& outputQueue)
{
    (void) inputQueue;
    auto paths = collectAllFilePaths(directoryPath);
    int n = paths.size();
    int perThread = (n + numThreads - 1) / numThreads;

    std::vector<std::thread> workers;
    for (int i = 0; i < numThreads; ++i)
    {
        int start = i * perThread;
        int end = std::min(start + perThread, n);
        workers.emplace_back([this, &outputQueue, &paths, start, end]() {
            this->generateTasks(outputQueue, paths, start, end);
        });
    }

    for (auto& worker : workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }

    outputQueue.shutdown();
}

void TaskGeneratorStage::shutdown()
{
}

void TaskGeneratorStage::generateTasks(TaskQueue& outputQueue, const std::vector<std::string>& paths, int start, int end)
{    
    for (int i = start; i < end; ++i)
    {
        Task task(paths[i]);
        logger.log("[" + formatTime(task.getCreationTime()) + "] Задача " +  std::to_string(task.getId()) + " создана.", "readerstage.txt");
        task.markStageEntry(PipelineStage::TaskGenerator);
        task.markStageExit(PipelineStage::TaskGenerator);
        task.markQueueEntry(TaskQueueID::TaskGeneratorOutputQueue);
        logger.log("[" + formatTime(task.getQueueEntryTime(TaskQueueID::TaskGeneratorOutputQueue)) + "] Задача " +  std::to_string(task.getId()) + " вошла в очеречь стадии чтения данных.", "readerstage.txt");
        outputQueue.enqueue(std::move(task));
    }
}

std::vector<std::string> TaskGeneratorStage::collectAllFilePaths(const std::string &directoryPath)
{
    std::vector<std::string> paths;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(directoryPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            paths.push_back(entry.path());
        }
    }
    return paths;
}
