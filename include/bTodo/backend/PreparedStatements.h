#pragma once

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Statement.h>
#define bTODO_CREATE_TASK_TABLE                                                                              \
  "CREATE TABLE IF NOT EXISTS Tasks( ID INTEGER PRIMARY KEY AUTOINCREMENT, TaskName varchar(255) NOT NULL, " \
  "TaskDescription varchar(1024) NOT NULL, CreationDate varchar(16) NOT NULL, DueDate varchar(16) NOT NULL)"
#define bTODO_INSERT_INTO_TASK_TABLE \
  "INSERT INTO Tasks (TaskName, TaskDescription, CreationDate, DueDate ) VALUES ( ?, ?, ?, ? )"
#define bTODO_GET_ALL_TASK_TABLE "SELECT * FROM Tasks"

namespace bTodo::backend {
class PreparedStatements {
 public:
  PreparedStatements() = default;
  PreparedStatements(std::shared_ptr<SQLite::Database> &database);

  std::shared_ptr<SQLite::Statement> insert_into_task_table;
  std::shared_ptr<SQLite::Statement> get_all_from_task_table;
};
void registerStatements(SQLite::Database database);
}  // namespace bTodo::backend
