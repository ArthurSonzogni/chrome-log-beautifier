#include "ui/main_component.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/string.hpp>
#include <set>
#include "data/session.h"

using namespace ftxui;

std::mutex MainComponent::test_lock;
std::string MainComponent::test_data;

MainComponent::MainComponent(Session& session, Closure&& screen_exit)
    : m_screen_exit_(std::move(screen_exit)),
      log_displayer_1_(Make<LogDisplayer>(*this)),
      log_displayer_2_(Make<LogDisplayer>(*this)),
      m_session(session)
    {
  Add(Container::Vertical({
      toggle_,
      Container::Tab(
          {
              Container::Vertical({
                  Container::Horizontal({
                      container_search_selector_,
                      btn_search_,
                      m_btn_clear_
                  }),
                  m_test,
                  log_displayer_1_,
                  text_box_,
                  m_btn_copy_
              }),
              log_displayer_2_,
              info_component_,
          },
          &tab_selected_),
      btn_exit_
  }));
}

bool MainComponent::OnEvent(Event event) {
  if (event == Event::Special("fetch")) {
    std::lock_guard<std::mutex> lk(test_lock);
    m_text = test_data;
  }

  return ComponentBase::OnEvent(event);
}


Element MainComponent::Render() {
  static int i = 0;

  m_text = log_displayer_1_->GetSelected();

  int current_line =
      (std::min(tab_selected_, 1) == 0 ? log_displayer_1_ : log_displayer_2_)
          ->selected();

  auto header = hbox({
      text(L"Diffusion monitor"),
      separator(),
      hcenter(toggle_->Render()),
      separator(),
      btn_exit_->Render(),
      separator(),
      text(to_wstring(current_line)),
      text(L"/"),
      text(to_wstring(m_session.getFetchedTopics().size())),
      text(L"  ["),
      text(to_wstring(0)),
      text(L"]"),
      separator(),
      gauge(float(current_line) /
            float(std::max(1, (int)m_session.getFetchedTopics().size() - 1))) |
          color(Color::GrayDark),
      separator(),
      spinner(18, i++),
  });

  Element tab_menu;
  if (tab_selected_ == 0) {
    return  //
        vbox({
            header,
            separator(),
            window(text(L"Selector"), hbox(text("Enter path:"), separator(), container_search_selector_->Render(), btn_search_->Render(), m_btn_clear_->Render()) | notflex),
            m_test->Render(),

            /*hbox({
                window(text(L"Type"), container_level_filter_->Render()) |
                    notflex,
                text(L" "),
                window(text(L"Filter"), hbox(thread_filter_document)) | notflex,
                text(L" ")
                //window(text(L"Selector"), hbox(container_search_selector_->Render(), btn_search_->Render())) | flex,
                //filler(),
            }) | notflex,*/
            log_displayer_1_->RenderLines(m_session.getFetchedTopics()) | flex_shrink,
            window(text("Content"), hbox(text_box_->Render() | size(ftxui::HEIGHT, ftxui::EQUAL, 10) | xflex_grow, vbox(m_btn_copy_->Render())))
        });
  }

  std::vector<Topic> dummy;
  if (tab_selected_ == 1) {
    return  //
        vbox({
            header,
            log_displayer_2_->RenderLines(dummy) | flex_shrink,
            filler(),
        });
  }

  return  //
      vbox({
          header,
          separator(),
          filler(),
          info_component_->Render() | center,
          filler(),
      });
}
