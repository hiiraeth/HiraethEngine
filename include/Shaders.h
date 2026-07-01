#ifndef HIRAETHENGINE_SHADERS_H
#define HIRAETHENGINE_SHADERS_H

#include <map>
#include <string>
#include <glm/glm.hpp>

#include "glad/glad.h"

class Shaders
{

public:
    GLuint m_programID;
    std::map<std::string, unsigned int> m_shaders;

	Shaders();
    ~Shaders();

    // create new shaders
    void LoadShaders(const char* vPath, const char* fPath,
        const std::string& shaderTag);

    // get m_programID for shader based off tag
    GLuint GetShaderID(const std::string &shaderTag) const;

    // set shader active
    void Use(const std::string &shaderTag);

     void setBool(const std::string &name, bool value) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform1i(glGetUniformLocation(currentProgram, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform1i(glGetUniformLocation(currentProgram, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform1f(glGetUniformLocation(currentProgram, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform2fv(glGetUniformLocation(currentProgram, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform2f(glGetUniformLocation(currentProgram, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform3fv(glGetUniformLocation(currentProgram, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform3f(glGetUniformLocation(currentProgram, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform4fv(glGetUniformLocation(currentProgram, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform4f(glGetUniformLocation(currentProgram, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniformMatrix2fv(glGetUniformLocation(currentProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniformMatrix3fv(glGetUniformLocation(currentProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniformMatrix4fv(glGetUniformLocation(currentProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

	void setSampler2DValue(const std::string& name, const int &value) const
	{
     	GLint currentProgram;
     	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
     	glUniform1i(glGetUniformLocation(currentProgram, name.c_str()), value);
	}

private:
    // check for errors during shader creation
    static void CheckCompileErrors(unsigned int shader, const std::string& tag,
        const std::string& type, char process='C');
};


#endif //INC_3D_SHADERS_H