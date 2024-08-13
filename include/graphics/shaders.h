#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

GLuint flan_createShaderProgram(const char* vertexPath, const char* fragmentPath);
GLuint flan_compileShader(const char* filepath, GLenum shaderType);
void flan_checkShaderCompilation(GLuint shader);
void flan_checkShaderProgramLinking(GLuint shaderProgram);
char* flan_readFile(const char* filepath);

#endif // !SHADER_H
