#include <stdio.h>
#include <util/error.h>
#include <util/logger.h>

static QsError lastError = { QS_NO_ERROR, "No error", NULL, 0, NULL };

const char* qsGetErrorMessage(QsErrorCode errorCode)
{
  switch (errorCode) {
  case QS_NO_ERROR: return "No error";
  case QS_ERROR_INITIALIZATION: return "Initialization failed";
  case QS_ERROR_API_FAILURE: return "API call failed";
  case QS_ERROR_INVALID_PARAMETER: return "Invalid parameter provided";
  case QS_ERROR_OUT_OF_MEMORY: return "Out of memory";
  case QS_ERROR_WINDOW_CREATION: return "Window creation failed";
  case QS_ERROR_CONTEXT_CREATION: return "Context creation failed";
  case QS_ERROR_RENDER_FAILURE: return "Render failure ocurred";
  case QS_ERROR_MEMORY_ALLOCATION_FAILURE: return "Memory allocation failed";
  case QS_ERROR_INVALID_WINDOW: return "Provided window is invalid";
  case QS_ERROR_INVALID_VSYNC_VALUE: return "Vsync value should be between 0 and 1";
  default: return "Unknown error";
  }
}

QsError qsGetLastError()
{
  return lastError;
}

void qsClearLastError()
{
  lastError = (QsError) { QS_NO_ERROR, "No error", NULL, 0, NULL };
}

void qsReportError(QsErrorCode code, const char* source, int line, const char* file)
{
  const char* message = qsGetErrorMessage(code);
  lastError = (QsError) {
    .code = code,
    .message = message,
    .source = source,
    .line = line,
    .file = file
  };

  // Log the error
  // fprintf(stderr, "Error [%d]: %s\nSource: %s\nLine: %d\nFile: %s\n",
  //    error.code, error.message, error.source, error.line, error.file);

  // Using logger to log the error
  qsLog(QS_LOG_LEVEL_ERROR, message, source, line, file);
}
