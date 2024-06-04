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
        //���㻺����
        float position[12] = {
            -0.5f, -0.5f,   //0
             0.5f, -0.5f,   //1
             0.5f,  0.5f,   //2
            -0.5f,  0.5f,   //3
        };
        //����������,ע�������������������޷��ŵģ�����
        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        //������������
        VertexArray va;

        //���㻺��
        VertexBuffer vb(position, 4 * 2 * sizeof(float));

        //���ö�������
        //1.�������ݲ���( glVertexAttribPointer )
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.addBuffer(vb, layout);

        //��������
        IndexBuffer ib(indices, 6);

        //������ɫ��
        Shader shader("res/shaders/Basic.shader");
        
        
        //ͳһ����
        shader.Bind();
        shader.SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

        //�����
        GLCALL(va.Unbind()); //��������
        GLCALL(vb.Unbind()); //���㻺����
        GLCALL(ib.Unbind());//��������
        GLCALL(shader.Unbind()); //��ɫ������

        float r = 0.0f;
        float increment = 0.05f;

        //��Ⱦ��
        Renderer renderer;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //��
            shader.Bind();
            //����ͳһ����
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