#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

//顶点着色器和片段着色器的源码结构体
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

//从一个文件中读取顶点着色器和片段着色器的源码
static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath); //创建一个名为stream的输入流

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

//编译着色器
static int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //获取字符串首地址
    glShaderSource(id, 1, &src, nullptr); //nullptr 代表不解析源代码，只是复制到着色器对象
    glCompileShader(id);

    //错误处理
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //查看编译状态
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));   //分配的内存空间在函数返回后会自动释放
        glGetShaderInfoLog(id, length, &length, message);   //返回指定着色器对象的信息日志
        std::cout << "编译" << (type == GL_VERTEX_SHADER ? "顶点" : "片段") << "着色器失败" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

//创建可用的复合着色器代码
//vertexShader：顶点着色器，fragmentShader：片段着色器
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    //编译着色器代码
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    //附加着色器代码到program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program); //如果将任何类型的着色器对象附加到程序中，它们将用于创建将在可编程着色器上运行的可执行文件
    glValidateProgram(program); //检查程序中包含的可执行文件是否可以在给定当前 OpenGL 状态下执行

    glDeleteShader(vs); //已经链接到program，删除着色器对象
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

        /*
            创建顶点数组
        */
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        //顶点缓冲
        VertexBuffer vb(position, 4 * 2 * sizeof(float));
        //启用顶点属性
        GLCALL(glEnableVertexAttribArray(0));
        GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0));
        //索引缓冲
        IndexBuffer ib(indices, 6);


        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);

        //统一变量
        GLCALL(int location = glGetUniformLocation(shader, "u_Color")); //统一变量声名必须使用，否则编译器会删除
        ASSERT(location != -1);
        GLCALL(glUniform4f(location, 0.7f, 0.3f, 0.8f, 1.0f));

        //解除绑定
        GLCALL(glBindVertexArray(0)); //顶点数组
        GLCALL(glUseProgram(0)); //着色器程序
        GLCALL(vb.Unbind()); //顶点缓冲区
        GLCALL(ib.Unbind());

        float r = 0.0f;
        float increment = 0.05f;

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            //绑定
            GLCALL(glUseProgram(shader));
            GLCALL(glBindVertexArray(vao));
            GLCALL(vb.Bind());
            GLCALL(ib.Bind());
   
            //索引缓冲区调用
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
        //回收资源
        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}