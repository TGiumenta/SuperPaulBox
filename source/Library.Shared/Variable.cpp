#include "pch.h"
#include "Variable.h"
#include "ShaderProgram.h"

using namespace std;
using namespace glm;

namespace FieaGameEngine
{
    Variable::Variable(ShaderProgram& shaderProgram, const std::string& name) :
        mShaderProgram(shaderProgram), mName(name)
    {
        mLocation = glGetUniformLocation(mShaderProgram.Program(), name.c_str());
        if (mLocation == -1)
        {
            throw runtime_error("glGetUniformLocation() did not find uniform location.");
        }
    }

    ShaderProgram& Variable::GetShaderProgram()
    {
        return mShaderProgram;
    }

    const GLint& Variable::Location() const
    {
        return mLocation;
    }

    const std::string& Variable::Name() const
    {
        return mName;
    }

    Variable& Variable::operator<<(const mat4& value)
    {
        glUniformMatrix4fv(mLocation, 1, GL_FALSE, &value[0][0]);

        return *this;
    }

    Variable& Variable::operator<<(const vec4& value)
    {
        glUniform4fv(mLocation, 1, &value[0]);

        return *this;
    }

    Variable& Variable::operator<<(const vec3& value)
    {
        glUniform3fv(mLocation, 1, &value[0]);

        return *this;
    }

    Variable& Variable::operator<<(const vec2& value)
    {
        glUniform2fv(mLocation, 1, &value[0]);

        return *this;
    }

    Variable& Variable::operator<<(float value)
    {
        glUniform1f(mLocation, value);

        return *this;
    }

    Variable& Variable::operator<<(int value)
    {
        glUniform1i(mLocation, value);

        return *this;
    }
}