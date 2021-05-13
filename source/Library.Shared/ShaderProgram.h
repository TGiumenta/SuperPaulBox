#pragma once

#include "RTTI.h"
#include "Variable.h"
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <GL/glcorearb.h>

namespace FieaGameEngine
{

    typedef std::pair<GLenum, std::string> ShaderDefinition;

    class ShaderProgram : public RTTI
    {
        RTTI_DECLARATIONS(ShaderProgram, RTTI)

    public:
        ShaderProgram();
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram(ShaderProgram&&) = delete;
        ShaderProgram& operator=(ShaderProgram&&) = delete;
        virtual ~ShaderProgram();

        static GLuint CompileShaderFromFile(GLenum shaderType, const std::string& filename);

        GLuint Program() const;
        const std::vector<Variable*>& Variables() const;
        const std::map<std::string, Variable*>& VariablesByName() const;

        void BuildProgram(const std::vector<ShaderDefinition>& shaderDefinitions);

        virtual void Initialize(GLuint vertexArrayObject);
        virtual void Use() const;

    protected:
        std::map<std::string, Variable*> mVariablesByName;
        std::vector<Variable*> mVariables;
        GLuint mProgram;
    };

#define SHADER_VARIABLE_DECLARATION(VariableName)		                \
	public:													            \
		FieaGameEngine::Variable& VariableName() const;			        \
	private:												            \
		FieaGameEngine::Variable* m ## VariableName;

#define SHADER_VARIABLE_DEFINITION(ShaderProgram, VariableName)		    \
		FieaGameEngine::Variable& ShaderProgram::VariableName() const   \
		{																\
			return *m ## VariableName;									\
		}

#define SHADER_VARIABLE_INITIALIZATION(VariableName) m ## VariableName(nullptr)

#define SHADER_VARIABLE_INSTANTIATE(VariableName)															        \
		m ## VariableName = new Variable(*this, #VariableName);													    \
		mVariables.push_back(m ## VariableName);																    \
		mVariablesByName.insert(std::pair<std::string, Variable*>(m ## VariableName->Name(), m ## VariableName));
}
