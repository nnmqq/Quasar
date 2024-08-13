#ifndef BUFFER_H
#define BUFFER_H

#include <glad/glad.h>
#include <stdio.h>

typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
} Buffer;

Buffer createBuffer(const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize);

void bindBuffer(Buffer* buffer);
void unbindBuffer();
void deleteBuffer(Buffer* buffer);

#endif // BUFFER_H
