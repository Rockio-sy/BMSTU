#include "parserstage.h"

void ParserStage::process(TaskQueue& inputQueue, TaskQueue& outputQueue)
{
    std::vector<std::thread> workers;
    for (int i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this, &inputQueue, &outputQueue]() {
            this->parseTasks(inputQueue, outputQueue);
        });
    }
    for (auto& worker : workers)
    {
        worker.join();
    }
    outputQueue.shutdown();
}

void ParserStage::shutdown()
{
}

void ParserStage::parseTasks(TaskQueue& inputQueue, TaskQueue& outputQueue)
{
    while (!inputQueue.isShutdown() || !inputQueue.isEmpty())
    {
        auto optTask = inputQueue.dequeue();
        if (optTask.has_value())
        {
            Task task = std::move(optTask.value());

            task.markQueueExit(TaskQueueID::ReaderOutputQueue);
            task.markStageEntry(PipelineStage::Parser);
            logger.log("[" + formatTime(task.getStageEntryTime(PipelineStage::Parser)) + "] Задача " +  std::to_string(task.getId()) + " начала обрабатываться на стадии обработки данных.", "parserstage.txt");

            parseTask(task);

            task.markStageExit(PipelineStage::Parser);
            task.markQueueEntry(TaskQueueID::ParserOutputQueue);
            logger.log("[" + formatTime(task.getQueueEntryTime(TaskQueueID::ParserOutputQueue)) + "] Задача " +  std::to_string(task.getId()) + " вошла в очеречь стадии записи данных.", "writerstage.txt");

            outputQueue.enqueue(std::move(task));
        }
    }
}

void ParserStage::parseTask(Task &task)
{
    std::istringstream stream(task.getContent());
    std::string line;
    std::vector<Ingredient> ingredients;
    std::vector<std::string> instructions;

    while (std::getline(stream, line))
    {
        if (line.starts_with("url: "))
        {
            task.setUrl(line.substr(5).erase(0, line.substr(5).find_first_not_of(" \n\r\t")));
        }
        else if (line.starts_with("title: "))
        {
            task.setTitle(line.substr(7).erase(0, line.substr(7).find_first_not_of(" \n\r\t")));
        }
        else if (line.starts_with("ingredient: "))
        {
            Ingredient ingredient = parseIngredient(line);
            ingredients.push_back(ingredient);
        }
        else if (line.starts_with("instruction: "))
        {
            instructions.push_back(line.substr(13).erase(0, line.substr(13).find_first_not_of(" \n\r\t")));
        }
        else if (line.starts_with("image: "))
        {
            task.setImageUrl(line.substr(7).erase(0, line.substr(7).find_first_not_of(" \n\r\t")));
        }
    }
    task.setIngredients(ingredients);
    task.setInstructions(instructions);
}

Ingredient ParserStage::parseIngredient(const std::string& line)
{
    const std::string dashUtf8 = "\xE2\x80\x94";
    size_t dashPos = line.find(dashUtf8);
    std::string name = line.substr(12, dashPos - 12);
    std::string rest = line.substr(dashPos + dashUtf8.length());
    if (dashPos == std::string::npos)
    {
        return Ingredient(name, "", 0);
    }

    std::istringstream restStream(rest);
    std::string quantityStr, unit;
    restStream >> quantityStr >> unit;

    int quantity = 0;
    try
    {
        quantity = std::stoi(quantityStr);
    }
    catch (const std::exception& e)
    {
        return Ingredient(name, unit, 0);
    }
    return Ingredient(name, unit, quantity);
}
