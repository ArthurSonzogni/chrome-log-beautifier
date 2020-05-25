#include "ui/main_component.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <set>

using namespace ftxui;

MainComponent::MainComponent(Receiver<std::wstring> receiver)
    : receiver_(std::move(receiver)) {
  Add(&container_main_);
  container_main_.Add(&container_filter_);
  container_filter_.Add(&container_level_filter_);
  container_filter_.Add(&container_thread_filter_);
  container_main_.Add(&log_displayer_);
}

bool MainComponent::OnEvent(Event event) {
  while (receiver_->HasPending()) {
    std::wstring line;
    receiver_->Receive(&line);
    AddLine(line);
  }
  return Component::OnEvent(event);
}

void MainComponent::AddLine(std::wstring line) {
  lines_.push_back(ParsedLine::Parse(line));
  ParsedLine& parsed_line = lines_.back();

  ComputeTranslatedThreadID(parsed_line);
  RegisterLogLevel(parsed_line.level);
  RegisterThreadId(parsed_line.translated_thread_id);
}

void MainComponent::ComputeTranslatedThreadID(ParsedLine& parsed_line) {
  if (parsed_line.process_id == 0) {
    parsed_line.translated_thread_id = L"None";
    return;
  }

  if (!translation_.count(parsed_line.process_id))
    translation_[parsed_line.process_id].first = U'A' + translation_.size();
  auto& translation_thread = translation_[parsed_line.process_id].second;
  if (!translation_thread.count(parsed_line.thread_id)) {
    translation_thread[parsed_line.thread_id] =
        to_wstring(translation_thread.size());
  }
  parsed_line.translated_thread_id =
      translation_[parsed_line.process_id].first +
      translation_thread[parsed_line.thread_id];
}

void MainComponent::RegisterLogLevel(const std::wstring& log_level) {
  auto& checkbox = level_checkbox[log_level];
  if (checkbox)
    return;

  checkbox = std::make_unique<CheckBox>();
  checkbox->label = log_level;
  checkbox->state = true;
  container_level_filter_.Add(checkbox.get());
}

void MainComponent::RegisterThreadId(const std::wstring& thread_id) {
  int key = (int)thread_id[0];
  auto& column = thread_filters_[key];
  if (!column) {
    column = std::make_unique<ThreadFilter>();
    container_thread_filter_.Add(&(column->container));
    thread_filters_order_.push_back(key);
  }

  auto& checkbox = column->checkboxes[thread_id];
  if (checkbox)
    return;

  checkbox = std::make_unique<CheckBox>();
  checkbox->label = thread_id;
  checkbox->state = true;
  column->container.Add(checkbox.get());
}

Element MainComponent::Render() {
  static int i = 0;
  std::set<std::wstring> allowed_level;
  std::set<std::wstring> allowed_thread;
  for (auto& [level, checkbox] : level_checkbox) {
    if (checkbox->state)
      allowed_level.insert(level);
  }
  for (auto key : thread_filters_order_) {
    for (auto& [id, checkbox] : thread_filters_[key]->checkboxes) {
      if (checkbox->state)
        allowed_thread.insert(id);
    }
  }
  std::vector<ParsedLine> filtered_lines;

  for (auto& it : lines_) {
    if (allowed_level.count(it.level) &&
        allowed_thread.count(it.translated_thread_id)) {
      filtered_lines.push_back(it);
    }
  }

  Elements thread_filter_document;
  bool is_first = true;
  for (auto key : thread_filters_order_) {
    Elements checkboxes;
    for (auto& [id, checkbox] : thread_filters_[key]->checkboxes)
      checkboxes.push_back(checkbox->Render());
    if (!is_first) {
      thread_filter_document.push_back(text(L" "));
      thread_filter_document.push_back(separator());
    }
    is_first = false;
    thread_filter_document.push_back(vbox(checkboxes));
  }

  return  //
      vbox({
          hbox({
              window(text(L" Log level"), container_level_filter_.Render()) |
                  notflex,
              text(L" "),
              window(text(L" Process/Thread"), hbox(thread_filter_document)) |
                  notflex,
              filler(),
              spinner(5, i++),
          }),
          log_displayer_.Render(filtered_lines) | notflex,
          filler(),
      });
}
