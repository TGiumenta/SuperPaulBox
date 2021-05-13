#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <gsl/gsl>

namespace FieaGameEngine
{
    template <typename T>
    struct VertexDeclaration
    {
        static constexpr uint32_t VertexSize() { return gsl::narrow_cast<uint32_t>(sizeof(T)); }
        static constexpr uint32_t VertexBufferByteWidth(size_t vertexCount) { return gsl::narrow_cast<uint32_t>(sizeof(T) * vertexCount); }
        static void CreateVertexBuffer(const gsl::span<const T>& vertices, GLuint& vertexBuffer)
        {
            glGenBuffers(1, &vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, VertexSize() * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        };
    };

    struct VertexPosition final : public VertexDeclaration<VertexPosition>
    {
        glm::vec4 Position{ 0 };

        VertexPosition() = default;

        VertexPosition(const glm::vec4& position) :
            Position(position) { }

        static void CreateVertexBuffer(const gsl::span<const VertexPosition>& vertices, GLuint& vertexBuffer)
        {
            VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
        }
    };

    struct VertexPositionColor final :public VertexDeclaration<VertexPositionColor>
    {
        glm::vec4 Position{ 0 };
        glm::vec4 Color{ 0 };

        VertexPositionColor() = default;

        VertexPositionColor(const glm::vec4& position, const glm::vec4& color) :
            Position(position), Color(color) { }

        static void CreateVertexBuffer(const gsl::span<const VertexPositionColor>& vertices, GLuint& vertexBuffer)
        {
            VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
        }
    };

    struct VertexPositionTexture final : public VertexDeclaration<VertexPositionTexture>
    {
        glm::vec4 Position{ 0 };
        glm::vec2 TextureCoordinates{ 0 };

        VertexPositionTexture() = default;

        VertexPositionTexture(const glm::vec4& position, const glm::vec2& textureCoordinates) :
            Position(position), TextureCoordinates(textureCoordinates) { }

        static void CreateVertexBuffer(const gsl::span<const VertexPositionTexture>& vertices, GLuint& vertexBuffer)
        {
            VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
        }
    };


    struct VertexPositionTextureNormal final : public VertexDeclaration<VertexPositionTextureNormal>
    {
        glm::vec4 Position{ 0 };
        glm::vec2 TextureCoordinates{ 0 };
        glm::vec3 Normal{ 0 };

        VertexPositionTextureNormal() = default;

        VertexPositionTextureNormal(const glm::vec4& position, const glm::vec2& textureCoordinates, const glm::vec3& normal) :
            Position(position), TextureCoordinates(textureCoordinates), Normal(normal) { }

        static void CreateVertexBuffer(const gsl::span<const VertexPositionTextureNormal>& vertices, GLuint& vertexBuffer)
        {
            VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
        }
    };

    struct VertexPositionTextureNormalTangentBinormal final : public VertexDeclaration<VertexPositionTextureNormalTangentBinormal>
    {
        glm::vec4 Position{ 0 };
        glm::vec2 TextureCoordinates{ 0 };
        glm::vec3 Normal{ 0 };
        glm::vec3 Tangent{ 0 };
        glm::vec3 Binormal{ 0 };

        VertexPositionTextureNormalTangentBinormal() = default;

        VertexPositionTextureNormalTangentBinormal(const glm::vec4& position, const glm::vec2& textureCoordinates, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& binormal) :
            Position(position), TextureCoordinates(textureCoordinates), Normal(normal), Tangent(tangent), Binormal(binormal) { }

        static void CreateVertexBuffer(const gsl::span<const VertexPositionTextureNormalTangentBinormal>& vertices, GLuint& vertexBuffer)
        {
            VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
        }
    };
}


