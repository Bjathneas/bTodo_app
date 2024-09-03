#pragma once

#include <filesystem>
#include <vector>

#include "bTodo/backend/PreparedStatements.h"
namespace bTodo::backend {
class DataBaseController {
 public:
  DataBaseController() = default;
  DataBaseController(std::filesystem::path &database_folder);

  std::vector<std::pair<int, std::string>> getTaskNames();
  std::pair<std::string, std::string> getTaskInfo(int task_id);

  void addTask(std::string &task_name, std::string &task_description, std::string &due_date);

  void removeTask(int task_id);

  bool wasUpdated();
  void resetUpdated();

 private:
  std::shared_ptr<SQLite::Database> database;
  PreparedStatements sql_queries;
  bool updated{false};
};
}  // namespace bTodo::backend

