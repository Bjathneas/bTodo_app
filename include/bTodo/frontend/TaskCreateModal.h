#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

#include "bTodo/backend/DataBaseController.h"

namespace bTodo::frontend {
class TaskCreateModal {
 public:
  TaskCreateModal() = default;

  ftxui::Component createModal(bTodo::backend::DataBaseController &dbc);

  ftxui::Component getModal();

  bool isActive();

  void Open();
  void Close();
  void Submit();

 private:
  bTodo::backend::DataBaseController *dbc;
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
