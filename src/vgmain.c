#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
int main(void) {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}