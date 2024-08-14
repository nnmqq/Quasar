#include "window/window.h"
#include "graphics/shaders.h"
#include "graphics/buffer.h"

// Vertex data for a square
const float vertices[] = {
  // Positions          // Colors
  -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-left
  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
  0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f // Top-right
};

const unsigned int indices[] = {
  0, 1, 2, // First triangle
  2, 3, 0 // Second triangle
};

int main(void)
{
  Window window = quasarCreateWindow(800, 800, "Shader Example", 1);
  quasarInitGLAD(&window);

  // Load and compile shaders
  GLuint shaderProgram = quasarCreateShaderProgram("src/shaders/vertex_shader.glsl", "src/shaders/fragment_shader.glsl");
  // Create buffer
  Buffer buffer = createBuffer(vertices, sizeof(vertices), indices, sizeof(indices));

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  quasarSetWindowTitle(&window, "           testing long           stuff");

  // Main loop
  while (!glfwWindowShouldClose(window.window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    bindBuffer(&buffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window.window);
    glfwPollEvents();
  }
  deleteBuffer(&buffer);
  quasarTerminate(&window);

  return EXIT_SUCCESS;
}
