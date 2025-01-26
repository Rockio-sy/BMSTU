#include "taskqueue.h"

bool TaskQueue::enqueue(Task&& task)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (shutdownFlag)
    {
        std::cerr << "Attempt to enqueue on a shutdown TaskQueue -> task will not be processed." << std::endl;
        return false;
    }
    queue.push(std::move(task));
    cv.notify_one();
    return true;
}

std::optional<Task> TaskQueue::dequeue()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return !queue.empty() || shutdownFlag; });
    if (queue.empty())
    {
        return {};
    }
    Task task = std::move(queue.front());
    queue.pop();
    return task;
}

std::vector<Task> TaskQueue::dequeueAllToVector()
{
    std::vector<Task> tasks;
    std::unique_lock<std::mutex> lock(mtx);
    while (!queue.empty())
    {
        tasks.push_back(std::move(queue.front()));
        queue.pop();
    }
    return tasks;
}

bool TaskQueue::isEmpty() const
{
    std::lock_guard<std::mutex> lock(mtx);
    return queue.empty();
}

bool TaskQueue::isShutdown() const
{
    std::lock_guard<std::mutex> lock(mtx);
    return shutdownFlag;
}

void TaskQueue::shutdown()
{
    std::lock_guard<std::mutex> lock(mtx);
    shutdownFlag = true;
    cv.notify_all();
}

int TaskQueue::size() const
{
    std::lock_guard<std::mutex> lock(mtx);
    return queue.size();
}
