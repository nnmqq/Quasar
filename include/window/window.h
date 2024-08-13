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
} Window;

Window flan_createWindow(int width, int height, const char* title);
void flan_initGLAD(Window* win);
void flan_terminate(Window* win);

#endif // WINDOW_H
