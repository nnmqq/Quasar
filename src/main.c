#include "window/opengl.h"
#include <util/error.h>
#include <window/window.h>

int main(void)
{
  QuasarAPI quasar;
  if (!initializeAPI(&quasar, QUASAR_API_OPENGL)) {
    QS_REPORT_ERROR(QS_ERROR_API_FAILURE);
    return -1;
  }

  QsConfig config = {
    .width = 800,
    .height = 600,
    .title = "OpenGL Test Window",
    .resizable = 1,
    .vsync = 1,
    .samples = 4,
    .glMajorVersion = 4,
    .glMinorVersion = 4,
    .openglProfile = GLFW_OPENGL_CORE_PROFILE,
    .api = QUASAR_API_OPENGL
  };
  QsWindow window;

  if (!quasar.createWindow(&window, &config)) {
    QS_REPORT_ERROR(QS_ERROR_WINDOW_CREATION);
  }
  quasar.setContext(&window);
  quasar.setWindowIcon(&window, "assets/icon.png");

  while (!glfwWindowShouldClose(window.window)) {
    // Use escape to close window
    if (glfwGetKey(window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window.window, true);
    }

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window.window);
    glfwPollEvents();
  }

  quasar.destroyWindow(&window);
  quasar.terminateAPI();
  return 0;
}
