#include "util/error.h"
#include "util/logger.h"
#include <stdio.h>
#include <window/window.h>

void test()
{
  QS_REPORT_ERROR(QS_NO_ERROR);
  QS_LOG(QS_LOG_LEVEL_INFO, "message");
  QS_LOG(QS_LOG_LEVEL_ERROR, "message");
  QS_LOG(QS_LOG_LEVEL_WARNING, "message");
  QS_LOG(QS_LOG_LEVEL_FATAL, "message");
}

int main()
{
  QuasarAPIInterface apiInterface;
  if (initializeAPIInterface(&apiInterface, QUASAR_API_OPENGL) != 0) {
    fprintf(stderr, "Failed to initialize API interface.\n");
    return -1;
  }

  QsConfig config1 = {
    .width = 800,
    .height = 600,
    .title = "OpenGL Window",
    .resizable = 1,
    .vsync = 1,
    .samples = 4,
    .glMajorVersion = 4,
    .glMinorVersion = 4,
    .openglProfile = GLFW_OPENGL_CORE_PROFILE,
    .api = QUASAR_API_OPENGL
  };
  QsConfig config2 = {
    .width = 800,
    .height = 600,
    .title = "OpenGL Window",
    .resizable = 1,
    .vsync = 1,
    .samples = 4,
    .glMajorVersion = 4,
    .glMinorVersion = 4,
    .openglProfile = GLFW_OPENGL_CORE_PROFILE,
    .api = QUASAR_API_OPENGL
  };

  QsWindow window1;
  QsWindow window2;

  if (apiInterface.createWindow(&window1, &config1) != 0) {
    fprintf(stderr, "Failed to create window 1.\n");
    return -1;
  }

  if (apiInterface.createWindow(&window2, &config2) != 0) {
    fprintf(stderr, "Failed to create window 2.\n");
    apiInterface.destroyWindow(&window1);
    return -1;
  }

  apiInterface.setContext(&window1);
  apiInterface.setContext(&window2);

  apiInterface.setVsync(&window1, 1);
  apiInterface.setVsync(&window2, 1);

  while (!glfwWindowShouldClose(window1.window) && !glfwWindowShouldClose(window2.window)) {
    // Render and swap buffers
    apiInterface.setContext(&window1);
    // Render code for window1...

    apiInterface.setContext(&window2);
    // Render code for window2...

    apiInterface.setContext(&window1);
    glfwSwapBuffers(window1.window);

    apiInterface.setContext(&window2);
    glfwSwapBuffers(window2.window);

    apiInterface.setContext(&window1);
    glfwPollEvents();
  }

  // Destroy the windows
  apiInterface.destroyWindow(&window1);
  apiInterface.destroyWindow(&window2);

  // Terminate the OpenGL API
  apiInterface.terminateAPI();

  return 0;
}
