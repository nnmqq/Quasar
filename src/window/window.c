#include "window/window.h"

// Haven't tested multiple window contexts so this is thought for single window apps as of right now
// TODO:
// - support multiple window contexts
// - add dynamic/static antialiasing configuration
// - add aspect ratio stuff
// - add more backends (will start doing this next year, too lazy rn)

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Make the current context active
    glfwMakeContextCurrent(window);

    // Set the viewport to match the new framebuffer size
    glViewport(0, 0, width, height);
}

// Handle creation of window
Window quasarCreateWindow(int width, int height, const char* title, bool resizable)
{
  // Initialize the window "object"
  Window win = { width, height, title, NULL, resizable, 1 };

  printf("Vsync: ");
  if(win.vsync) printf("on");
  else printf("off");
  // Initialize GLFW, boiler plate
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return win; // Return empty Window instead of exiting
  }

  // OpenGL version and profile, pretty much boiler plate
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // TODO: add dynamic/static antialiasing configuration
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA

  // Set window resizable hint
  glfwWindowHint(GLFW_RESIZABLE, win.resizable);

  // Create GLFW window
  win.window = glfwCreateWindow(win.width, win.height, win.title, NULL, NULL);
  if (!win.window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return win; // Return empty Window instead of exiting
  }

  glfwMakeContextCurrent(win.window);
  glfwSwapInterval(1); // Disable V-Sync by default

  if (win.resizable) {
    glfwSetFramebufferSizeCallback(win.window, framebuffer_size_callback);
  }

  return win;
}

// More boiler plate
void quasarInitGLAD(Window* win)
{
  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    quasarTerminate(win);
    return; // Return instead of exiting
  }

  // Set OpenGL viewport
  glViewport(0, 0, win->width, win->height);
}

// Terminate window and set to null the window pointer
void quasarTerminate(Window* win)
{
  if (win->window) {
    glfwDestroyWindow(win->window);
    win->window = NULL; // No dangling pointers in my yard
  }
  glfwTerminate();
}

// Dynamically toggle window resizing
void quasarSetWindowResizable(Window* win, bool resizable)
{
  if (win->resizable != resizable) {
    glfwSetWindowAttrib(win->window, GLFW_RESIZABLE, resizable);
    win->resizable = resizable;

    if (resizable) {
      glfwSetFramebufferSizeCallback(win->window, framebuffer_size_callback);
    } else {
      glfwSetFramebufferSizeCallback(win->window, NULL);
    }
  }
}

// Change window size
void quasarSetWindowSize(Window* win, int width, int height)
{
  win->width = width;
  win->height = height;
  glfwSetWindowSize(win->window, width, height);
  glViewport(0, 0, width, height); // Adjust the OpenGL viewport
}

// Change window title
void quasarSetWindowTitle(Window* win, const char* title)
{
  win->title = title;
  glfwSetWindowTitle(win->window, title);
}

// Toggle vsync
void quasarSetVsync(Window* win, bool vsync)
{
  // I need to proper set this, I think this whole function is hard coded
  glfwMakeContextCurrent(win->window);
  // Set the swap interval for the current context
  glfwSwapInterval(vsync ? 1 : 0);
  win->vsync = vsync;
}
