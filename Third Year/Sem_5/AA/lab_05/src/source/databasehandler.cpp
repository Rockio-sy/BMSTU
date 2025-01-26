#include "databasehandler.h"

bool DatabaseHandler::connect(const std::string &connStr)
{
    try
    {
        conn = new pqxx::connection(connStr);
        if (!conn->is_open())
        {
            std::cout << "failed to open database connection" << std::endl;
            return false;
        }
        return true;
    }
    catch(const std::exception& e)
    {
        std::cout << "connection failed: " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseHandler::createTasksTable(void)
{
    if (!conn || !conn->is_open())
    {
        std::cout << "connection not open" << std::endl;
        return false;
    }

    try
    {
        pqxx::work W(*conn);

        std::string sqlCreateTasks = 
            "CREATE TABLE IF NOT EXISTS tasks ("
            "id INT PRIMARY KEY, "
            "issue_id INTEGER, "
            "url TEXT, "
            "title TEXT, "
            "ingredients TEXT, "
            "instructions TEXT, "
            "image_url TEXT);";
        W.exec(sqlCreateTasks);
        W.commit();
        return true;
    }
    catch (const pqxx::sql_error &e)
    {
        std::cout << "SQL error: " << e.what() << std::endl
                  << "query was: " << e.query() << std::endl;
        return false;
    }
    catch(const std::exception& e)
    {
        std::cout << "error creating table: " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseHandler::insertTask(const Task &task, int issueId)
{
    std::lock_guard<std::mutex> lock(dbMutex);
    
    if (!conn || !conn->is_open())
    {
        std::cout << "connectio not open" << std::endl;
        return false;
    }

    nlohmann::json jsonIngredients;
    for (const auto & ingredient : task.getIngredients())
    {
        jsonIngredients.push_back({
            {"name", ingredient.getName()},
            {"unit", ingredient.getUnit()},
            {"quantity", ingredient.getQuantity()}
        });
    }
    nlohmann::json jsonInstructions;
    for (const auto &step : task.getInstructions())
    {
        jsonInstructions.push_back(step);
    }

    try
    {
        pqxx::work W(*conn);
        std::string sql = "INSERT INTO Tasks (id, issue_id, url, title, ingredients, instructions, image_url) "
                          "VALUES ($1, $2, $3, $4, $5, $6, $7);";
        
        W.exec_params(
            sql,
            task.getId(),
            issueId,
            task.getUrl(),
            task.getTitle(),
            jsonIngredients.dump(),
            jsonInstructions.dump(),
            task.getImageUrl()
        );
        W.commit();
        return true;
    }
    catch (const pqxx::sql_error& e)
    {
        std::cout << "sql error: task insertion: " << e.what() << std::endl << "query: " << e.query() << std::endl;
        return false;
    }
    catch (const std::exception& e)
    {
        std::cout << "insertTask: " << e.what() << std::endl;
        return false;
    }
}

void DatabaseHandler::close()
{
    if (conn)
    {
        conn->disconnect();
        delete conn;
        conn = nullptr;
    }
}