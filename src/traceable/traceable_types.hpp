#pragma once

#include <string>
#include <memory>

enum TraceSource_Type {
  TRACESOURCE_REPL,
  TRACESOURCE_ARGUMENT,
  TRACESOURCE_FILE
};

typedef struct _TraceSource {
  TraceSource_Type type;
  std::string path_or_commands;
  int row;
  int column;
  int token_length;
} TraceSource;
typedef std::shared_ptr<TraceSource> TraceSource_ptr;
