#include "graphics/shaders.h"

GLuint quasarCreateShaderProgram(const char* vertexPath, const char* fragmentPath)
{
  GLuint shaderProgram = glCreateProgram();

  GLuint vertexShader = quasarCompileShader(vertexPath, GL_VERTEX_SHADER);
  GLuint fragmentShader = quasarCompileShader(fragmentPath, GL_FRAGMENT_SHADER);

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  quasarCheckShaderProgramLinking(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

GLuint quasarCompileShader(const char* sourcePath, GLenum shaderType)
{
    char* source = quasarReadFile(sourcePath);  // Read the file content

    GLuint shader = glCreateShader(shaderType);
    const GLchar* shaderSource = (const GLchar*)source;
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    quasarCheckShaderCompilation(shader);

    free(source);  // Free the source buffer after compilation

    return shader;
}

void quasarCheckShaderCompilation(GLuint shader)
{
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    fprintf(stderr, "Shader compilation error: %s\n", infoLog);
    exit(EXIT_FAILURE);
  }
}

void quasarCheckShaderProgramLinking(GLuint shaderProgram)
{
  GLint success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    fprintf(stderr, "Program linking error: %s\n", infoLog);
    exit(EXIT_FAILURE);
  }
}

char* quasarReadFile(const char* filepath)
{
  FILE* file = fopen(filepath, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file: %s\n", filepath);
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = (char*)malloc(length + 1);
  if (!buffer) {
    fprintf(stderr, "Failed to allocate memory for file: %s\n", filepath);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  fread(buffer, 1, length, file);
  fclose(file);

  buffer[length] = '\0'; // Null-terminate the string
  return buffer;
}
