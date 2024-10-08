#pragma once

#include <ftxui/component/component.hpp>

namespace bTodo::frontend {
class TodoMenu {
 public:
  TodoMenu() = default;

  ftxui::Component createMenu();
  void setDisplayedTaskInfo(std::string &info, std::string &date);

  void addTask(int &task_id, std::string &task_name);
  void setTasks(std::vector<std::pair<int, std::string>> &tasks);
  void removeTask(int &task_id);
  void clearTasks();

  int getSelectedTask();

  bool hasTaskSelectedChanged();

 private:
  // id, title
  std::vector<std::string> tasks;
  std::vector<int> task_ids;
  int task_selected{0};
  int task_selected_previously{-1};
  ftxui::Component information_container;
};
}  // namespace bTodo::frontend
