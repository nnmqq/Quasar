#ifndef LOGGER_H
#define LOGGER_H

// Macro to simplify logging
#define QS_LOG(level, message) qsLog(level, message, __func__, __LINE__, __FILE__)

typedef enum {
    QS_LOG_LEVEL_DEBUG = 0,
    QS_LOG_LEVEL_INFO,
    QS_LOG_LEVEL_WARNING,
    QS_LOG_LEVEL_ERROR,
    QS_LOG_LEVEL_FATAL
} QsLogLevel;

typedef struct {
    QsLogLevel level;
    const char* message;
    const char* source;
    int line;
    const char* file;
} __attribute__((aligned(32))) __attribute__((packed)) QsLogMessage;

void qsLog(QsLogLevel level, const char* message, const char* source, int line, const char* file);

#endif // !LOGGER_H
