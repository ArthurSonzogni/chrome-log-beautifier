#include "parsed_line.hpp"
#include <regex>

// static
ParsedLine ParsedLine::Parse(std::wstring line) {
  ParsedLine out;
  {
    std::wregex regex(
        LR"(^\[(\d+):(\d+):(\d\d)(\d\d)\/(\d\d)(\d\d)(\d\d)\.(\d+):(\d+):(\w+):(.+)\((\d+)\)\] (.*)$)");
    std::wsmatch match;
    if (std::regex_match(line, match, regex)) {
      int i = 1;

      out.process_id = std::stoi(match[i++].str());
      out.thread_id = std::stoi(match[i++].str());
      out.month = std::stoi(match[i++].str());
      out.day = std::stoi(match[i++].str());
      out.hour = std::stoi(match[i++].str());
      out.minute = std::stoi(match[i++].str());
      out.seconds = std::stoi(match[i++].str());
      out.milliseconds = std::stoi(match[i++].str());
      out.unknown = match[i++].str();
      out.level = match[i++].str();
      out.file = match[i++].str();
      out.line = L"(" + match[i++].str() + L")";
      out.log = match[i++].str();

      out.type = ParsedLine::LOG;
      return out;
    }
  }

  {
    std::wregex regex(
        LR"(^\[(\d+):(\d+):(\d\d)(\d\d)\/(\d\d)(\d\d)(\d\d)\.(\d+):(\w+):(.+)\((\d+)\)\] (.*)$)");
    std::wsmatch match;
    if (std::regex_match(line, match, regex)) {
      int i = 1;

      out.process_id = std::stoi(match[i++].str());
      out.thread_id = std::stoi(match[i++].str());
      out.month = std::stoi(match[i++].str());
      out.day = std::stoi(match[i++].str());
      out.hour = std::stoi(match[i++].str());
      out.minute = std::stoi(match[i++].str());
      out.seconds = std::stoi(match[i++].str());
      out.milliseconds = std::stoi(match[i++].str());
      out.level = match[i++].str();
      out.file = match[i++].str();
      out.line = L"(" + match[i++].str() + L")";
      out.log = match[i++].str();

      out.type = ParsedLine::LOG;
      return out;
    }
  }

  {
    std::wregex regex(LR"(^\[(..........)\] (.*)$)");
    std::wsmatch match;
    if (std::regex_match(line, match, regex)) {
      out.level = L"GTEST";
      out.line = L"";
      out.log = L"[" + match[1].str() + L"]";
      match[2].str();

      out.type = ParsedLine::GTEST;
      return out;
    }
  }

  out.type = ParsedLine::UNKNOWN;
  out.level = L"UNKNOWN";
  out.log = line;
  return out;
}
