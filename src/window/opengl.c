#include <stdlib.h>
#include <util/error.h>
#include <util/logger.h>
#include <window/opengl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <stdio.h>

// Function to print the state of the window manager
void printWindowManager()
{
  if (windowManager.windows == NULL) {
    QS_LOG(QS_LOG_LEVEL_WARNING, "Window Manager: No windows currently managed.\n");
    return;
  }

  printf("Window Manager State:\n");
  printf("Total windows managed: %d\n", windowManager.windowCount);
  printf("Window array capacity: %d\n", windowVar.capacity);

  for (int i = 0; i < windowManager.windowCount; i++) {
    QsWindow* win = &windowManager.windows[i];
    printf("Window %d:\n", i + 1);
    printf("  ID: %ld\n", win->id);
    printf("  Title: %s\n", win->title);
    printf("  Size: %d x %d\n", win->width, win->height);
    printf("  Resizable: %s\n", win->resizable ? "Yes" : "No");
    printf("  V-Sync: %s\n", win->vsync ? "Enabled" : "Disabled");
    printf("  OpenGL Context: %s\n", (glfwGetCurrentContext() == win->window) ? "Active" : "Inactive");
  }
}

// Helper function to resize the window array safely
static int resizeWindowArray(int newSize)
{
  // If the new size is 0 or negative, free the memory and reset related variables
  if (newSize <= 0) {
    free(windowManager.windows); // Deallocate the current window array
    windowManager.windows = NULL; // Set the window array pointer to NULL to avoid dangling pointers
    windowManager.windowCount = 0;
    windowVar.capacity = 0;
    return QS_SUCCESS;
  }

  // Try to resize the window array to the new size
  QsWindow* temp = realloc(windowManager.windows, sizeof(QsWindow) * newSize);
  if (temp == NULL) {
    QS_REPORT_ERROR(QS_ERROR_MEMORY_ALLOCATION_FAILURE);
    return QS_FAILURE;
  }

  // If realloc succeeds, update the window array pointer and capacity
  windowManager.windows = temp;
  windowVar.capacity = newSize;
  return QS_SUCCESS;
}

// Create the Window object with the configs provided and adds it to the window manager object
int qsOpenglCreateWindow(QsWindow* win, QsConfig* config)
{
  // Initialiaze glad
  if (!windowVar.isGlfwInitialized) {
    if (!glfwInit()) {
      QS_REPORT_ERROR(QS_ERROR_INITIALIZATION);
      return QS_FAILURE;
    }
    windowVar.isGlfwInitialized = 1;
  }

  // Set opengl hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config->glMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config->glMinorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, config->openglProfile);
  glfwWindowHint(GLFW_RESIZABLE, config->resizable ? GL_TRUE : GL_FALSE);

  // Set antialiasing
  if (config->samples > 0) {
    glfwWindowHint(GLFW_SAMPLES, config->samples);
  }

  // Creates the window
  win->window = glfwCreateWindow(config->width, config->height, config->title, NULL, NULL);
  if (!win->window) {
    QS_REPORT_ERROR(QS_ERROR_WINDOW_CREATION);
    glfwTerminate();
    return QS_FAILURE;
  }

  // Set the window's initial state
  win->width = config->width;
  win->height = config->height;
  win->title = config->title;
  win->resizable = config->resizable;
  win->vsync = config->vsync;
  win->id = (intptr_t)win->window;

  // Make the just made window the current context
  glfwMakeContextCurrent(win->window);
  // Toggle vsync
  qsOpenglSetVsync(win, config->vsync);

  // Initialiaze glad
  if (!windowVar.isGladInitialized) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      glfwDestroyWindow(win->window);
      glfwTerminate();
      QS_REPORT_ERROR(QS_ERROR_INITIALIZATION);
      return QS_FAILURE;
    }
    windowVar.isGladInitialized = 1;
  }

  // Check if the window array needs to be resized
  if (windowManager.windowCount >= windowVar.capacity) {
    int newCapacity = (windowVar.capacity == 0) ? 1 : windowVar.capacity * 2;
    if (!resizeWindowArray(newCapacity)) {
      glfwDestroyWindow(win->window);
      glfwTerminate();
      return QS_FAILURE;
    }
  }

  // Allocate memory
  if (windowManager.windows == NULL) {
    void* temp = malloc(sizeof(QsWindow) * INITIAL_WINDOW_CAPACITY);
    if (temp == NULL) {
      QS_REPORT_ERROR(QS_ERROR_MEMORY_ALLOCATION_FAILURE);
      glfwDestroyWindow(win->window);
      glfwTerminate();
      return QS_FAILURE;
    }
    windowManager.windows = temp;
  }

  // Add window to the window manager object
  windowManager.windows[windowManager.windowCount++] = *win;
  return QS_SUCCESS;
}

