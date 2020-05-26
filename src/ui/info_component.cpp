#include "ui/info_component.hpp"

Element InfoComponent::Render() {
  auto document = vbox({
      hbox({
          text(L"Website            ") | bold,
          separator(),
          text(L"https://github.com/ArthurSonzogni/chrome-log-beautifier"),
      }),
      separator(),
      hbox({
          text(L"Build date         ") | bold,
          separator(),
          text(L"@today@"),
      }),
      separator(),
      hbox({
          text(L"Git Commit hash    ") | bold,
          separator(),
          text(L"@git_hash@"),
      }),
      separator(),
      hbox({
          text(L"Git Commit position") | bold,
          separator(),
          text(L"@git_position@"),
      }),
  });

  if (Focused())
    document = document | inverted;

  return document | border;
}
