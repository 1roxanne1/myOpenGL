#pragma once

#include <iostream>
#include "GL/glew.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak()

/* 反斜杠后面不能有空格 */
#define GLCALL(x) do { \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
 } while (0)

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

//清除在目标行之前的所有错误
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


