#include "bTodo/frontend/TodoMenu.h"

#include <ftxui/dom/elements.hpp>

#include "bTodo/frontend/Components.h"
#include "bTodo/frontend/scroller.hpp"

namespace bTodo::frontend {
ftxui::Component TodoMenu::createMenu() {
  auto task_container = ftxui::Container::Vertical({bTodo::frontend::components::Window(
      "Tasks", ftxui::Menu(&this->tasks, &this->task_selected) | ftxui::vscroll_indicator | ftxui::frame)});

  this->information_container = ftxui::Container::Vertical({bTodo::frontend::components::Window(
      "Info", ftxui::Scroller({bTodo::frontend::components::PhantomComponent(ftxui::vbox({
                  ftxui::paragraph("No Task is currently Selected"),
                  // ftxui::separator(),
                  // ftxui::text("Date Due: mm/dd/yyyy") | ftxui::align_right | ftxui::color(Color::Red)
              }))}))});
  return ftxui::Container::Horizontal({
             task_container,
             bTodo::frontend::components::PhantomComponent(ftxui::separatorEmpty()),
             this->information_container | ftxui::borderEmpty | ftxui::xflex_grow,
         }) |
         ftxui::flex_grow;
}

void TodoMenu::setDisplayedTaskInfo(std::string &info, std::string &date) {
  this->information_container->DetachAllChildren();
  this->information_container->Add(bTodo::frontend::components::Window(
      "Info", ftxui::Scroller({bTodo::frontend::components::PhantomComponent(ftxui::vbox(
                  {// ftxui::text(name),
                   // ftxui::separator,
                   ftxui::paragraph(info), ftxui::separator(),
                   ftxui::text("Date Due: " + date) | ftxui::align_right | ftxui::color(ftxui::Color::Red)}))})));
}

void TodoMenu::addTask(int &task_id, std::string &task_name) {
  this->tasks.push_back(task_name);
  this->task_ids.push_back(task_id);
}

void TodoMenu::setTasks(std::vector<std::pair<int, std::string>> &tasks) {
  this->tasks.clear();
  this->task_ids.clear();

  for (std::pair<int, std::string> &task : tasks) {
    this->tasks.push_back(task.second);
    this->task_ids.push_back(task.first);
  }
}

void TodoMenu::removeTask(int &task_id) {}

int TodoMenu::getSelectedTask() { return this->task_ids[this->task_selected]; }

bool TodoMenu::hasTaskSelectedChanged() {
  bool has_changed = this->task_selected != this->task_selected_previously;

  if (has_changed) this->task_selected_previously = this->task_selected;

  return has_changed;
}

}  // namespace bTodo::frontend

