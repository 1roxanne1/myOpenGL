#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "GL Study!!!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        //顶点缓冲区
        float position[12] = {
            -0.5f, -0.5f,   //0
             0.5f, -0.5f,   //1
             0.5f,  0.5f,   //2
            -0.5f,  0.5f,   //3
        };
        //索引缓冲区,注意索引缓冲区必须是无符号的！！！
        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        //创建顶点数组
        VertexArray va;

        //顶点缓冲
        VertexBuffer vb(position, 4 * 2 * sizeof(float));

        //启用顶点属性
        //1.顶点数据布局( glVertexAttribPointer )
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.addBuffer(vb, layout);

        //索引缓冲
        IndexBuffer ib(indices, 6);

        //处理着色器
        Shader shader("res/shaders/Basic.shader");
        
        
        //统一变量
        shader.Bind();
        shader.SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

        //解除绑定
        GLCALL(va.Unbind()); //顶点数组
        GLCALL(vb.Unbind()); //顶点缓冲区
        GLCALL(ib.Unbind());//索引缓冲
        GLCALL(shader.Unbind()); //着色器程序

        float r = 0.0f;
        float increment = 0.05f;

        //渲染器
        Renderer renderer;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //绑定
            shader.Bind();
            //设置统一变量
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}