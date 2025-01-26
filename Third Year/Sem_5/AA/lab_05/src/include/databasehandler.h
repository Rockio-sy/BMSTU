#ifndef DATABASEHANLDER_H
#define DATABASEHANLDER_H

#include <sqlite3.h>
#include <iostream>
#include <string>
#include <mutex>
#include "task.h"
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

class DatabaseHandler
{
    public:
        DatabaseHandler() : conn(nullptr) {}

        ~DatabaseHandler()
        {
            close();
        }

        bool connect(const std::string &connStr);

        bool createTasksTable(void);

        bool insertTask(const Task &task, int issueId);

        void close();

    private:

        pqxx::connection *conn;
        std::mutex dbMutex;
};

#endif // DATABASEHANDLER_H