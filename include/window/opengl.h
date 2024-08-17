#ifndef OPENGL_H
#define OPENGL_H

#include <window/window.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

// OpenGL-specific function declarations
// OpenGL-specific function declarations
int qsOpenglCreateWindow(QsWindow* win, QsConfig* config);
void qsOpenglSetContext(QsWindow* win);
void qsOpenglSetVsync(QsWindow* win, int vsync);
void qsOpenglDestroyWindow(QsWindow* win);
void qsOpenglTerminateAPI();

int qsOpenglGetWindowCount();
QsWindow* qsOpenglGetWindowByIndex(int index);
QsWindow* qsOpenglGetWindowByID(int windowID);
int qsOpenglIsWindowOpen(QsWindow* win);
void qsOpenglSetWindowPosition(QsWindow* win, int x, int y);
void qsOpenglSetWindowSize(QsWindow* win, int width, int height);
void qsOpenglGetWindowSize(QsWindow* win, int* width, int* height);
void qsOpenglGetWindowPosition(QsWindow* win, int* x, int* y);
void qsOpenglSetWindowIcon(QsWindow* win, const char* iconPath);
void qsOpenglSetWindowOpacity(QsWindow* win, float opacity);
void qsOpenglSetWindowFullscreen(QsWindow* win, int fullscreen);
void qsOpenglSetWindowBorderless(QsWindow* win, int borderless);
void qsOpenglToggleFullscreen(QsWindow* win);
void qsOpenglSetWindowFocus(QsWindow* win);
void qsOpenglShowWindow(QsWindow* win);
void qsOpenglHideWindow(QsWindow* win);
void qsOpenglMinimizeWindow(QsWindow* win);
void qsOpenglMaximizeWindow(QsWindow* win);
void qsOpenglRestoreWindow(QsWindow* win);

void qsOpenglPollEvents();
void qsOpenglSetEventCallback(QsWindow* win, void (*callback)(QsWindow* win, int eventType, ...));
int qsOpenglShouldClose(QsWindow* win);

int qsOpenglIsKeyPressed(QsWindow* win, int keyCode);
int qsOpenglIsKeyReleased(QsWindow* win, int keyCode);
int qsOpenglIsKeyDown(QsWindow* win, int keyCode);
int qsOpenglIsKeyUp(QsWindow* win, int keyCode);
int qsOpenglIsMouseButtonPressed(QsWindow* win, int button);
int qsOpenglIsMouseButtonReleased(QsWindow* win, int button);
int qsOpenglIsMouseButtonDown(QsWindow* win, int button);
int qsOpenglIsMouseButtonUp(QsWindow* win, int button);
void qsOpenglGetMousePosition(QsWindow* win, double* xpos, double* ypos);

void qsOpenglSwapBuffers(QsWindow* win);
void qsOpenglMakeContextCurrent(QsWindow* win);
void qsOpenglGetCurrentContext(QsWindow** win);
void qsOpenglShareContext(QsWindow* win1, QsWindow* win2);
void qsOpenglCreateContext(QsWindow* win);
void qsOpenglDestroyContext(QsWindow* win);
int qsOpenglGetCurrentContextID();
void qsOpenglMakeContextCurrentByID(int contextID);

void qsOpenglSetWindowUserData(QsWindow* win, void* data);
void* qsOpenglGetWindowUserData(QsWindow* win);

int qsOpenglGetAPI();
void qsOpenglSetAPI(QsAPI api);

#endif // OPENGL_H
