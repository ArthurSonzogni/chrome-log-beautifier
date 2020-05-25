#ifndef PARSED_LINE_H
#define PARSED_LINE_H

#include <string>

struct ParsedLine {
  static ParsedLine Parse(std::wstring line);

  enum Type {
    GTEST,
    LOG,
    UNKNOWN,
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

#endif /* end of include guard: PARSED_LINE_H */
