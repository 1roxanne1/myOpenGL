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
//        //��������
//        GLfloat vertices[] = {
//            // λ��              // ��ɫ
//            -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����  0
//             0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����  1
//             0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    // ����  2
//            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f   // ����  3
//        };
//        //��������
//        unsigned int indices[6] = {
//            0,1,2,
//            2,3,0
//        };
//        //������������
//        VertexArray va;
//        //�������㻺��
//        VertexBuffer vb(vertices,4*6*sizeof(float));
//        //���Ͷ�������
//        VertexBufferLayout layout;
//        layout.Push<float>(3);   //λ�ø�ʽ
//        layout.Push<float>(3);   //��ɫ��ʽ
//        va.addBuffer(vb, layout);
//        //������������
//        IndexBuffer ib(indices,6);
//        //������ɫ��
//        Shader shader("res/shaders/work2.shader");
//
//        //���
//        GLCALL(va.Unbind());
//        GLCALL(vb.Unbind());
//        GLCALL(ib.Unbind());
//        GLCALL(shader.Unbind());
//
//        //������Ⱦ��
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