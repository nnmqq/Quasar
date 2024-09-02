#include "util/error.h"
#include <stdio.h>
#include <window/opengl.h>
#include <window/window.h>

QsWinVar windowVar = {.isGlfwInitialized=0, .isGladInitialized=0, .capacity=INITIAL_WINDOW_CAPACITY};
QsWindowManager windowManager = { NULL, 0};

// Dynamic dispatcher of API specific functions
int initializeAPI(QuasarAPI* apiInterface, QsAPI api)
{
  switch (api) {
  case QUASAR_API_OPENGL:
    apiInterface->createWindow = qsOpenglCreateWindow;
    apiInterface->setContext = qsOpenglSetContext;
    apiInterface->setVsync = qsOpenglSetVsync;
    apiInterface->destroyWindow = qsOpenglDestroyWindow;
    apiInterface->terminateAPI = qsOpenglTerminateAPI;

    apiInterface->setWindowIcon = qsOpenglSetWindowIcon;
    return QS_SUCCESS;
    break;

  case QUASAR_API_VULKAN:
    break;

  case QUASAR_API_DIRECTX:
    break;

  default:
    return QS_FAILURE;
    break;
  }
  return QS_FAILURE;
}
