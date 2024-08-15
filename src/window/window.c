#include "window/window.h"

// Haven't tested multiple window contexts so this is thought for single window apps as of right now
// TODO:
// - error handling
// - add dynamic/static antialiasing configuration
// - add aspect ratio stuff
// - add more backends (will start doing this next year, too lazy rn)

Window windowRegistry[MAX_WINDOWS];
int quasarWindowCount = 0;
int isGlfwInitialized = 0;
int isGladInitialized = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // Make the current context active
  glfwMakeContextCurrent(window);

  // Set the viewport to match the new framebuffer size
  glViewport(0, 0, width, height);
}

// Handle creation of window
int quasarCreateWindow(Window* win, int width, int height, const char* title, int resizable)
{
  if (!quasarInitGlfw())
    return 0;

  if (quasarWindowCount >= MAX_WINDOWS) {
    fprintf(stderr, "Maximum number of windows reached\n");
    return 0;
  }

  // Initialize the window "object"
  *win = (Window) {
    .width = width,
    .height = height,
    .title = title,
    .window = NULL,
    .resizable = resizable,
    .vsync = 1,
    .id = quasarWindowCount
  };

  // OpenGL version and profile, pretty much boiler plate
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // TODO: add dynamic/static antialiasing configuration
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA

  // Set window resizable hint
  glfwWindowHint(GLFW_RESIZABLE, win->resizable);

  // Create GLFW window
  win->window = glfwCreateWindow(win->width, win->height, win->title, NULL, NULL);
  if (!win->window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return 0; // Failed
  }

  glfwMakeContextCurrent(win->window);
  glfwSwapInterval(1); // Enable V-Sync by default

  if (win->resizable) {
    glfwSetFramebufferSizeCallback(win->window, framebuffer_size_callback);
  }

  if (!quasarInitGLAD(win))
    return 0;

  windowRegistry[quasarWindowCount] = *win;
  quasarWindowCount++;
  return 1;
}

// More boiler plate
int quasarInitGLAD(Window* win)
{
  if (!isGladInitialized) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      fprintf(stderr, "Failed to initialize GLAD\n");
      return 0; // Return instead of exiting
    }
    isGladInitialized = 1;
    // Set OpenGL viewport
    glViewport(0, 0, win->width, win->height);
  }
  return 1;
}

int quasarInitGlfw()
{
  if (!isGlfwInitialized) {
    if (!glfwInit()) {
      fprintf(stderr, "Failed to initialize GLFW\n");
      return 0;
    }
    isGlfwInitialized = 1;
  }
  return 1;
}

// Terminate window and set to null the window pointer
int quasarCloseWindow(Window* win)
{
  if (win->window) {
    glfwDestroyWindow(win->window);
    win->window = NULL; // No dangling pointers in my yard
  } else {
    return 0;
  }
  return 1;
}

int quasarCloseWindowByID(int id)
{
  Window* win = quasarGetWindowByID(id);
  if (win->window) {
    glfwDestroyWindow(win->window);
    win->window = NULL; // No dangling pointers in my yard
  } else {
    return 0;
  }
  return 1;
}

int quasarTerminate()
{
  for (int i = 0; i < quasarWindowCount; i++) {
    quasarCloseWindow(&windowRegistry[i]);
  }
  glfwTerminate();
  return 1;
}

// Dynamically toggle window resizing
void quasarSetWindowResizable(Window* win, int resizable)
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
void quasarSetVsync(Window* win, int vsync)
{
  glfwMakeContextCurrent(win->window);
  glfwSwapInterval(vsync ? 1 : 0);
  win->vsync = vsync;
}

void quasarSetWindowContext(Window* win)
{
  glfwMakeContextCurrent(win->window);
}

void quasarSetWindowContextByID(int id)
{
  Window* win = quasarGetWindowByID(id);
  if (win) {
    glfwMakeContextCurrent(win->window);
  }
}

Window* quasarGetWindowByID(int id)
{
  if (id >= 0 && id < quasarWindowCount) {
    return &windowRegistry[id];
  }
  return NULL;
}
