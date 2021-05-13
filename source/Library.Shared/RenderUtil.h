#pragma once
#include "HashMap.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "OrthographicCamera.h"
#include "ShaderProgram.h"

namespace FieaGameEngine
{
	class Entity;
	class Animation;

	struct TextureInfo
	{
		GLuint m_textureIndex;
		int m_width;
		int m_height;

		TextureInfo() = default;

		TextureInfo(GLuint textureIndex, int width, int height)
			: m_textureIndex(textureIndex), m_width(width), m_height(height) {};

	};


	class RenderUtil final
	{
	public:

		static void Init();

		inline static float GetUnitScale() { return s_UnitScale; }

		inline static const glm::vec3& GetDefaultCameraPosition() { return s_DefaultCameraPosition; }

		static TextureInfo LoadTexture(const std::string& filepath);

		static void Render(const Entity& entityToRender);

		static void Render(const Animation* animationInfo, const TextureInfo& textureInfo, glm::vec4 position, glm::vec4 rotation, glm::vec4 scale);

		static void Shutdown();

		static HashMap<const std::string, TextureInfo> s_textures;

		static OrthographicCamera& GetCamera();

	private:
		inline static const float s_UnitScale = 72.0f;

		inline static const float s_ScreenWidth = 1280.0f;
		inline static const float s_ScreenHeight = 720.0f;

		inline static const glm::vec3 s_DefaultCameraPosition{0, 0, 70};

		inline static ShaderProgram* m_shaderProgram = nullptr;
		inline static OrthographicCamera m_camera;
		inline static unsigned int m_VAO = 0;
		inline static unsigned int m_VBO = 0;
		inline static unsigned int m_worldViewProjLoc;
	};
}

