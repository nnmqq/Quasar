#include <stdio.h>
#include <window/opengl.h>
#include <window/window.h>

QsWinVar windowVar = {.isGlfwInitialized=0, .isGladInitialized=0};
QsWindowManager windowManager = { NULL, 0, INITIAL_WINDOW_CAPACITY };

int initializeAPIInterface(QuasarAPIInterface* apiInterface, QsAPI api)
{
  switch (api) {
  case QUASAR_API_OPENGL:
    apiInterface->createWindow = qsOpenglCreateWindow;
    apiInterface->setContext = qsOpenglSetContext;
    apiInterface->setVsync = qsOpenglSetVsync;
    apiInterface->destroyWindow = qsOpenglDestroyWindow;
    apiInterface->terminateAPI = qsOpenglTerminateAPI;
    return 0;
    break;

  case QUASAR_API_VULKAN:
    break;

  case QUASAR_API_DIRECTX:
    break;

  default:
    return -1;
    break;
  }
  return -1;
}
