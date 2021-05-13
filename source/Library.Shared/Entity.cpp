#include "pch.h"
#include "Entity.h"
#include "Scope.h"
#include "Action.h"
#include "Animation.h"


namespace FieaGameEngine
{
    RTTI_DEFINITIONS(Entity);

    Entity::Entity()
        : Attributed(Entity::TypeIdClass())
    {
    }

    Entity::Entity(RTTI::IdType type)
        : Attributed(type)
    {
    }

    const Vector<Signature> Entity::Signatures()
    {
        return Vector<Signature>
        {
            { "Name", Datum::DatumType::String, 1, offsetof(Entity, m_name) },
            { "Tag", Datum::DatumType::String, 1, offsetof(Entity, m_tag) },
            { "Texture", Datum::DatumType::String, 1, offsetof(Entity, m_textureFileName) },
            { "Position", Datum::DatumType::Vector, 1, offsetof(Entity, m_transform.m_position) },
            { "Rotation", Datum::DatumType::Vector, 1, offsetof(Entity, m_transform.m_rotation) },
            { "Scale", Datum::DatumType::Vector, 1, offsetof(Entity, m_transform.m_scale) },
            { "Children", Datum::DatumType::Table, 0, 0 },
            { "Actions", Datum::DatumType::Table, 0, 0 },
            { "Animations", Datum::DatumType::Table, 0, 0 },
            { "HasRelativePosition", Datum::DatumType::Integer, 1, offsetof(Entity, m_UseRelativePosition)},
            { "HasRelativeFacing", Datum::DatumType::Integer, 1, offsetof(Entity, m_UseRelativeFacing) }
        };
    }

    void Entity::SetTextureFile(std::string textureFile)
    {
        if (!textureFile.empty())
        {
            m_textureFileName = textureFile;
            m_textureInfo = RenderUtil::LoadTexture(m_textureFileName);
            m_hasTexture = true;
        }
    }

	void Entity::SetTextureInfo(TextureInfo textureInfo)
	{
		m_textureInfo = textureInfo;
	}

    void Entity::Init(WorldState* worldState)
    {
        // Load in the texture for this entity if it has one
        if (!m_textureFileName.empty())
        {
            m_textureInfo = RenderUtil::LoadTexture(m_textureFileName);
            m_hasTexture = true;
        }

        if (m_parent)
        {
            m_EntityParent = static_cast<Entity*>(m_parent);
        }

        // Init all child entities
        Datum& children = Children();
        for (size_t i = 0; i < children.Size(); ++i)
        {
            Scope* scopeChild = &children.Get<Scope>(i);
            assert(scopeChild->Is(Entity::TypeIdClass()));
            Entity& child = static_cast<Entity&>(*scopeChild);
            child.Init(worldState);
        }

        Datum& animations = Animations();
        for (size_t i = 0; i < animations.Size(); ++i)
        {
            Scope* animation = &animations.Get<Scope>(i);
            assert(animation->Is(Animation::TypeIdClass()));
            Animation& child = static_cast<Animation&>(*animation);
            
            m_Animations[child.GetStateName()] = &child;
            child.Initialize();
        }
        if (animations.Size() != 0)
        {
            m_hasAnimation = true;
            SetCurrentAnimation("Walking");
        }
    }

    void Entity::Update(WorldState* worldState)
    {
        Datum& children = Children();

        for (size_t i = 0; i < children.Size(); ++i)
        {
            Scope* scopeChild = &children.Get<Scope>(i);
            assert(scopeChild->Is(Entity::TypeIdClass()));
            Entity& child = static_cast<Entity&>(*scopeChild);
            child.Update(worldState);
        }

        Datum& actions = Actions();

        for (size_t i = 0; i < actions.Size(); ++i)
        {
            Scope* scopeChild = &actions.Get<Scope>(i);
            assert(scopeChild->Is(Action::TypeIdClass()));
            Action& child = static_cast<Action&>(*scopeChild);
            child.Update(worldState);
        }

        Datum& animations = Animations();
        for (size_t i = 0; i < animations.Size(); ++i)
        {
            Scope* scopeChild = &animations.Get<Scope>(i);
            assert(scopeChild->Is(Animation::TypeIdClass()));
            Animation& child = static_cast<Animation&>(*scopeChild);
            child.Update();
        }

        if (m_UseRelativeFacing && m_EntityParent)
        {
            SetFacingDirection(m_EntityParent->GetFacingDirection());
        }
    }

    void Entity::Render(WorldState* worldState)
    {
        if (m_hasTexture && m_IsVisible)
        {
            RenderUtil::Render(*this);
        }

        // Render all children
        Datum& children = Children();
        for (size_t i = 0; i < children.Size(); ++i)
        {
            Scope* scopeChild = &children.Get<Scope>(i);
            assert(scopeChild->Is(Entity::TypeIdClass()));
            Entity& child = static_cast<Entity&>(*scopeChild);
            child.Render(worldState);
        }
    }

    gsl::owner<Scope*> Entity::Clone() const
    {
        return new Entity(*this);
    }

    Action* Entity::CreateAction(const std::string& className, const std::string& instanceName)
    {
        Scope* createdScope = Factory<Scope>::Create(className);
        if (createdScope == nullptr)
        {
            throw std::exception("Trying to create an action whose class doesn't exist!");
        }

        Action* actionPtr = createdScope->As<Action>();
        if (actionPtr == nullptr)
        {
            delete createdScope;
            throw std::exception("Trying to create an action with a non-action class name!");
        }

        Adopt(*actionPtr, "Actions");
        actionPtr->SetName(instanceName);

        return actionPtr;
    }

    std::string Entity::GetCurrentAnimationState() const
    {
        if (m_CurrentAnimation)
        {
            return m_CurrentAnimation->GetStateName();
        }

        return "";
    }

    void Entity::SetFacingDirection(FacingDirection direction)
    {
        if (direction == FacingDirection::FACING_LEFT)
        {
            if (m_transform.m_scale.x > 0)
            {
                m_transform.m_scale.x *= -1;
            }
        }
        else
        {
            if (m_transform.m_scale.x < 0)
            {
                m_transform.m_scale.x *= -1;
            }
        }
    }

    FacingDirection Entity::GetFacingDirection() const
    {
        return (m_transform.m_scale.x > 0) ? FacingDirection::FACING_RIGHT : FacingDirection::FACING_LEFT;
    }

    glm::vec4 Entity::GetPosition() const
    {
        if (m_UseRelativePosition)
        {
            return GetRelativePosition();
        }
        else
        {
            return m_transform.m_position;
        }
    }

    void Entity::SetPosition(b2Vec2 newPosition)
    {
        m_transform.m_position = glm::vec4(newPosition.x, newPosition.y, 0.0f, 0.0f);
    }

    glm::vec4 Entity::GetRelativePosition() const
    {
        glm::vec4 relativePosition{ 0,0,0,0 };
        glm::vec4 thisPosition = m_transform.m_position;

        if (m_EntityParent)
        {
            relativePosition += m_EntityParent->GetRelativePosition();

            if (m_EntityParent->GetFacingDirection() == FacingDirection::FACING_LEFT)
            {
                thisPosition.x *= -1;
            }
        }

        return relativePosition + thisPosition;
    }
}