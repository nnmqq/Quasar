#include "graphics/buffer.h"

Buffer createBuffer(const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize)
{
    Buffer buffer;

    glGenVertexArrays(1, &buffer.VAO);
    glGenBuffers(1, &buffer.VBO);
    glGenBuffers(1, &buffer.EBO);

    glBindVertexArray(buffer.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

    return buffer;
}

void bindBuffer(Buffer* buffer)
{
    glBindVertexArray(buffer->VAO);
}

void unbindBuffer()
{
    glBindVertexArray(0);
}

void deleteBuffer(Buffer* buffer)
{
    glDeleteVertexArrays(1, &buffer->VAO);
    glDeleteBuffers(1, &buffer->VBO);
    glDeleteBuffers(1, &buffer->EBO);
}

