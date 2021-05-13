#pragma once
#include "GameTime.h"
#include "Scope.h"
#include "SList.h"
#include "EventQueue.h"
#include "Stack.h"

namespace FieaGameEngine
{
    class Entity;

	class WorldState final
	{
	public:

        WorldState();
        ~WorldState();

        /// <summary>
        /// Get the GameTime object associated with this WorldState
        /// </summary>
        /// <returns> The GameTime object associated with this WorldState </returns>
		GameTime& GetGameTime();

        EventQueue& GetEventQueue();

        void Clear();

        /// <summary>
        /// Set the GameTime object associated with this WorldState
        /// </summary>
        /// <returns> The GameTime object to associate with this WorldState </returns>
		void SetGameTime(GameTime& gameTime);

        /// <summary>
        /// Gets the change in time since the last tick in seconds
        /// </summary>
        /// <returns> The change in time since the last tick in seconds </returns>
        float GetDeltaTime() const;

        /// <summary>
        /// Completes all pending requests that were accumulated during the last update call.
        /// This method should be called directly after the main update call in the game loop.
        /// </summary>
        void CompletePendingRequests();

        /// <summary>
        /// Adds a destroy action request to the WorldState, to be processed on the next call to 
        /// CompletePendingRequests
        /// </summary>
        /// <param name="name"> The name of the action to destroy </param>
        /// <param name="context"> The starting context to use when searching for the action
        /// to destroy </param>
		void AddDestroyActionRequest(const std::string& name, Scope& context);

        /// <summary>
        /// Adds a create action request to the WorldState, to be processed on the next call to 
        /// CompletePendingRequests
        /// </summary>
        /// <param name="className"> The name of the class to construct </param>
        /// <param name="name"> The name to give the new Action </param>
        /// <param name="context"> The context to create this Action in </param>
        void AddCreateActionRequest(const std::string& className, std::string& name, Scope& context);

        /// <summary>
        /// Destroys an entity by removing it from its parent and deleting its memory. The destroy will actually
        /// occur next tick as to not mess with the current update loop.
        /// </summary>
        /// <param name="entity"></param>
        void DestroyEntity(Entity* entity);

        /// <summary>
        /// Instantiate methods used to spawn entities at runtime
        /// </summary>
        #pragma region Instantiate Methods
        Entity* Instantiate(const std::string& className, const std::string& jsonFileName = "");
        Entity* Instantiate(const std::string& className, const std::string& jsonFileName, Entity* parent);
        Entity* Instantiate(const std::string& className, const std::string& jsonFileName, glm::vec4 position, Entity* parent = nullptr);
        Entity* Instantiate(const std::string& className, Entity* parent);
        Entity* Instantiate(const std::string& className, glm::vec4 position, Entity* parent = nullptr);
        #pragma endregion

        Stack<Scope*> m_scopeStack;

	private:
        /// <summary>
        /// Struct that represents a queued DestroyAction request
        /// </summary>
        struct DestroyActionRequest
        {
            const std::string& m_name;
            Scope& m_context;

            DestroyActionRequest(const std::string& name, Scope& context)
                : m_name(name), m_context(context) {}
        };

        /// <summary>
        /// Struct that represents a queued CreateAction request
        /// </summary>
        struct CreateActionRequest
        {
            const std::string& m_className;
            const std::string& m_name;
            Scope& m_context;

            CreateActionRequest(const std::string& className, const std::string& name, Scope& context)
                : m_className(className), m_name(name), m_context(context) {}
        };

        /// <summary>
        /// Struct that represents a queued CreateEntity request
        /// </summary>
        struct CreateEntityRequest
        {
            Entity* m_entity;
            Entity* m_parentEntity;

            CreateEntityRequest(Entity* entity, Entity* parentEntity = nullptr)
                : m_entity(entity), m_parentEntity(parentEntity) {}
        };

        /// <summary>
        /// Struct that represents a queued DestroyEntity request
        /// </summary>
        struct DestroyEntityRequest
        {
            Entity* m_Entity;

            DestroyEntityRequest(Entity* entity)
                : m_Entity(entity) {}
        };

        /// <summary>
        /// List of queued destroy action requests
        /// </summary>
		SList<DestroyActionRequest> m_destroyActionRequests;

        /// <summary>
        /// List of queued create action requests
        /// </summary>
        SList<CreateActionRequest> m_createActionRequests;

        /// <summary>
        /// Queue of entities to be created
        /// </summary>
        SList<CreateEntityRequest> m_createEntityQueue;

        /// <summary>
        /// Queue of entities to be destroyed
        /// </summary>
        SList<DestroyEntityRequest> m_destroyEntityQueue;

        /// <summary>
        /// The GameTime object associated with this WorldState.
        /// </summary>
		GameTime m_gameTime;

        /// <summary>
        /// 
        /// </summary>
        EventQueue* m_eventQueue;

        /// <summary>
        /// Recursive function that attempts to find the target action and then destroy it. This will
        /// recurse up the hierarchy searching for the action.
        /// </summary>
        /// <param name="name"> The action name to search for </param>
        /// <param name="context"> The starting context to search </param>
        /// <returns> True if the action was found and deleted, false otherwise </returns>
        bool AttemptDestroyAction(const std::string& name, Scope& context);

        void AttemptCreateEntity(Entity& entity, Entity* parent);
        void AttemptDestroyEntity(Entity& entity);

        Entity* CreateEntityFromJSONHelper(const std::string& className, const std::string& jsonFileName);
        Entity* CreateDefaultEntityHelper(const std::string& className);
	};
}

