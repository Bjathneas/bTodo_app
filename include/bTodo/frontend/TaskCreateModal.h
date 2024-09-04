#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

//TODO Change This File into a Basic Modal

namespace bTodo::frontend {
class TaskCreateModal {
 public:
  struct Task {
   std::string task_name;
   std::string task_description;
   std::string due_date;
  };

  
  TaskCreateModal() = default;

  ftxui::Component createModal();

  ftxui::Component getModal();

  bool isActive();

  void Open();
  void Close();
  void Submit();

  bool hasCachedTasks();
  std::vector<Task> &getTaskCache();
  void clearCache();

 private:
  std::vector<Task> task_cache;
  bool is_active{false};
  ftxui::Component modal;

  std::string task_name;
  std::string task_description;
  int day{0};
  int month{0};
  int year{0};

  std::vector<std::string> days;
  std::vector<std::string> months;
  std::vector<std::string> years;
};
}  // namespace bTodo::frontend
