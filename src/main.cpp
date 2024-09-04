#ifdef __unix__
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#else
#error This OS is not supported yet!
#endif

#include <cstdlib>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <filesystem>

#include "bTodo/frontend/Components.h"
#include "bTodo/frontend/TaskModal.h"
#include "bTodo/backend/DataBaseController.h"
#include "bTodo/frontend/TodoMenu.h"



// #define TEST_TASKS 64

using namespace ftxui;

const std::shared_ptr<bTodo::frontend::TodoMenu> todo_menu{std::make_shared<bTodo::frontend::TodoMenu>()};
const std::shared_ptr<bTodo::frontend::TaskModal> task_create_modal{
    std::make_shared<bTodo::frontend::TaskModal>()};
//TODO BREAK THIS SHIT DOWN SO IT CAN BE READABLE
int main() {
  auto screen = ScreenInteractive::Fullscreen();

  
  std::string homedir = getenv("HOME");

  if (homedir == "") {
    homedir = getpwuid(getuid())->pw_dir;
  }
  
  auto default_folder = std::filesystem::path(homedir + "/.bTodo");

  bTodo::backend::DataBaseController dbc(default_folder);

  auto db_tasks = dbc.getTaskNames();

  todo_menu->setTasks(db_tasks);

  auto search_menu = Container::Vertical({
      bTodo::frontend::components::PhantomComponent(text("Page Has Not Been Constructed") | bgcolor(Color::Red)),
  });

  auto settings_menu = Container::Vertical({
      bTodo::frontend::components::PhantomComponent(text("Page Has Not Been Constructed") | bgcolor(Color::Red)),
  });

  std::vector<std::string> menu_tab_names{
      "Todo",
      "Search",
      "Settings",
  };

  int menu_tab_selected = 0;

  auto menu_tab_toggle = Toggle(&menu_tab_names, &menu_tab_selected);

  auto menu_tab_container = Container::Tab(
      {
          // EACH MENU GOES HERE
          todo_menu->createMenu(),
          search_menu,
          settings_menu,
      },
      &menu_tab_selected);

  auto container = Container::Vertical({menu_tab_toggle, menu_tab_container});

  auto menu_renderer = Renderer(container, [&] {
    return vbox({
               menu_tab_toggle->Render(),
               separator(),
               menu_tab_container->Render(),
           }) |
           border;
  });

  int active_layer{0};

  auto application_container = Container::Tab({menu_renderer, task_create_modal->createModal("Create Task")}, &active_layer);

  auto renderer = Renderer(application_container, [&] {

    if(task_create_modal->hasCachedTasks()) {
      auto cached_tasks = task_create_modal->getTaskCache();

      for(bTodo::frontend::TaskModal::Task task : cached_tasks) {
        dbc.addTask(task.task_name, task.task_description, task.due_date);
      }

      task_create_modal->clearCache();
    }
    
    if (dbc.wasUpdated()) {
      auto db_tasks = dbc.getTaskNames();
      todo_menu->setTasks(db_tasks);
    }

    if (todo_menu->hasTaskSelectedChanged() || dbc.wasUpdated()) {
      std::string task_info;
      std::string date;

      if (todo_menu->getSelectedTask() == -1) {
        task_info = "Press \'c\' to create a new task";
        date = "yyyy-mm-dd";
      } else {
        // get the description and due date
        auto dbc_info = dbc.getTaskInfo(todo_menu->getSelectedTask());
        task_info = dbc_info.first;
        date = dbc_info.second;
      }
      todo_menu->setDisplayedTaskInfo(task_info, date);
    }

    dbc.resetUpdated();
    auto document = menu_renderer->Render();

    if (task_create_modal->isActive()) {
      active_layer = 1;
      document = dbox({document, task_create_modal->getModal()->Render() | clear_under | center});
    } else {
      active_layer = 0;
    }
    return document;
  });

  auto captureInput = CatchEvent(renderer, [&](const Event &event) {
    if (event == Event::Escape) {
      if (task_create_modal->isActive()) {
        task_create_modal->Close();
        return true;
      }
      screen.Clear();
      screen.ExitLoopClosure()();
      return true;
    }

    if(event == Event::End) {
      dbc.removeTask(todo_menu->getSelectedTask());
      return true;
    }

    if (event == Event::Character('c')) {
      task_create_modal->Open();
    }

    return false;
  });

  screen.Loop(captureInput);

  return EXIT_SUCCESS;
}

