#include "bTodo/backend/PreparedStatements.h"

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Statement.h>

#include <memory>

namespace bTodo::backend {
PreparedStatements::PreparedStatements(std::shared_ptr<SQLite::Database> &database) {
  this->insert_into_task_table = std::make_shared<SQLite::Statement>(*database.get(), bTODO_INSERT_INTO_TASK_TABLE);
  this->get_all_from_task_table = std::make_shared<SQLite::Statement>(*database.get(), bTODO_GET_ALL_TASK_TABLE);
  this->remove_task = std::make_shared<SQLite::Statement>(*database.get(), bTODO_REMOVE_TASK);
}
}  // namespace bTodo::backend

