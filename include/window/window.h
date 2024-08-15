#ifndef WINDOW_H
#define WINDOW_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#define MAX_WINDOWS 10

typedef struct {
    int majorVersion;
    int minorVersion;
    int profile;
    int samples;
    int vsync;
    int resizable;
    // Add more fields as needed
} OpenGLConfig;

typedef struct {
  int width;
  int height;
  const char* title;
  GLFWwindow* window;
  int resizable;
  int vsync;
  int id;
} Window;

// Store created window
extern Window windowRegistry[MAX_WINDOWS];
extern int quasarWindowCount;
extern int isGlfwInitialized;
extern int isGladInitialized;

int quasarCreateWindow(Window* win, int width, int height, const char* title, int resizable);
int quasarInitGLAD(Window* win);
int quasarInitGlfw();
int quasarCloseWindow(Window* win);
int quasarCloseWindowByID(int id);
int quasarTerminate();

void quasarSetWindowResizable(Window* win, int resizable);
void quasarSetVsync(Window* win, int vsync);
void quasarSetWindowSize(Window* win, int width, int height);
void quasarSetWindowTitle(Window* win, const char* title);
void quasarSetWindowContext(Window* win);
void quasarSetWindowContextByID(int id);
Window* quasarGetWindowByID(int id);

#endif // WINDOW_H
