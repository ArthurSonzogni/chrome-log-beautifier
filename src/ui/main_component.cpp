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
      m_session(session) {
  Add(Container::Vertical({
      toggle_,
      Container::Tab(
          {
              Container::Vertical({
                  Container::Horizontal({
                      //container_level_filter_,
                     // container_thread_filter_,
                      container_search_selector_,
                      btn_search_
                  }),
                  log_displayer_1_,
                  text_box_,
                  m_btn_copy_,
                  m_btn_clear_
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


void MainComponent::RegisterLogLevel(const std::wstring& log_level) {
  if (level_checkbox.count(log_level))
    return;

  level_checkbox[log_level] = true;
  container_level_filter_->Add(
      Checkbox(log_level.c_str(), &level_checkbox[log_level]));
}

void MainComponent::RegisterThreadId(const std::wstring& thread_id) {
  int key = (int)thread_id[0];
  auto& column = thread_filters_[key];
  if (!column) {
    column = std::make_unique<ThreadFilter>();
    container_thread_filter_->Add(column->container);
    thread_filters_order_.push_back(key);
  }

  if (column->checkboxes.count(thread_id))
    return;

  column->checkboxes[thread_id] = true;
  column->container->Add(Checkbox(thread_id, &column->checkboxes[thread_id]));
}

Element MainComponent::Render() {
  static int i = 0;
  std::set<std::wstring> allowed_level;
  std::set<std::wstring> allowed_thread;
  for (auto& [level, state] : level_checkbox) {
    if (state)
      allowed_level.insert(level);
  }
  for (auto key : thread_filters_order_) {
    for (auto& [thread_id, state] : thread_filters_[key]->checkboxes) {
      if (state)
        allowed_thread.insert(thread_id);
    }
  }

  m_text = log_displayer_1_->GetSelected();

  Elements thread_filter_document;
  thread_filter_document.push_back(vbox({
    text(L"Process:") | bold,
    separator(),
    text(L"Thread:") | bold,
    filler()
  }));
  for (auto key : thread_filters_order_) {
    Elements checkboxes;
    wchar_t c = key;
    std::wstring c_str;
    c_str += c;
    checkboxes.push_back(text(c_str));
    checkboxes.push_back(separator());
    checkboxes.push_back(thread_filters_[key]->container->Render());
    thread_filter_document.push_back(separator());
    thread_filter_document.push_back(vbox(checkboxes));
  }

  int current_line =
      (std::min(tab_selected_, 1) == 0 ? log_displayer_1_ : log_displayer_2_)
          ->selected();

  auto header = hbox({
      text(L"Diffusion monitor"),
      separator(),
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
      spinner(5, i++),
  });

  Element tab_menu;
  if (tab_selected_ == 0) {
    return  //
        vbox({
            header,
            separator(),
            window(text(L"Selector"), hbox(text("Enter path:"), separator(), container_search_selector_->Render(), btn_search_->Render()) | notflex),

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
            window(text("Content"), hbox(text_box_->Render() | size(ftxui::HEIGHT, ftxui::EQUAL, 10) | xflex_grow, vbox(m_btn_copy_->Render(), m_btn_clear_->Render())))
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
