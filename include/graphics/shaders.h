#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

GLuint quasarCreateShaderProgram(const char* vertexPath, const char* fragmentPath);
GLuint quasarCompileShader(const char* filepath, GLenum shaderType);
void quasarCheckShaderCompilation(GLuint shader);
void quasarCheckShaderProgramLinking(GLuint shaderProgram);
char* quasarReadFile(const char* filepath);

#endif // !SHADER_H
