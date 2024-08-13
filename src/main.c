#include "window/window.h"

int main(void)
{
  Window window = flan_createWindow(800, 800, "hello");

  flan_initGLAD(&window);

  glClearColor(0.7f, 0.13f, 0.17f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window.window);

  while (!glfwWindowShouldClose(window.window)) {
    glfwPollEvents();
  }

  flan_terminate(&window);
  return EXIT_SUCCESS;
}
