#pragma once
#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

//�����Ŀ����֮ǰ�����д���
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
