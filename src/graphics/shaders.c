#include "graphics/shaders.h"

GLuint flan_createShaderProgram(const char* vertexPath, const char* fragmentPath)
{
  GLuint shaderProgram = glCreateProgram();

  GLuint vertexShader = flan_compileShader(vertexPath, GL_VERTEX_SHADER);
  GLuint fragmentShader = flan_compileShader(fragmentPath, GL_FRAGMENT_SHADER);

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  flan_checkShaderProgramLinking(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

GLuint flan_compileShader(const char* sourcePath, GLenum shaderType)
{
    char* source = flan_readFile(sourcePath);  // Read the file content

    GLuint shader = glCreateShader(shaderType);
    const GLchar* shaderSource = (const GLchar*)source;
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    flan_checkShaderCompilation(shader);

    free(source);  // Free the source buffer after compilation

    return shader;
}

void flan_checkShaderCompilation(GLuint shader)
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

void flan_checkShaderProgramLinking(GLuint shaderProgram)
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

char* flan_readFile(const char* filepath)
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
