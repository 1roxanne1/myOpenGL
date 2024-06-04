//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//
//#include "../Renderer.h"
//#include "../VertexBuffer.h"
//#include "../IndexBuffer.h"
//#include "../VertexArray.h"
//#include "../VertexBufferLayout.h"
//#include "../Shader.h"
//
//int main()
//{
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "GL Work1!!!", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    glfwSwapInterval(1);
//
//    if (glewInit() != GLEW_OK) {
//        std::cout << "Error!" << std::endl;
//    }
//
//    std::cout << glGetString(GL_VERSION) << std::endl;
//    {
//        //顶点数据
//        GLfloat vertices[] = {
//            // 位置              // 颜色
//            -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 左下  0
//             0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 右下  1
//             0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    // 右上  2
//            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f   // 左上  3
//        };
//        //索引数据
//        unsigned int indices[6] = {
//            0,1,2,
//            2,3,0
//        };
//        //创建顶点数组
//        VertexArray va;
//        //创建顶点缓冲
//        VertexBuffer vb(vertices,4*6*sizeof(float));
//        //解释顶点数据
//        VertexBufferLayout layout;
//        layout.Push<float>(3);   //位置格式
//        layout.Push<float>(3);   //颜色格式
//        va.addBuffer(vb, layout);
//        //创建索引缓冲
//        IndexBuffer ib(indices,6);
//        //创建着色器
//        Shader shader("res/shaders/work2.shader");
//
//        //解绑
//        GLCALL(va.Unbind());
//        GLCALL(vb.Unbind());
//        GLCALL(ib.Unbind());
//        GLCALL(shader.Unbind());
//
//        //创建渲染器
//        Renderer renderer;
//        while (!glfwWindowShouldClose(window))
//        {
//            renderer.Clear();
//            renderer.Draw(va,ib,shader);
//            /* Swap front and back buffers */
//            glfwSwapBuffers(window);
//            /* Poll for and process events */
//            glfwPollEvents();
//        }
//    }
//    glfwTerminate();
//    return 0;
//}