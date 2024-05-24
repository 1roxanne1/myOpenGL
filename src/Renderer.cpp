#include "Renderer.h"
#include <iostream>

//清除在目标行之前的所有错误
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGl Error](" << error << "):" << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}