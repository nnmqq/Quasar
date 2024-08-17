#include <stdlib.h>
#include <util/error.h>
#include <util/logger.h>
#include <window/opengl.h>

// Helper function to resize the window array safely
static int resizeWindowArray(int newSize)
{
  if (newSize <= 0) {
    free(windowManager.windows);
    windowManager.windows = NULL;
    windowManager.windowCount = 0;
    windowVar.capacity = 0;
    return 0;
  }

  QsWindow* temp = realloc(windowManager.windows, sizeof(QsWindow) * newSize);
  if (temp == NULL) {
    QS_REPORT_ERROR(QS_ERROR_MEMORY_ALLOCATION_FAILURE);
    return -1;
  }

  windowManager.windows = temp;
  windowVar.capacity = newSize;
  return 0;
}

int qsOpenglCreateWindow(QsWindow* win, QsConfig* config)
{
  if (!windowVar.isGlfwInitialized) {
    if (!glfwInit()) {
      QS_REPORT_ERROR(QS_ERROR_INITIALIZATION);
      return -1;
    }
    windowVar.isGlfwInitialized = 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config->glMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config->glMinorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, config->openglProfile);
  glfwWindowHint(GLFW_RESIZABLE, config->resizable ? GL_TRUE : GL_FALSE);

  if (config->samples > 0) {
    glfwWindowHint(GLFW_SAMPLES, config->samples);
  }

  win->window = glfwCreateWindow(config->width, config->height, config->title, NULL, NULL);
  if (!win->window) {
    QS_REPORT_ERROR(QS_ERROR_WINDOW_CREATION);
    glfwTerminate();
    return -1;
  }

  // Set the window's initial state
  win->width = config->width;
  win->height = config->height;
  win->title = config->title;
  win->resizable = config->resizable;
  win->vsync = config->vsync;
  win->id = (int)(intptr_t)win->window;

  glfwMakeContextCurrent(win->window);
  qsOpenglSetVsync(win, config->vsync);

  if (!windowVar.isGladInitialized) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      glfwDestroyWindow(win->window);
      glfwTerminate();
      QS_REPORT_ERROR(QS_ERROR_INITIALIZATION);
      return -1;
    }
    windowVar.isGladInitialized = 1;
  }

  // Check if the window array needs to be resized
  if (windowManager.windowCount >= windowVar.capacity) {
    int newCapacity = (windowVar.capacity == 0) ? 1 : windowVar.capacity * 2;
    if (resizeWindowArray(newCapacity) == -1) {
      glfwDestroyWindow(win->window);
      glfwTerminate();
      return -1;
    }
  }

  // Ensure windowManager.windows is valid
  if (windowManager.windows == NULL) {
    QS_REPORT_ERROR(QS_ERROR_MEMORY_ALLOCATION_FAILURE);
    glfwDestroyWindow(win->window);
    glfwTerminate();
    return -1;
  }

  windowManager.windows[windowManager.windowCount++] = *win;
  return 0;
}

void qsOpenglSetContext(QsWindow* win)
{
  if (win && win->window) {
    glfwMakeContextCurrent(win->window);
  } else {
    QS_REPORT_ERROR(QS_ERROR_INVALID_WINDOW);
  }
}

void qsOpenglSetVsync(QsWindow* win, int vsync)
{
  if (win && win->window) {
    if (vsync == 0) {
      glfwSwapInterval(0); // Disable V-Sync
    } else if (vsync == 1) {
      glfwSwapInterval(1); // Enable V-Sync
    } else {
      QS_REPORT_ERROR(QS_ERROR_INVALID_VSYNC_VALUE);
    }
  } else {
    QS_REPORT_ERROR(QS_ERROR_INVALID_WINDOW);
  }
}

void qsOpenglDestroyWindow(QsWindow* win)
{
  if (!win || !win->window) {
    QS_REPORT_ERROR(QS_ERROR_INVALID_WINDOW);
    return;
  }

  glfwDestroyWindow(win->window);
  win->window = NULL;

  int index = -1;
  for (int i = 0; i < windowManager.windowCount; i++) {
    if (windowManager.windows[i].id == win->id) {
      index = i;
      break;
    }
  }

  if (index != -1) {
    for (int i = index; i < windowManager.windowCount - 1; i++) {
      windowManager.windows[i] = windowManager.windows[i + 1];
    }

    windowManager.windowCount--;
    if (resizeWindowArray(windowManager.windowCount) == -1) {
      QS_REPORT_ERROR(QS_ERROR_MEMORY_ALLOCATION_FAILURE);
    }
  }
}

void qsOpenglTerminateAPI()
{
  for (int i = 0; i < windowManager.windowCount; i++) {
    if (windowManager.windows[i].window) {
      qsOpenglDestroyWindow(&windowManager.windows[i]);
    }
  }

  resizeWindowArray(0);

  if (windowVar.isGlfwInitialized) {
    glfwTerminate();
    windowVar.isGlfwInitialized = 0;
  }
}

