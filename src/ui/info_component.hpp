#ifndef UI_INFO_COMPONENT_HPP
#define UI_INFO_COMPONENT_HPP

#include <ftxui/component/component.hpp>

using namespace ftxui;

class InfoComponent : public Component {
 public:
  InfoComponent() = default;
  Element Render() override;
};

#endif /* end of include guard: UI_INFO_COMPONENT_HPP */
