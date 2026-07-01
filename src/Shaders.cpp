#include "../include/Shaders.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>

#include "utils.h"

Shaders::Shaders()
{
    m_shaders = {};
}

Shaders::~Shaders()
{
    glDeleteProgram(m_programID);
    m_shaders.clear();
}

void Shaders::LoadShaders(const char* vPath, const char* fPath, const std::string& shaderTag)
{
    GLuint vertexShader, fragmentShader, ID;
    std::string vData, fData;

    try
    {
        // read in vertex shader
        std::ifstream vFile;
        std::stringstream vStream;

        vFile.open(vPath);
        vStream << vFile.rdbuf();
        vFile.close();

        vData = vStream.str();

        // read in fragment shader
        std::ifstream fFile;
        std::stringstream fStream;

        fFile.open(fPath);
        fStream << fFile.rdbuf();
        fFile.close();

        fData = fStream.str();
    }
    catch ([[maybe_unused]] std::ifstream::failure& err)
    {
        LOG_ERROR("Shader file(s) not read!");
    }

    // compile vertex shader
    const char* vCode = vData.c_str();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vCode, nullptr);

    glCompileShader(vertexShader);
    CheckCompileErrors(vertexShader, shaderTag, "VERTEX");

    // compile fragment shader
    const char* fCode = fData.c_str();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fCode, nullptr);

    glCompileShader(fragmentShader);
    CheckCompileErrors(fragmentShader, shaderTag, "FRAGMENT");

    // link shader to program
    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    CheckCompileErrors(ID, shaderTag, "LINKING", 'L');
	std::cout << std::endl;

    m_programID = ID;

    // cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_shaders[shaderTag] = m_programID;
}

GLuint Shaders::GetShaderID(const std::string& shaderTag) const
{
    return m_shaders.at(shaderTag);
}

void Shaders::Use(const std::string& shaderTag)
{
    GLuint ID = GetShaderID(shaderTag);
	glUseProgram(ID);
}

void Shaders::CheckCompileErrors(const unsigned int shader, const std::string& tag,
                                 const std::string& type, const char process)
{
    int success;
    char log[1028];

    if (process == 'C')
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1028, nullptr, log);
            LOG_ERROR(tag << " " << type << " SHADER COMPILATION FAILED: " << log);
        }
        else
        {
            LOG_INFO(tag << " " << type << " shader compilation success!\n");
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1028, nullptr, log);
            LOG_ERROR(tag << " LINKING FAILED: " << log);
        }
        else
        {
            LOG_INFO(tag << " linking success!\n");
        }
    }
}
