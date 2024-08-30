#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

namespace bTodo::frontend::components {

ftxui::Component Window(const std::string& title, const ftxui::Component& component);

ftxui::Component PhantomComponent(ftxui::Element element);

}  // namespace bTodo::frontend::components
