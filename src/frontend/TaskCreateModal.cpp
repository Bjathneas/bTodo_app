#include "bTodo/frontend/TaskCreateModal.h"

#include <ftxui/component/component_base.hpp>

#include "bTodo/frontend/Components.h"

namespace bTodo::frontend {
ftxui::Component TaskCreateModal::createModal(bTodo::backend::DataBaseController &dbc) {
  this->dbc = &dbc;
  for (int i = 1; i < 32; i++) {
    if (i <= 12) this->months.push_back(std::to_string(i));
    if (i >= 24) this->years.push_back("20" + std::to_string(i));
    this->days.push_back(std::to_string(i));
  }

  this->modal =
      ftxui::Container::Vertical(
          {ftxui::Container::Horizontal(
               {bTodo::frontend::components::PhantomComponent(ftxui::text("Create Task")) | ftxui::vcenter,
                bTodo::frontend::components::PhantomComponent(ftxui::filler()),
                ftxui::Button("X", [&]() { this->Close(); }) | ftxui::align_right}),
           bTodo::frontend::components::PhantomComponent(ftxui::separatorDashed()),
           ftxui::Container::Horizontal(
               {bTodo::frontend::components::PhantomComponent(ftxui::text("Task Name: ")) | ftxui::vcenter,
                ftxui::Input(&this->task_name, "Task Name") | ftxui::bgcolor(ftxui::Color::Grey42) |
                    ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 1)}),
           ftxui::Container::Horizontal(
               {bTodo::frontend::components::PhantomComponent(ftxui::text("Task Description: ")) | ftxui::vcenter,
                ftxui::Input(&this->task_description, "Task Description") | ftxui::bgcolor(ftxui::Color::Grey42) |
                    ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 10)}),
           ftxui::Container::Horizontal(
               {bTodo::frontend::components::PhantomComponent(ftxui::text("Due Date: ")) | ftxui::vcenter,
                ftxui::Dropdown(&this->months, &this->month),
                bTodo::frontend::components::PhantomComponent(ftxui::text("/")) | ftxui::vcenter,
                ftxui::Dropdown(&this->days, &this->day),
                bTodo::frontend::components::PhantomComponent(ftxui::text("/")) | ftxui::vcenter,
                ftxui::Dropdown(&this->years, &this->year)}),
           ftxui::Button("Submit", [&]() { this->Submit(); }) | ftxui::align_right}) |
      ftxui::border;

  return this->modal;
}

ftxui::Component TaskCreateModal::getModal() { return this->modal; }

bool TaskCreateModal::isActive() { return this->is_active; }

void TaskCreateModal::Open() { this->is_active = true; }

void TaskCreateModal::Close() {
  this->task_name = "";
  this->task_description = "";
  this->is_active = false;
}
// TODO BACKEND: Implement Creation of Tasks
void TaskCreateModal::Submit() {
  std::string due_date = months[month] + "/" + days[day] + "/" + years[year];
  this->dbc->addTask(task_name, task_description, due_date);
  this->Close();
}

}  // namespace bTodo::frontend
