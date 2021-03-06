#ifndef DATA_PARSED_LINE_HPP
#define DATA_PARSED_LINE_HPP

#include <string>

struct ParsedLine {
  static ParsedLine Parse(std::wstring line);

  enum Type {
    GTEST,
    LOG,
    UNKNOWN,
    NINJA,
  };
  Type type;

  int process_id = 0;
  int thread_id = 0;
  std::wstring translated_thread_id;

  int month = 0;
  int day = 0;

  int hour = 0;
  int minute = 0;
  int seconds = 0;
  int milliseconds = 0;

  std::wstring unknown;

  std::wstring level;

  std::wstring file;
  std::wstring line;
  std::wstring log;
};

#endif /* end of include guard: DATA_PARSED_LINE_HPP */
