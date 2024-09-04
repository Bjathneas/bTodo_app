#include "bTodo/frontend/TaskModal.h"

#include <ftxui/component/component_base.hpp>
#include <sstream>
#include <algorithm>

#include "bTodo/frontend/Components.h"

namespace bTodo::frontend {
ftxui::Component TaskModal::createModal(std::string name) {
  for (int i = 1; i < 32; i++) {
    if (i <= 12) this->months.push_back(std::to_string(i));
    if (i >= 24) this->years.push_back("20" + std::to_string(i));
    this->days.push_back(std::to_string(i));
  }

  this->modal =
      ftxui::Container::Vertical(
          {ftxui::Container::Horizontal(
               {bTodo::frontend::components::PhantomComponent(ftxui::text(name)) | ftxui::vcenter,
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

ftxui::Component TaskModal::getModal() { return this->modal; }

bool TaskModal::isActive() { return this->is_active; }

void TaskModal::Open() { this->is_active = true; }

void TaskModal::Close() {
  this->task_name = "";
  this->task_description = "";
  this->is_active = false;
}

void TaskModal::Submit() {
  std::string due_date = months[month] + "/" + days[day] + "/" + years[year];
  this->task_cache.push_back(Task{task_name, task_description, due_date});
  this->Close();
}

bool TaskModal::hasCachedTasks() { return task_cache.size() > 0; }

std::vector<TaskModal::Task> &TaskModal::getTaskCache() {
    return task_cache;
}

void TaskModal::clearCache() {
    task_cache.clear();
}

void TaskModal::loadTask(TaskModal::Task &task) {
    this->task_name = task.task_name;
    this->task_description = task.task_description;

    //break down month day year
    std::vector<std::string> date;

    std::string date_str;

    std::stringstream due_date_ss(task.due_date);

    while(getline(due_date_ss, date_str,'/')) {
        date.push_back(date_str);
    }

    this->day =  std::find(this->days.begin(), this->days.end(), date[1]) - this->days.begin(); 
    this->month = std::find(this->months.begin(), this->months.end(), date[0]) - this->months.begin();
    this->year = std::find(this->years.begin(), this->years.end(), date[2]) - this->years.begin();
}
}  // namespace bTodo::frontend

