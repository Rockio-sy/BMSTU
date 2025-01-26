#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <optional>
#include "task.h"

class TaskQueue
{
    public:
        TaskQueue() : shutdownFlag(false) {}

        bool enqueue(Task&& task);

        std::optional<Task> dequeue();

        std::vector<Task> dequeueAllToVector();

        bool isEmpty() const;

        bool isShutdown() const;

        void shutdown();

        int size() const;

    private:
        mutable std::mutex mtx;
        std::condition_variable cv;
        std::queue<Task> queue;
        bool shutdownFlag;
};

#endif // TASKQUEUE_H