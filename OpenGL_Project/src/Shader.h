#pragma once
#include <string>
#include <unordered_map>

#include <glm\glm.hpp>


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader(const std::string& filepath);
    ~Shader();

    
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformVec3f(const std::string& name, const glm::vec3& vector);
    void SetUniformGLMVec3f(const std::string& name, const glm::vec3& vector);
    void SetUniformVec3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform1i(const std::string& name, int value);
    void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void Bind() const;
    void Unbind() const;

private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int GetUniformLocation(const std::string& name);
};