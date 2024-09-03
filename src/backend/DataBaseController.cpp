#include "bTodo/backend/DataBaseController.h"

#include <SQLiteCpp/Database.h>

#include <cstddef>
#include <ctime>
#include <filesystem>
#include <string>

#include "bTodo/backend/PreparedStatements.h"

namespace bTodo::backend {
DataBaseController::DataBaseController(std::filesystem::path &database_folder) {
  if (!std::filesystem::exists(database_folder)) {
    std::filesystem::create_directory(database_folder);
  }

  database = std::make_shared<SQLite::Database>(database_folder.string() + "/tasks.db3",
                                                SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

  database->exec(bTODO_CREATE_TASK_TABLE);

  sql_queries = PreparedStatements(database);
}

std::vector<std::pair<int, std::string>> DataBaseController::getTaskNames() {
  std::vector<std::pair<int, std::string>> task_names;

  while (sql_queries.get_all_from_task_table->executeStep()) {
    int task_id = sql_queries.get_all_from_task_table->getColumn(0);
    std::string task_name = sql_queries.get_all_from_task_table->getColumn(1);

    std::pair<int, std::string> row(task_id, task_name);

    task_names.emplace_back(row);
  }

  sql_queries.get_all_from_task_table->reset();

  if (task_names.size() > 0) {
    return task_names;
  }

  return {{-1, "NO TASKS"}};
}

std::pair<std::string, std::string> DataBaseController::getTaskInfo(int task_id) {
  std::pair<std::string, std::string> task_info{"FAILED TO FIND TASK INFO", "mm/dd/yyyy"};

  bool found{false};

  while (sql_queries.get_all_from_task_table->executeStep() && !found) {
    int task_id_from_query = sql_queries.get_all_from_task_table->getColumn(0);
    if (task_id_from_query == task_id) {
      task_info = {sql_queries.get_all_from_task_table->getColumn(2),
                   sql_queries.get_all_from_task_table->getColumn(4)};
      found = true;  // because ive seen break fail in this situation
    }
  }

  sql_queries.get_all_from_task_table->reset();
  return task_info;
}

void DataBaseController::addTask(std::string &task_name, std::string &task_description, std::string &due_date) {
  auto query = this->sql_queries.insert_into_task_table;

  query->bind(1, task_name);
  query->bind(2, task_description);

  time_t current_time = time(nullptr);
  struct tm date_time = *localtime(&current_time);

  query->bind(3, std::to_string(date_time.tm_mon) + "/" + std::to_string(date_time.tm_mday) + "/" +
                     std::to_string(date_time.tm_year));

  query->bind(4, due_date);

  query->exec();
  query->reset();

  this->updated = true;
}

bool DataBaseController::wasUpdated() { return this->updated; }

void DataBaseController::resetUpdated() { this->updated = false; }
}  // namespace bTodo::backend
