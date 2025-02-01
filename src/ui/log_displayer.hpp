#ifndef UI_LOG_DISPLAYER_HPP
#define UI_LOG_DISPLAYER_HPP

#include <ftxui/component/component.hpp>
#include "data/session.h"

using namespace ftxui;

class LogDisplayer : public ComponentBase {
 public:
  LogDisplayer(ComponentBase& parent) : m_parent(parent) {

  }
  Element RenderLines(const std::vector<Topic>& topics);
  bool OnEvent(Event) override;
  int selected() { return selected_; }
  bool Focusable() const override {
    return true;
  }

  std::string GetSelected() {
    return m_seltext;
  }

 private:
  int selected_ = 0;
  int size = 0;
  std::string m_seltext;
  ComponentBase& m_parent;
};

#endif /* end of include guard: UI_LOG_DISPLAYER_HPP */
