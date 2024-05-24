#pragma once
#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

//清除在目标行之前的所有错误
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
