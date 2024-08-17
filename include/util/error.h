#ifndef ERROR_H
#define ERROR_H

// Macro for easy error reporting
#define QS_REPORT_ERROR(code) qsReportError(code, __func__, __LINE__, __FILE__)

typedef enum {
  QS_NO_ERROR = 0,
  QS_ERROR_INITIALIZATION,
  QS_ERROR_API_FAILURE,
  QS_ERROR_INVALID_PARAMETER,
  QS_ERROR_OUT_OF_MEMORY,
  QS_ERROR_WINDOW_CREATION,
  QS_ERROR_CONTEXT_CREATION,
  QS_ERROR_RENDER_FAILURE,
  QS_ERROR_MEMORY_ALLOCATION_FAILURE,
  QS_ERROR_INVALID_WINDOW,
  QS_ERROR_INVALID_VSYNC_VALUE,
} QsErrorCode;

typedef struct {
  QsErrorCode code;
  const char* message;
  const char* source;
  int line;
  const char* file;
} __attribute__((aligned(32))) QsError;

const char* qsGetErrorMessage(QsErrorCode errorCode);
QsError qsGetLastError();
void qsReportError(QsErrorCode code, const char* source, int line, const char* file);
void qsClearLastError();

#endif // !ERROR_H
