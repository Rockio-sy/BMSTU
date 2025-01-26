#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <map>
#include <iomanip>

enum class PipelineStage
{
    TaskGenerator,
    Reader,
    Parser,
    Writer,
    Accumulator
};

enum class TaskQueueID
{
    TaskGeneratorOutputQueue,
    ReaderOutputQueue,
    ParserOutputQueue,
    WriterOutputQueue,
    AccumulatorOutputQueue
};

class Ingredient
{
    public:
        Ingredient (const std::string &name_, const std::string &unit_, int quantity_)
        : name(name_), unit(unit_), quantity(quantity_) {}

        const std::string &getName() const;

        const std::string &getUnit() const;

        int getQuantity() const;

        void setName(const std::string &name_);

        void setUnit(const std::string &unit_);

        void setQuantity(int quantity_);

    private:
        std::string name;
        std::string unit;
        int quantity;
};

class Task
{
    public:

        static int globalId;

        Task(const std::string &filePath_) : filePath(filePath_), creationTime(std::chrono::system_clock::now())
        {
            id = ++globalId;
        }

        int getId() const;
        const std::string &getFilePath() const;
        const std::string &getContent() const;
        const std::string &getUrl() const;
        const std::string &getTitle() const;
        const std::vector<Ingredient> &getIngredients() const;
        const std::vector<std::string> &getInstructions() const;
        const std::string &getImageUrl() const;

        void setId(int id_);
        void setFilePath(const std::string &path_);
        void setContent(const std::string &content_);
        void setUrl(const std::string &url_);
        void setTitle(const std::string &title_);
        void setIngredients(const std::vector<Ingredient> &ingredients_);
        void setInstructions(const std::vector<std::string> &instructions_);
        void setImageUrl(const std::string &imageUrl_);
        void addIngredient(const Ingredient &ingredient_);


        std::chrono::system_clock::time_point getCreationTime() const;
        std::chrono::system_clock::time_point getQueueEntryTime(TaskQueueID queueID) const;
        std::chrono::system_clock::time_point getQueueExitTime(TaskQueueID queueID) const;
        std::chrono::system_clock::time_point getStageEntryTime(PipelineStage stageID) const;
        std::chrono::system_clock::time_point getStageExitTime(PipelineStage stageID) const;
        std::chrono::system_clock::time_point getDestructionTime() const;

        void markQueueEntry(TaskQueueID queueID);
        void markQueueExit(TaskQueueID queueID);
        void markStageEntry(PipelineStage stageID);
        void markStageExit(PipelineStage stageID);
        void markDestruction();

        long long getQueueDuration(TaskQueueID queueID) const;
        long long getStageDuration(PipelineStage stageID) const;
        long long getTaskLifeTime() const;

        friend std::ostream &operator<<(std::ostream &os, const Task &task);

    private:
        int id;
        std::string filePath;
        std::string content;
        std::string url;
        std::string title;
        std::vector<Ingredient> ingredients;
        std::vector<std::string> instructions;
        std::string imageUrl;

        struct Timestamp
        {
            std::chrono::system_clock::time_point entry;
            std::chrono::system_clock::time_point exit;
        };

        std::chrono::system_clock::time_point creationTime;
        std::chrono::system_clock::time_point destructionTime;

        std::map<TaskQueueID, Timestamp> queueTimes;
        std::map<PipelineStage, Timestamp> stageTimes;
};

std::string formatTime(const std::chrono::system_clock::time_point& timePoint);

std::string toStringPipelineStage(PipelineStage stage);

std::string toStringTaskQueueID(TaskQueueID queueID);

long long computeAverageTaskLifeTime(std::vector<Task> &taskVector);

std::map<TaskQueueID, long long> computeAverageQueueDurations(const std::vector<Task>& taskVector);

std::map<PipelineStage, long long> computeAverageStageDurations(const std::vector<Task>& taskVector);

void outputOverallStatistics(std::vector<Task> taskVector);

#endif // ASSIGNMENT_H