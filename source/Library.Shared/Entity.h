#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "Factory.h"
#include "RenderUtil.h"
#include "HashMap.h"
#include <box2d/box2d.h>

namespace FieaGameEngine
{
    class WorldState;
    class Action;
    class Animation;

    /// <summary>
    /// Transform struct used to keep track of position, rotation, and scale
    /// </summary>
    struct Transform
    {
        glm::vec4 m_position{ 0, 0, 0, 0 };
        glm::vec4 m_rotation{ 0, 0, 0, 0 };
        glm::vec4 m_scale{ 1, 1, 1, 0 };
    };

    enum class FacingDirection
    {
        FACING_RIGHT,
        FACING_LEFT
    };

    /// <summary>
    /// Entity class that serves as a base class for all objects in the game
    /// </summary>
	class Entity : public Attributed
	{
        RTTI_DECLARATIONS(Entity, Attributed);

	public:
        /// <summary>
        /// Default constructor
        /// </summary>
        Entity();

        /// <summary>
        /// Constructor used to pass the class ID to the attributed constructor
        /// </summary>
        /// <param name="type"></param>
        Entity(RTTI::IdType type);
        Entity(const Entity& other) = default;
        Entity(Entity&& other) = default;
        Entity& operator=(const Entity& other) = default;
        Entity& operator=(Entity&& other) = default;
        virtual ~Entity() = default;

        /// <summary>
        /// Sets the name of this Entity
        /// </summary>
        /// <param name="name"> The name to set this Entity to </param>
        inline void SetName(const std::string& name) { m_name = name; };

        /// <summary>
        /// Gets the name of this Entity
        /// </summary>
        /// <returns> The name of this Entity </returns>
        inline std::string GetName() { return m_name; };

        /// <summary>
        /// Sets the tag of this Entity
        /// </summary>
        /// <param name="tag"> The tag to set this Entity to </param>
        inline void SetTag(const std::string& tag) { m_tag = tag; };

        /// <summary>
        /// Gets the tag of this Entity
        /// </summary>
        /// <returns> The tag to set this Entity to </returns>
        inline std::string GetTag() { return m_tag; };

        /// <summary>
        /// Gets the child entities of this Entity
        /// </summary>
        /// <returns> A datum containing the child entities </returns>
        inline Datum& Children() { return m_order[m_childrenIndex]->second; };

        /// <summary>
        /// Gets the child actions of this Entity
        /// </summary>
        /// <returns> A datum containing the child actions </returns>
        inline Datum& Actions() { return m_order[m_actionsIndex]->second; };

        inline Datum& Animations() { return m_order[m_animationsIndex]->second; }

        inline const TextureInfo& GetTextureInfo() const { return m_textureInfo; };

        inline void SetCurrentAnimation(const std::string& animation) { m_CurrentAnimation = m_Animations[animation]; };

        inline const Animation* GetCurrentAnimation() const { return m_CurrentAnimation; }

        std::string GetCurrentAnimationState() const;

        void SetFacingDirection(FacingDirection direction);
        FacingDirection GetFacingDirection() const;

        #pragma region Transform
        /// <summary>
        /// Gets the position of this Entity
        /// </summary>
        /// <returns> A vec4 representing the position of this Entity</returns>
        virtual glm::vec4 GetPosition() const;

        virtual void SetPosition(b2Vec2 newPosition);

        /// <summary>
        /// Gets the relative position of this Entity
        /// </summary>
        /// <returns> A vec4 representing the rellative position of this Entity</returns>
        virtual glm::vec4 GetRelativePosition() const;

        /// <summary>
        /// Gets the rotation of this Entity
        /// </summary>
        /// <returns> A vec4 representing the rotation of this Entity</returns>
        virtual glm::vec4 GetRotation() const { return m_transform.m_rotation; };

        /// <summary>
        /// Gets the scale of this Entity
        /// </summary>
        /// <returns> A vec4 representing the scale of this Entity</returns>
        virtual glm::vec4 GetScale() const { return m_transform.m_scale; };
        #pragma endregion Transform

