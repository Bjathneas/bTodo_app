#include <cstdlib>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

#include "bTodo/frontend/Components.h"
#include "bTodo/frontend/TaskCreateModal.h"
#include "bTodo/frontend/TodoMenu.h"

using namespace ftxui;

const std::shared_ptr<bTodo::frontend::TodoMenu> todo_menu{std::make_shared<bTodo::frontend::TodoMenu>()};
const std::shared_ptr<bTodo::frontend::TaskCreateModal> task_create_modal{
    std::make_shared<bTodo::frontend::TaskCreateModal>()};

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  // todo_menu = std::make_shared<bTodo::frontend::TodoMenu>();
  std::string test_task = std::string("Test Task");
  for (int i = 0; i < 25; i++) {
    todo_menu->addTask(i, test_task);
  }

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

  auto application_container = Container::Tab({menu_renderer, task_create_modal->createModal()}, &active_layer);

  auto renderer = Renderer(application_container, [&] {
    // TODO Put this into a thread for performance with the db
    if (todo_menu->hasTaskSelectedChanged()) {
      std::string task_info{"Task Number Selected: " + std::to_string(todo_menu->getSelectedTask())};
      std::string date{"11/15/2004"};
      todo_menu->setDisplayedTaskInfo(task_info, date);
    }
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

    if (event == Event::Character('c')) {
      task_create_modal->Open();
    }

    return false;
  });

  screen.Loop(captureInput);

  return EXIT_SUCCESS;
}
