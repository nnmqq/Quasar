#include <stdio.h>
#include <time.h>
#include <util/logger.h>

// Define ANSI colors
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Convert log level to string with color
const char* qsLogLevelToString(QsLogLevel level)
{
  switch (level) {
  case QS_LOG_LEVEL_DEBUG: return ANSI_COLOR_CYAN "DEBUG" ANSI_COLOR_RESET;
  case QS_LOG_LEVEL_INFO: return ANSI_COLOR_GREEN "INFO" ANSI_COLOR_RESET;
  case QS_LOG_LEVEL_WARNING: return ANSI_COLOR_YELLOW "WARNING" ANSI_COLOR_RESET;
  case QS_LOG_LEVEL_ERROR: return ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET;
  case QS_LOG_LEVEL_FATAL: return ANSI_COLOR_MAGENTA "FATAL" ANSI_COLOR_RESET;
  default: return ANSI_COLOR_RESET "UNKNOWN" ANSI_COLOR_RESET;
  }
}

// Log function
void qsLog(QsLogLevel level, const char* message, const char* source, int line, const char* file)
{
  time_t now = 0;
  time(&now);
  struct tm* local = localtime(&now);

  fprintf(stderr, "[%02d:%02d:%02d] [%s] [%s:%d] %s: %s\n",
      local->tm_hour, local->tm_min, local->tm_sec,
      qsLogLevelToString(level), file, line, source, message);
}
