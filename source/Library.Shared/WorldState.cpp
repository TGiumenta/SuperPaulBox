#include "pch.h"
#include "WorldState.h"
#include "Action.h"
#include "Entity.h"
#include "Game.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"

using namespace std::chrono;

namespace FieaGameEngine
{
    WorldState::WorldState()
    {
        m_eventQueue = new EventQueue(m_gameTime);
    }

    WorldState::~WorldState()
    {
        delete m_eventQueue;
    }

    GameTime& WorldState::GetGameTime()
    {
        return m_gameTime;
    }

    EventQueue& WorldState::GetEventQueue()
    {
        return *m_eventQueue;
    }

    void WorldState::Clear()
    {
        m_eventQueue->Clear();
        m_createEntityQueue.Clear();
        m_destroyEntityQueue.Clear();
        m_createActionRequests.Clear();
        m_destroyActionRequests.Clear();
    }

    void WorldState::SetGameTime(GameTime& gameTime)
    {
        m_gameTime = gameTime;
    }

    float WorldState::GetDeltaTime() const
    {
        auto& ms = m_gameTime.ElapsedGameTime();

        float deltaTime = static_cast<float>(ms.count());

        deltaTime /= 1000;

        return deltaTime;
    }

    void WorldState::CompletePendingRequests()
    {
        m_eventQueue->Update();

        // Perform pending destroy action requests
        for (DestroyActionRequest& request : m_destroyActionRequests)
        {
            AttemptDestroyAction(request.m_name, request.m_context);
        }
        m_destroyActionRequests.Clear();

        // Perform pending create action requests
        for (CreateActionRequest& request : m_createActionRequests)
        {
            Scope* newScope = Factory<Scope>::Create(request.m_className);
            if (newScope == nullptr)
            {
                throw std::exception("Trying to create an Action with a class who has no factory!");
            }

            Action* newAction = newScope->As<Action>();

            if (newAction == nullptr)
            {
                delete newScope;
                throw std::exception("Trying to create an Action with a non-action class name!");
            }

            newAction->SetName(request.m_name);
            request.m_context.Adopt(*newAction, "Actions");
        }
        m_createActionRequests.Clear();

        for (CreateEntityRequest& entityRequest : m_createEntityQueue)
        {
            AttemptCreateEntity(*entityRequest.m_entity, entityRequest.m_parentEntity);
        }
        m_createEntityQueue.Clear();

        for (DestroyEntityRequest& entity : m_destroyEntityQueue)
        {
            AttemptDestroyEntity(*entity.m_Entity);
        }
        m_destroyEntityQueue.Clear();
    }

    void WorldState::AddDestroyActionRequest(const std::string& name, Scope& context)
    {
        m_destroyActionRequests.PushBack(DestroyActionRequest(name, context));
    }

    void WorldState::AddCreateActionRequest(const std::string& className, std::string& name, Scope& context)
    {
        m_createActionRequests.PushBack(CreateActionRequest(className, name, context));
    }

    void WorldState::DestroyEntity(Entity* entity)
    {
        if (!entity->IsPendingDestruction())
        {
            m_destroyEntityQueue.PushBack(entity);
            entity->SetIsPendingDestruction(true);
        }
    }

    bool WorldState::AttemptDestroyAction(const std::string& name, Scope& context)
    {
        Datum* actionsDatum = context.Find("Actions");
        bool elementFound = false;

        if (actionsDatum)
        {
            for (size_t i = 0; i < actionsDatum->Size(); ++i)
            {
                Scope& scope = actionsDatum->Get<Scope>(i);
                Action& action = static_cast<Action&>(scope);

                if (action.GetName() == name)
                {
                    delete& action;
                    actionsDatum->RemoveAt(i);
                    elementFound = true;
                    break;
                }
            }
        }

        if (!elementFound && context.GetParent() != nullptr)
        {
            elementFound = AttemptDestroyAction(name, *context.GetParent());
        }

        return elementFound;
    }
   
    void WorldState::AttemptCreateEntity(Entity& entity, Entity* parent)
    {
        // If the request had a parent in it, add this entity as a child of it,
        // otherwise adopt the child into the world itself
        if (parent)
        {
            parent->Adopt(entity, "Children");
        }
        else
        {
            Game::GetGame()->GetRootEntity().Adopt(entity, "Children");
        }
    }

    void WorldState::AttemptDestroyEntity(Entity& entity)
    {
        if (entity.GetParent())
        {
            entity.Orphan();
        }

        delete &entity;
    }

    Entity* WorldState::Instantiate(const std::string& className, const std::string& jsonFileName)
    {
        Entity* entity = CreateEntityFromJSONHelper(className, jsonFileName);
        entity->Init(this);
        m_createEntityQueue.PushBack(CreateEntityRequest(entity));
        return entity;
    }

    Entity* WorldState::Instantiate(const std::string& className, const std::string& jsonFileName, Entity* parent)
    {
        Entity* entity = CreateEntityFromJSONHelper(className, jsonFileName);
        entity->Init(this);
        m_createEntityQueue.PushBack(CreateEntityRequest(entity, parent));
        return entity;
    }

    Entity* WorldState::Instantiate(const std::string& className, const std::string& jsonFileName, glm::vec4 position, Entity* parent)
    {
        Entity* entity = CreateEntityFromJSONHelper(className, jsonFileName);
        entity->SetPosition(b2Vec2(position.x, position.y));
        entity->Init(this);
        m_createEntityQueue.PushBack(CreateEntityRequest(entity, parent));
        return entity;
    }

    Entity* WorldState::Instantiate(const std::string& className, Entity* parent)
    {
        Entity* entity = CreateDefaultEntityHelper(className);
        entity->Init(this);
        m_createEntityQueue.PushBack(CreateEntityRequest(entity, parent));
        return entity;
    }

    Entity* WorldState::Instantiate(const std::string& className, glm::vec4 position, Entity* parent)
    {
        Entity* entity = CreateDefaultEntityHelper(className);
        entity->SetPosition(b2Vec2(position.x, position.y));
        entity->Init(this);
        m_createEntityQueue.PushBack(CreateEntityRequest(entity, parent));
        return entity;
    }

    Entity* WorldState::CreateEntityFromJSONHelper(const std::string& className, const std::string& jsonFileName)
    {
        Scope* scope = Factory<Scope>::Create(className);
        Entity* entity = scope->As<Entity>();
        assert(entity != nullptr);

        if (!jsonFileName.empty())
        {
            JsonTableParseHelper tableHelper;
            JsonTableParseHelper::SharedData sharedData(*entity);
            JsonParseCoordinator coordinator(sharedData);
            coordinator.AddHelper(tableHelper);
            coordinator.ParseFromFile(jsonFileName);
        }

        return entity;
    }

    Entity* WorldState::CreateDefaultEntityHelper(const std::string& className)
    {
        Scope* scope = Factory<Scope>::Create(className);
        Entity* entity = scope->As<Entity>();
        assert(entity != nullptr);
        return entity;
    }
}