// Set the provided window as opengl current context
void qsOpenglSetContext(QsWindow* win)
{
  if (win && win->window) {
    glfwMakeContextCurrent(win->window);
  } else {
    QS_REPORT_ERROR(QS_ERROR_INVALID_WINDOW);
  }
}

// Toggle vsync
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

// Destroy window and clean up
void qsOpenglDestroyWindow(QsWindow* win)
{
  // Check if window is valid
  if (!win || !win->window) {
    QS_REPORT_ERROR(QS_ERROR_INVALID_WINDOW);
    return;
  }

  // Destroy window and free the memory
  glfwDestroyWindow(win->window);
  win->window = NULL;

  // Remove the window from the window manager object
  int index = -1;
  for (int i = 0; i < windowManager.windowCount; i++) {
    if (windowManager.windows[i].id == win->id) {
      index = i;
      break;
    }
  }

  // Shift remaining objects after the obliterated window to fill the gap
  if (index != -1) {
    for (int i = index; i < windowManager.windowCount - 1; i++) {
      windowManager.windows[i] = windowManager.windows[i + 1];
    }

    windowManager.windowCount--;
    // Resizes the array
    if (!resizeWindowArray(windowManager.windowCount)) {
      QS_REPORT_ERROR(QS_ERROR_MEMORY_ALLOCATION_FAILURE);
    }
  }
}

// Obliterate the api (terminate everything related the the window)
void qsOpenglTerminateAPI()
{
  // Close all window
  for (int i = 0; i < windowManager.windowCount; i++) {
    if (windowManager.windows[i].window) {
      qsOpenglDestroyWindow(&windowManager.windows[i]);
    }
  }

  // Set array to null
  resizeWindowArray(0);

  // terminate glfw
  if (windowVar.isGlfwInitialized) {
    glfwTerminate();
    windowVar.isGlfwInitialized = 0;
  }
}

inline int qsOpenglGetWindowCount()
{
  return windowManager.windowCount;
}

// TODO(nnmqq): implement this functions
QsWindow* qsOpenglGetWindowByIndex(int index)
{
  return NULL;
}
QsWindow* qsOpenglGetWindowByID(int windowID)
{
  return NULL;
}
int qsOpenglIsWindowOpen(QsWindow* win)
{
  return QS_SUCCESS;
}

inline void qsOpenglSetWindowPosition(QsWindow* win, int x, int y)
{
  glfwSetWindowPos(win->window, x, y);
}

inline void qsOpenglSetWindowSize(QsWindow* win, int width, int height)
{
  glfwSetWindowSize(win->window, width, height);
}

inline void qsOpenglGetWindowPosition(QsWindow* win, int* x, int* y)
{
  glfwGetWindowSize(win->window, x, y);
}

void qsOpenglSetWindowIcon(QsWindow* win, const char* iconPath)
{
  GLFWimage image;
  int channels = 0;
  image.pixels = stbi_load(iconPath, &image.width, &image.height, &channels, 4);

  if (image.pixels) {
    glfwSetWindowIcon(win->window, 1, &image);

    stbi_image_free(image.pixels);
  } else {
    QS_REPORT_ERROR(QS_ERROR_INVALID_PARAMETER);
  }
}
