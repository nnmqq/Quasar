#ifndef WINDOW_H
#define WINDOW_H

#define INITIAL_WINDOW_CAPACITY 4

// Graphics api backend
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Backends
typedef enum {
  QUASAR_API_OPENGL,
  QUASAR_API_VULKAN,
  QUASAR_API_DIRECTX
} QsAPI;

// Global variables
typedef struct {
  int isGlfwInitialized;
  int isGladInitialized;
  int capacity;
} __attribute__((aligned(16))) QsWinVar;

// Window settings
typedef struct {
  int width;
  int height;
  const char* title;
  int resizable;
  int vsync;
  int samples;
  int glMajorVersion;
  int glMinorVersion;
  int openglProfile;
  QsAPI api;
} __attribute__((aligned(64))) QsConfig;

// Window object
typedef struct {
  int width;
  int height;
  const char* title;
  GLFWwindow* window;
  int resizable;
  int vsync;
  long int id;
} __attribute__((aligned(64))) QsWindow;

// Multiple window manager definition
typedef struct {
    QsWindow* windows;
    int windowCount;
} __attribute__((aligned(16))) QsWindowManager;

// Dynamic dispatcher
typedef struct {
    // Window Management
    int (*createWindow)(QsWindow* win, QsConfig* config);
    void (*setContext)(QsWindow* win);
    void (*setVsync)(QsWindow* win, int vsync);
    void (*destroyWindow)(QsWindow* win);
    void (*terminateAPI)();

    // Core Window Manipulation
    int (*getWindowCount)();
    QsWindow* (*getWindowByIndex)(int index);
    QsWindow* (*getWindowByID)(int windowID);
    int (*isWindowOpen)(QsWindow* win);
    void (*setWindowPosition)(QsWindow* win, int x, int y);
    void (*setWindowSize)(QsWindow* win, int width, int height);
    void (*getWindowSize)(QsWindow* win, int* width, int* height);
    void (*getWindowPosition)(QsWindow* win, int* x, int* y);
    void (*setWindowIcon)(QsWindow* win, const char* iconPath);
    void (*setWindowOpacity)(QsWindow* win, float opacity);
    void (*setWindowFullscreen)(QsWindow* win, int fullscreen);
    void (*setWindowBorderless)(QsWindow* win, int borderless);
    void (*toggleFullscreen)(QsWindow* win);
    void (*setWindowFocus)(QsWindow* win);
    void (*showWindow)(QsWindow* win);
    void (*hideWindow)(QsWindow* win);
    void (*minimizeWindow)(QsWindow* win);
    void (*maximizeWindow)(QsWindow* win);
    void (*restoreWindow)(QsWindow* win);

    // Event Handling
    void (*pollEvents)();
    void (*setEventCallback)(QsWindow* win, void (*callback)(QsWindow* win, int eventType, ...));
    int (*shouldClose)(QsWindow* win);

    // Input Handling
    int (*isKeyPressed)(QsWindow* win, int keyCode);
    int (*isKeyReleased)(QsWindow* win, int keyCode);
    int (*isKeyDown)(QsWindow* win, int keyCode);
    int (*isKeyUp)(QsWindow* win, int keyCode);
    int (*isMouseButtonPressed)(QsWindow* win, int button);
    int (*isMouseButtonReleased)(QsWindow* win, int button);
    int (*isMouseButtonDown)(QsWindow* win, int button);
    int (*isMouseButtonUp)(QsWindow* win, int button);
    void (*getMousePosition)(QsWindow* win, double* xpos, double* ypos);

    // Context and Rendering
    void (*swapBuffers)(QsWindow* win);
    void (*makeContextCurrent)(QsWindow* win);
    void (*getCurrentContext)(QsWindow** win);
    void (*shareContext)(QsWindow* win1, QsWindow* win2);
    void (*createContext)(QsWindow* win); // Create a new context for a window
    void (*destroyContext)(QsWindow* win); // Destroy the context associated with a window
    int (*getCurrentContextID)(); // Get the ID of the currently active context
    void (*makeContextCurrentByID)(int contextID); // Make a context current by its ID

    // Custom Data
    void (*setWindowUserData)(QsWindow* win, void* data);
    void* (*getWindowUserData)(QsWindow* win);

    // Backend Agnostic
    int (*getAPI)();
    void (*setAPI)(QsAPI api);
} __attribute__((aligned(128))) QuasarAPI;

int initializeAPI(QuasarAPI* apiInterface, QsAPI api);

// Global variables that indicate some state like glad already running etc
extern QsWinVar windowVar;

// Manages multiple windows
extern QsWindowManager windowManager;

#endif // WINDOW_H
