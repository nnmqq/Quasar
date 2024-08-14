#ifndef WINDOW_H
#define WINDOW_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int width;
  int height;
  const char* title;
  GLFWwindow* window;
  bool resizable;
  bool vsync;
} Window;

Window quasarCreateWindow(int width, int height, const char* title, bool resizable);
void quasarInitGLAD(Window* win);
void quasarTerminate(Window* win);

void quasarSetWindowResizable(Window* win, bool resizable);
void quasarSetVsync(Window* win, bool vsync);
void quasarSetWindowSize(Window* win, int width, int height);
void quasarSetWindowTitle(Window* win, const char* title);

#endif // WINDOW_H
