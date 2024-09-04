#pragma once

#include <filesystem>
#include <vector>

#include "bTodo/backend/PreparedStatements.h"
namespace bTodo::backend {
class DataBaseController {
 public:
   struct Task {
     int uid;
     std::string name;
     std::string description;
     std::string creation_date;
     std::string due_date;
   };
  DataBaseController() = default;
  DataBaseController(std::filesystem::path &database_folder);

  std::vector<Task> getAllTasks();
  Task getTask(int task_id);

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

