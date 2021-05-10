#include "ShaderProgram.hpp"
#include<fstream>
#include<iostream>

Shader::Shader()
{
    m_programID = glCreateProgram();
}

void Shader::AttachShader(const char* fileName, unsigned int shaderType)
{
    GLuint shaderID = glCreateShader(shaderType);

    std::string sourceCode = getShaderFromFile(fileName);

    const char* chSourceCode = &sourceCode[0];

    glShaderSource(shaderID, 1, &chSourceCode, 0);

    glCompileShader(shaderID);

    int isComplied;
    char log[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isComplied);
    if (!isComplied)
    {
        glGetShaderInfoLog(shaderID, 512, 0, log);
        std::cout << "Error Shader Type: " << shaderType << std::endl << log << std::endl;
    }

    glAttachShader(m_programID, shaderID);

    glDeleteShader(shaderID);
}


void Shader::link()
{
    glLinkProgram(m_programID);
}

void Shader::use()
{
    glUseProgram(m_programID);
}

void Shader::addUniform(const std::string varName)
{
    m_UniformVars[varName] = glGetUniformLocation(m_programID, varName.c_str());
}

void Shader::setvec3(const std::string& varName, const glm::vec3& value)
{
    glUniform3f(m_UniformVars[varName], value.x, value.y, value.z);
}

void Shader::setvec4(const std::string& varName, const glm::vec4& value)
{
    glUniform4f(m_UniformVars[varName], value.r, value.g, value.b, value.a);
}

void Shader::setMat3(const std::string& varName, const glm::mat3* ptrValue)
{
    glUniformMatrix3fv(m_UniformVars[varName], 1, false, (GLfloat*)ptrValue);
}
void Shader::setMat4(const std::string& varName, const glm::mat4* mtx)
{
    glUniformMatrix4fv(m_UniformVars[varName], 1, false, (GLfloat*)mtx);
}

std::string Shader::getShaderFromFile(const char* fileName)
{
    std::ifstream file(fileName);

    std::string data;

    if (file.is_open())
    {
        char readChar;

        while ((readChar = file.get()) != EOF)
        {
            data += readChar;
        }
        file.close();
    }
    return data;
}


void Shader::setFloat(const std::string& varName, const float& vec)
{
    glUniform1f(m_UniformVars[varName], vec);
}
void Shader::setInt(const std::string& varName, unsigned int val)
{
    glUniform1i(m_UniformVars[varName], val);
}