#include "task.h"

// Ingredient

const std::string &Ingredient::getName() const
{
    return name;
}

const std::string &Ingredient::getUnit() const
{
    return unit;
}

int Ingredient::getQuantity() const
{
    return quantity;
}

void Ingredient::setName(const std::string &name_)
{
    name = name_;
}

void Ingredient::setUnit(const std::string &unit_)
{
    unit = unit_;
}

void Ingredient::setQuantity(int quantity_)
{
    quantity = quantity_;
}

// Task

int Task::globalId = 0;

int Task::getId() const
{
    return id;
}

const std::string &Task::getFilePath() const
{
    return filePath;
}

const std::string &Task::getContent() const
{
    return content;
}

const std::string &Task::getUrl() const
{
    return url;
}

const std::string &Task::getTitle() const
{
    return title;
}

const std::vector<Ingredient> &Task::getIngredients() const
{
    return ingredients;
}

const std::vector<std::string> &Task::getInstructions() const
{
    return instructions;
}

const std::string &Task::getImageUrl() const
{
    return imageUrl;
}

void Task::setId(int id_)
{
    id = id_;
}

void Task::setFilePath(const std::string &path_)
{
    filePath = path_;
}

void Task::setContent(const std::string &content_)
{
    content = content_;
}

void Task::setUrl(const std::string &url_)
{
    url = url_;
}

void Task::setTitle(const std::string &title_)
{
    title = title_;
}

void Task::setIngredients(const std::vector<Ingredient> &ingredients_)
{
    ingredients = ingredients_;
}

void Task::setInstructions(const std::vector<std::string> &instructions_)
{
    instructions = instructions_;
}

void Task::setImageUrl(const std::string &imageUrl_)
{
    imageUrl = imageUrl_;
}

void Task::addIngredient(const Ingredient &ingredient_)
{
    ingredients.push_back(ingredient_);
}

std::chrono::system_clock::time_point Task::getCreationTime() const
{
    return creationTime;
}

std::chrono::system_clock::time_point Task::getQueueEntryTime(TaskQueueID queueID) const
{
    auto it = queueTimes.find(queueID);
    if (it != queueTimes.end())
    {
        return it->second.entry;
    }
    return {};
}

std::chrono::system_clock::time_point Task::getQueueExitTime(TaskQueueID queueID) const
{
    auto it = queueTimes.find(queueID);
    if (it != queueTimes.end())
    {
        return it->second.exit;
    }
    return {};
}

std::chrono::system_clock::time_point Task::getStageEntryTime(PipelineStage stageID) const
{
    auto it = stageTimes.find(stageID);
    if (it != stageTimes.end())
    {
        return it->second.entry;
    }
    return {};
}

std::chrono::system_clock::time_point Task::getStageExitTime(PipelineStage stageID) const
{
    auto it = stageTimes.find(stageID);
    if (it != stageTimes.end())
    {
        return it->second.exit;
    }
    return {};
}

std::chrono::system_clock::time_point Task::getDestructionTime() const
{
    return destructionTime;
}

void Task::markQueueEntry(TaskQueueID queueID)
{
    queueTimes[queueID].entry = std::chrono::system_clock::now();
}

void Task::markQueueExit(TaskQueueID queueID)
{
    queueTimes[queueID].exit = std::chrono::system_clock::now();
}

void Task::markStageEntry(PipelineStage stageID)
{
    stageTimes[stageID].entry = std::chrono::system_clock::now();
}

void Task::markStageExit(PipelineStage stageID)
{
    stageTimes[stageID].exit = std::chrono::system_clock::now();
}

void Task::markDestruction()
{
    destructionTime = queueTimes[TaskQueueID::AccumulatorOutputQueue].entry;
}

long long Task::getQueueDuration(TaskQueueID queueID) const
{
    auto it = queueTimes.find(queueID);
    if (it != queueTimes.end() && it->second.entry.time_since_epoch().count() > 0)
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(it->second.exit - it->second.entry).count();
    }
    return 0;
}

long long Task::getStageDuration(PipelineStage stageID) const
{
    auto it = stageTimes.find(stageID);
    if (it != stageTimes.end() && it->second.exit.time_since_epoch().count() > 0)
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(it->second.exit - it->second.entry).count();
    }
    return 0;
}

long long Task::getTaskLifeTime() const
{
    if (destructionTime.time_since_epoch().count() > 0)
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(destructionTime - creationTime).count();
    }
    return 0;
}

std::ostream &operator<<(std::ostream &os, const Task &task)
{
    os << "Task ID: " << task.id << std::endl
       << "File Path: " << task.filePath << std::endl
       << "URL: " << task.url << std::endl
       << "Title: " << task.title << std::endl
       << "Ingredients:" << std::endl;
    
    for (unsigned long int i = 0; i < task.ingredients.size(); i++)
    {
        Ingredient ingredient = task.ingredients[i];
        os << i + 1 << ". " << "{'name': " << ingredient.getName() << ", 'unit': " << ingredient.getUnit() << ", 'quantity': " << ingredient.getQuantity() << "}" << std::endl;
    }
    os << "Instructions:" << std::endl;
    for (unsigned long int i = 0; i < task.instructions.size(); i++)
    {
        std::string instruction = task.instructions[i];
        os << i + 1 << ". " << instruction << std::endl;
    }
    os << "Image URL: " << task.url << std::endl;
    os << "Entered " << toStringTaskQueueID(TaskQueueID::TaskGeneratorOutputQueue) << " at: " << formatTime(task.getQueueEntryTime(TaskQueueID::TaskGeneratorOutputQueue)) << std::endl;
    os << "Entered " << toStringTaskQueueID(TaskQueueID::ReaderOutputQueue) << " at: " << formatTime(task.getQueueEntryTime(TaskQueueID::ReaderOutputQueue)) << std::endl;
    os << "Entered " << toStringTaskQueueID(TaskQueueID::ParserOutputQueue) << " at: " << formatTime(task.getQueueEntryTime(TaskQueueID::ParserOutputQueue)) << std::endl;
    os << "Entered " << toStringTaskQueueID(TaskQueueID::WriterOutputQueue) << " at: " << formatTime(task.getQueueEntryTime(TaskQueueID::WriterOutputQueue)) << std::endl;
    os << "Entered " << toStringTaskQueueID(TaskQueueID::AccumulatorOutputQueue) << " at: " << formatTime(task.getQueueEntryTime(TaskQueueID::AccumulatorOutputQueue)) << std::endl;
    return os;
}

