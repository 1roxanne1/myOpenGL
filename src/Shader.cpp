#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath),m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCALL(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name) const
{
    unsigned int location;
    //做统一变量缓存
    if (m_UniformlocationCache.find(name) != m_UniformlocationCache.end()) {
        return m_UniformlocationCache[name];
    }
    GLCALL( location= glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Waring: uniform ' " << name << "'doesn`t exist!" << std::endl;
    }
     m_UniformlocationCache[name] = location;
    return location;
}

//从文件读取着色器代码
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath); /* 这里没判断文件是否能正常打开 is_open */
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) { /* 找到#shader标记 */
            if (line.find("vertex") != std::string::npos) { /* 顶点着色器标记 */
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) { /* 片段着色器标记 */
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id;
    /* 提升作用域 */
    GLCALL(id = glCreateShader(type)); /* 创建对应类型的着色器 */
    const char* src = source.c_str();
    GLCALL(glShaderSource(id, 1, &src, nullptr)); /* 设置着色器源码 */
    GLCALL(glCompileShader(id)); /* 编译着色器 */

    /* 编译错误处理 */
    int result;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // 获取当前着色器编译状态
    if (result == GL_FALSE) {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // 获取日志长度
        char* msg = (char*)_malloca(length * sizeof(char)); /* Cherno这里采用的alloca, 根据IDE提示, 我这里改成了_malloca函数 */
        GLCALL(glGetShaderInfoLog(id, length, &length, msg)); // 获取日志信息
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << msg << std::endl;
        GLCALL(glDeleteShader(id)); // 删除着色器
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program;
    GLCALL(program = glCreateProgram()); /* 创建程序 */
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* 将着色器附加到程序上 */
    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    GLCALL(glLinkProgram(program)); /* 链接程序 */
    GLCALL(glValidateProgram(program)); /* 验证 */

    /* 删除着色器 */
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return program;
}