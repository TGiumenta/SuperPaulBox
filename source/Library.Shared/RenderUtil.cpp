#include "pch.h"
#include "RenderUtil.h"
#include <iostream>
#include "Entity.h"
#include "Animation.h"
#include "VertexDeclarations.h"
#include "ShaderUtil.h"
#include "ShaderProgram.h"
#include "OrthographicCamera.h"
#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace FieaGameEngine
{
    HashMap<const std::string, TextureInfo> RenderUtil::s_textures;

    void RenderUtil::Init()
    {
        // Build the shader program
        m_shaderProgram = new ShaderProgram();
        std::vector<ShaderDefinition> shaders;
        shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "shader.vert"));
        shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "shader.frag"));
        m_shaderProgram->BuildProgram(shaders);

        // Create vertices and texture coords for each corner of the quad
        const VertexPositionTexture vertices[] =
        {
            VertexPositionTexture(glm::vec4(-1.0f, +1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
            VertexPositionTexture(glm::vec4(+1.0f, +1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
            VertexPositionTexture(glm::vec4(+1.0f, -1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
            VertexPositionTexture(glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f))
        };

        // Create vertex buffer
        VertexPositionTexture::CreateVertexBuffer(vertices, m_VBO);

        // Create vertex array object
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        // Create position attribute
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position));
        glEnableVertexAttribArray(0);

        // Create texture coordinates attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // Get the location of the WorldViewProjection matrix input to the shader
        m_worldViewProjLoc = glGetUniformLocation(m_shaderProgram->Program(), "WorldViewProjection");
        if (m_worldViewProjLoc == -1)
        {
            throw std::runtime_error("glGetUniformLocation() did not find uniform location.");
        }

        // Turn on transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Move the camera so we can see correctly
        m_camera.SetPosition(s_DefaultCameraPosition);
    }

    TextureInfo RenderUtil::LoadTexture(const std::string& filepath)
    {
        // Check if we have already loaded this texture in
        auto it = s_textures.Find(filepath);
        if (it != s_textures.end())
        {
            // If we have already loaded this texture in, just return it
            return it->second;
        }

        stbi_set_flip_vertically_on_load(true);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            assert(FALSE);
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        // Add this newly loaded texture to our hashmap
        s_textures[filepath] = TextureInfo(texture, width, height);

        return TextureInfo(texture, width, height);
    }

    void RenderUtil::Render(const Entity& entityToRender)
    {
        Render(entityToRender.GetCurrentAnimation(), entityToRender.GetTextureInfo(), entityToRender.GetPosition(), entityToRender.GetRotation(), entityToRender.GetScale());
    }

    void RenderUtil::Render(const Animation* animation, const TextureInfo& textureInfo, glm::vec4 position, glm::vec4 rotation, glm::vec4 scale )
    {
        // Load in our vertex array and buffer
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        TextureInfo textureToUse = (animation) ? animation->GetTextureInfo() : textureInfo;
        glm::vec2 topLeft, topRight, bottomRight, bottomLeft;

        if (animation != nullptr)
        {
            TextureCoords coordinates = animation->GetTextureCoords();
            topLeft = { coordinates.m_ULeft, coordinates.m_VTop };
            topRight = { coordinates.m_URight, coordinates.m_VTop };
            bottomRight = {coordinates.m_URight, coordinates.m_VBottom};
            bottomLeft = {coordinates.m_ULeft, coordinates.m_VBottom};
        }
        else
        {
            topLeft = { 0.f, 1.0f };
            topRight = { 1.0f, 1.0f };
            bottomRight = { 1.0f, 0.0f };
            bottomLeft = { 0.0f, 0.f };
        }

        const VertexPositionTexture vertices[] =
        {
            VertexPositionTexture(glm::vec4(-1.0f, +1.0f, 0.0f, 1.0f), topLeft),
            VertexPositionTexture(glm::vec4(+1.0f, +1.0f, 0.0f, 1.0f), topRight),
            VertexPositionTexture(glm::vec4(+1.0f, -1.0f, 0.0f, 1.0f), bottomRight),
            VertexPositionTexture(glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), bottomLeft)
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Use the shaders we loaded in
        glUseProgram(m_shaderProgram->Program());

        // Update the camera's projection matrix in case it has moved
        m_camera.UpdateProjectionMatrix();

        glm::vec4 adjustedScale = scale * 0.5f;

        // Calculate the world matrix based on the location and scale of the object
        glm::mat4 mWorldMatrix{ 1.0f };
        glm::mat4 translationMat = glm::translate(glm::mat4(1), glm::vec3(position));

        float rotationInRadians = glm::radians(rotation.z);
        glm::mat4 rotationMat = glm::rotate(glm::mat4(1), rotationInRadians, glm::vec3(0,0,1));
        glm::mat4 scaleMat = glm::scale(glm::mat4(1), glm::vec3(adjustedScale));
        mWorldMatrix = translationMat * rotationMat * scaleMat;

        // multiply in the camera projection matrix so we can go into view space
        glm::mat4 wvp = m_camera.ProjectionMatrix() * mWorldMatrix;
        glUniformMatrix4fv(m_worldViewProjLoc, 1, GL_FALSE, &wvp[0][0]);

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, textureToUse.m_textureIndex);

        // Draw the object
        const GLsizei vertexCount = 4;
        glDrawArrays(GL_QUADS, 0, vertexCount);

        // Clear the vertex array for safety purposes
        glBindVertexArray(0);
    }

    void RenderUtil::Shutdown()
    {
        delete m_shaderProgram;
    }

    OrthographicCamera& RenderUtil::GetCamera()
    {
        return m_camera;
    }
}