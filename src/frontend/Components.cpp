#include "bTodo/frontend/Components.h"

#include <utility>

namespace bTodo::frontend::components {
ftxui::Component Window(const std::string& title, const ftxui::Component& component) {
  return ftxui::Renderer(
      component, [component, title] { return ftxui::window(ftxui::text(title), component->Render()) | ftxui::flex; });
}

class PhantomComponentBase : public ftxui::ComponentBase {
 public:
  PhantomComponentBase() = default;
  explicit PhantomComponentBase(ftxui::Element element);
  ftxui::Element Render() override;

 private:
  ftxui::Element element;
};

PhantomComponentBase::PhantomComponentBase(ftxui::Element element) : element(std::move(element)) {}

ftxui::Element PhantomComponentBase::Render() { return element; }

ftxui::Component PhantomComponent(ftxui::Element element) {
  return ftxui::Make<PhantomComponentBase>(std::move(element));
}

}  // namespace bTodo::frontend::components
