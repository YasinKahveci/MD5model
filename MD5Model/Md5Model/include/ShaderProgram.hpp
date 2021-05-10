#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "string"
#include <map>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>

class Shader
{
public:
    Shader();
    void use();

    void link();

    void AttachShader(const char* fileName, unsigned int shaderType);

    void addUniform(const std::string varName);

    void setvec3(const std::string& varName, const glm::vec3& value);
    void setvec4(const std::string& varName, const glm::vec4& value);
    void setMat3(const std::string& varName, const glm::mat3* value);
    void setMat4(const std::string& varName, const glm::mat4* mtx);
    void setFloat(const std::string& varName, const float& vec);
    void setInt(const std::string& varName, unsigned int val);
private:
    GLuint m_programID;
    std::string getShaderFromFile(const char* fileName);
    std::map<std::string, unsigned int> m_UniformVars;
};
#endif