std::string formatTime(const std::chrono::system_clock::time_point &timePoint)
{
    auto tt = std::chrono::system_clock::to_time_t(timePoint);
    auto tm = *std::localtime(&tt);
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(timePoint.time_since_epoch()) % 1000000;
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(6) << us.count();

    return ss.str();
}

std::string toStringPipelineStage(PipelineStage stage)
{
    switch (stage)
    {
        case PipelineStage::TaskGenerator:
            return "'стадия создания задач'";
        case PipelineStage::Reader:
            return "'стадия чтения данных'";
        case PipelineStage::Parser:
            return "'стадия обработки данных'";
        case PipelineStage::Writer:
            return "'стадия записи данных'";
        case PipelineStage::Accumulator:
            return "'стадия накопления задач'";
        default:
            return "Unknown Stage";
    }
}

std::string toStringTaskQueueID(TaskQueueID queueID)
{
    switch (queueID)
    {
        case TaskQueueID::TaskGeneratorOutputQueue:
            return "'очередь для чтения данных'";
        case TaskQueueID::ReaderOutputQueue:
            return "'очередь для обработки данных'";
        case TaskQueueID::ParserOutputQueue:
            return "'очередь для записи данных'";
        case TaskQueueID::WriterOutputQueue:
            return "'очередь для накопления задач'";
        case TaskQueueID::AccumulatorOutputQueue:
            return "'очередь для уничтожения задач'";
        default:
            return "Unknown queue ID";
    }
}

long long computeAverageTaskLifeTime(std::vector<Task> &taskVector)
{
    int n = taskVector.size();
    if (!n)
    {
        return 0;
    }
    long long sum = 0;
    for (auto &task : taskVector)
    {
        sum += task.getTaskLifeTime();
    }
    long long average = sum / n;
    return average;
}

std::map<TaskQueueID, long long> computeAverageQueueDurations(const std::vector<Task>& taskVector)
{
    std::map<TaskQueueID, long long> totalQueueDurations;
    std::map<TaskQueueID, int> counts;

    for (const auto& queueID : {TaskQueueID::TaskGeneratorOutputQueue, TaskQueueID::ReaderOutputQueue, TaskQueueID::ParserOutputQueue})
    {
        totalQueueDurations[queueID] = 0;
        counts[queueID] = 0;
    }

    for (const Task& task : taskVector)
    {
        for (const auto& queue : totalQueueDurations)
        {
            long long duration = task.getQueueDuration(queue.first);
            totalQueueDurations[queue.first] += duration;
            counts[queue.first]++;
        }
    }

    std::map<TaskQueueID, long long> averageQueueDurations;
    for (const auto& queue : totalQueueDurations)
    {
        TaskQueueID queueID = queue.first;
        if (counts[queueID] > 0) 
        {
            averageQueueDurations[queueID] = queue.second / counts[queueID];
        }
        else
        {
            averageQueueDurations[queueID] = 0;
        }
    }
    return averageQueueDurations;
}

std::map<PipelineStage, long long> computeAverageStageDurations(const std::vector<Task>& taskVector)
{
    std::map<PipelineStage, long long> totalStageDurations;
    std::map<PipelineStage, int> counts;

    for (const auto& pipelineStage : {PipelineStage::Reader, PipelineStage::Parser, PipelineStage::Writer})
    {
        totalStageDurations[pipelineStage] = 0;
        counts[pipelineStage] = 0;
    }

    for (const Task& task : taskVector)
    {
        for (const auto& stage : totalStageDurations)
        {
            long long duration = task.getStageDuration(stage.first);
            totalStageDurations[stage.first] += duration;
            counts[stage.first]++;
        }
    }

    std::map<PipelineStage, long long> averageStageDurations;
    for (const auto& stage : totalStageDurations)
    {
        PipelineStage stageID = stage.first;
        if (counts[stageID] > 0) 
        {
            averageStageDurations[stageID] = stage.second / counts[stageID];
        }
        else
        {
            averageStageDurations[stageID] = 0;
        }
    }
    return averageStageDurations;
}

void outputOverallStatistics(std::vector<Task> taskVector)
{
    long long averageTaskLifeTime = computeAverageTaskLifeTime(taskVector);
    std::map<TaskQueueID, long long> averageQueueDurations = computeAverageQueueDurations(taskVector);
    std::map<PipelineStage, long long> averageStageDurations = computeAverageStageDurations(taskVector);

    std::cout << "Результаты исследования: " << std::endl;
    std::cout << "Среднее время существования задачи: " << averageTaskLifeTime << " микросекунды." << std::endl;
    for (auto &queue : averageQueueDurations)
    {
        std::cout << "Среднее время ожидания задачи в очереди " << toStringTaskQueueID(queue.first) << ": " << queue.second << " микросекунды." << std::endl;
    }
    for (auto &stage : averageStageDurations)
    {
        std::cout << "Среднее время обработки задачи на стадии " << toStringPipelineStage(stage.first) << ": " << stage.second << " микросекунды." << std::endl;
    }
}
