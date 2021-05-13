#pragma once

#include <GL/glcorearb.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <string>

namespace FieaGameEngine
{
    class ShaderProgram;

    class Variable final
    {
    public:
        Variable(ShaderProgram& shaderProgram, const std::string& name);
        Variable(const Variable&) = delete;
        Variable& operator=(const Variable&) = delete;
        Variable(Variable&&) = delete;
        Variable& operator=(Variable&&) = delete;
        ~Variable() = default;

        ShaderProgram& GetShaderProgram();
        const GLint& Location() const;
        const std::string& Name() const;

        Variable& operator<<(const glm::mat4& value);
        Variable& operator<<(const glm::vec4& value);
        Variable& operator<<(const glm::vec3& value);
        Variable& operator<<(const glm::vec2& value);
        Variable& operator<<(float value);
        Variable& operator<<(int value);

    private:
        ShaderProgram& mShaderProgram;
        GLint mLocation = -1;
        std::string mName;
    };
}