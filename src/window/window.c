#include "window/window.h"

Window flan_createWindow(int width, int height, const char* title)
{
  Window win = { width, height, title, NULL };

  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  // OpenGL version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create GLFW window
  win.window = glfwCreateWindow(win.width, win.height, win.title, NULL, NULL);
  if (!win.window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(win.window);

  return win;
}

void flan_initGLAD(Window* win)
{
  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    win->window = NULL;
    exit(EXIT_FAILURE);
  }

  // Set OpenGL viewport
  glViewport(0, 0, win->width, win->height);
}

void flan_terminate(Window* win)
{
  if (win->window) {
    glfwDestroyWindow(win->window);
    // No dangling pointers in my yard
    win->window = NULL;
  }
  glfwTerminate();
}
