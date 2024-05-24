#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

//������ɫ����Ƭ����ɫ����Դ��ṹ��
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

//��һ���ļ��ж�ȡ������ɫ����Ƭ����ɫ����Դ��
static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath); //����һ����Ϊstream��������

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(),ss[1].str() };
}

//������ɫ��
static int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //��ȡ�ַ����׵�ַ
    glShaderSource(id, 1, &src, nullptr); //nullptr ��������Դ���룬ֻ�Ǹ��Ƶ���ɫ������
    glCompileShader(id);

    //������
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //�鿴����״̬
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));   //������ڴ�ռ��ں������غ���Զ��ͷ�
        glGetShaderInfoLog(id, length, &length, message);   //����ָ����ɫ���������Ϣ��־
        std::cout << "����" << (type == GL_VERTEX_SHADER ? "����" : "Ƭ��") << "��ɫ��ʧ��" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

//�������õĸ�����ɫ������
//vertexShader��������ɫ����fragmentShader��Ƭ����ɫ��
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    //������ɫ������
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    //������ɫ�����뵽program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program); //������κ����͵���ɫ�����󸽼ӵ������У����ǽ����ڴ������ڿɱ����ɫ�������еĿ�ִ���ļ�
    glValidateProgram(program); //�������а����Ŀ�ִ���ļ��Ƿ�����ڸ�����ǰ OpenGL ״̬��ִ��

    glDeleteShader(vs); //�Ѿ����ӵ�program��ɾ����ɫ������
    glDeleteShader(fs);

    return program;
}

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

        /*
            ������������
        */
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        //���㻺��
        VertexBuffer vb(position, 4 * 2 * sizeof(float));
        //���ö�������
        GLCALL(glEnableVertexAttribArray(0));
        GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0));
        //��������
        IndexBuffer ib(indices, 6);


        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);

        //ͳһ����
        GLCALL(int location = glGetUniformLocation(shader, "u_Color")); //ͳһ������������ʹ�ã������������ɾ��
        ASSERT(location != -1);
        GLCALL(glUniform4f(location, 0.7f, 0.3f, 0.8f, 1.0f));

        //�����
        GLCALL(glBindVertexArray(0)); //��������
        GLCALL(glUseProgram(0)); //��ɫ������
        GLCALL(vb.Unbind()); //���㻺����
        GLCALL(ib.Unbind());

        float r = 0.0f;
        float increment = 0.05f;

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            //��
            GLCALL(glUseProgram(shader));
            GLCALL(glBindVertexArray(vao));
            GLCALL(vb.Bind());
            GLCALL(ib.Bind());
   
            //��������������
            GLCALL(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
            GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
            glBindVertexArray(0);


            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;
            ;        /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        //������Դ
        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}