        /// <summary>
        /// Retrieves the Signatures for this class, which describe its prescribed 
        /// attributes
        /// </summary>
        /// <returns> A Vector of Signatures representing the prescribed attributes
        /// of this class </returns>
        static const Vector<Signature> Signatures();

        /// <summary>
        /// Sets the entity's texture
        /// </summary>
        /// <param name="textureFile"> The string representing the texture file </param>
        void SetTextureFile(std::string textureFile);

		/// <summary>
		/// Sets the entity's textureInfo. Allows use of already loaded textureInfos.
		/// </summary>
		/// <param name="textureFile"> The string representing the texture file </param>
		void SetTextureInfo(TextureInfo textureInfo);

        void SetVisibility(bool isVisible) { m_IsVisible = isVisible; }

        void SetIsPendingDestruction(bool isPending) { m_isPendingDestruction = isPending; }

        /// <summary>
        /// Returns whether or not this object is pending destruction
        /// </summary>
        /// <returns> True if this object is pending destruction, false otherwise </returns>
        bool IsPendingDestruction() const { return m_isPendingDestruction; }

        /// <summary>
        /// Initializes this entity. This should be called before the first time update is
        /// called on this entity.
        /// </summary>
        /// <param name="worldState"></param>
        virtual void Init(WorldState* worldState);

        /// <summary>
        /// Updates this Entity and all of its child Entities
        /// </summary>
        /// <param name="worldState"> The current world state </param>
        virtual void Update(WorldState* worldState);

        /// <summary>
        /// Renders this Entity at its current position, rotation, and scale
        /// </summary>
        /// <param name="worldState"> The current world state </param>
        virtual void Render(WorldState* worldState);

        /// <summary>
        /// Creates a new heap allocated copy of this Entity
        /// </summary>
        /// <returns> The newly heap-allocated entity </returns>
        virtual gsl::owner<Scope*> Clone() const override;

        Action* CreateAction(const std::string& className, const std::string& instanceName);

	protected:
        /// <summary>
        /// The name of this Entity
        /// </summary>
        std::string m_name;

        /// <summary>
        /// This Entity's tag
        /// </summary>
        std::string m_tag;
        
        /// <summary>
        /// This Entity's transform
        /// </summary>
        Transform m_transform;

        /// <summary>
        /// The name of the texture that belongs to this entity 
        /// </summary>
        std::string m_textureFileName;

        /// <summary>
        /// This entity's texture info, filled out in Init if m_textureFileName is set
        /// </summary>
        TextureInfo m_textureInfo{};

        /// <summary>
        /// The current Animation of this Entity
        /// </summary>
        Animation* m_CurrentAnimation = nullptr;

        /// <summary>
        /// Map of Strings to Animations, in order to quickly query for the animation we need on state swap
        /// </summary>
        HashMap<const std::string, Animation*> m_Animations;

    private:
        /// <summary>
        /// The index in this Entity's scope where the child datum resides
        /// </summary>
        static const size_t m_childrenIndex = 7;
        static const size_t m_actionsIndex = 8;
        static const size_t m_animationsIndex = 9;

        /// <summary>
        /// Whether or not this entity has a texture. If it doesn't have one, it won't be rendered
        /// </summary>
        bool m_hasTexture = false;

        bool m_IsVisible = true;

        /// <summary>
        /// Whether or not this entity has an animation.
        /// </summary>
        bool m_hasAnimation = false;

        /// <summary>
        /// Whether or not this entity is pending destruction.
        /// </summary>
        bool m_isPendingDestruction = false;

        /// <summary>
        /// Whether or not this entity should use relative position. If this is 1, the entity will add its
        /// position to that of all its ancestor's combined.
        /// </summary>
        int m_UseRelativePosition = 0;

        int m_UseRelativeFacing = 0;

        /// <summary>
        /// The cached parent of this Entity stored as an Entity for ease of access
        /// </summary>
        Entity* m_EntityParent = nullptr;

        
	};

    ConcreteFactory(Entity, Scope);
}

