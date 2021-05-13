#pragma once
#include "Entity.h"
#include "WorldContactListener.h"
#include <box2d/box2d.h>
#include "CollisionDefines.h"

namespace FieaGameEngine
{
	class PhysicsEntity : public Entity
	{
		RTTI_DECLARATIONS(PhysicsEntity, Entity);
	public:

		/// <summary>
		/// Default constructor, used to pass the PhysicsEntity typeId up to Entity
		/// </summary>
		PhysicsEntity();

		PhysicsEntity(RTTI::IdType type, ECollisionChannel collisionChannel = ECollisionChannel::MAP);

		~PhysicsEntity();

		/// <summary>
		/// Registers this with the physics system and calls Init on all children
		/// </summary>
		/// <param name="worldState"></param>
		virtual void Init(WorldState* worldState) override;

		/// <summary>
		/// Updates the position of the entity based on box2d calculations
		/// </summary>
		/// <param name="worldState"></param>
		virtual void Update(WorldState* worldState) override;

		/// <summary>
		/// Retrieves the Signatures for this class, which describe its prescribed 
		/// attributes
		/// </summary>
		/// <returns> A Vector of Signatures representing the prescribed attributes
		/// of this class </returns>
		static const Vector<Signature> Signatures();

		#pragma region Box2D Getters/Setters
		const b2Vec2& GetLinearVelocity() const { return m_body->GetLinearVelocity(); }
		void SetLinearVelocity(const b2Vec2& newVelocity) { m_body->SetLinearVelocity(newVelocity); }
		void SetLinearVelocity(b2Vec2&& newVelocity) { m_body->SetLinearVelocity(newVelocity); }

        const float GetAngularVelocity() const { return m_body->GetAngularVelocity(); }
        void SetAngularVelocity(float newVelocity) { m_body->SetAngularVelocity(newVelocity); }

		float GetGravityScale() const { return m_body->GetGravityScale(); }
		void SetGravityScale(float newScale) { m_body->SetGravityScale(newScale); }

		bool HasFixedRotation() const { return m_body->IsFixedRotation(); }
		void SetFixedRotation(bool shouldBeFixed) { m_body->SetFixedRotation(shouldBeFixed); }

		float GetLinearDamping() const { return m_body->GetLinearDamping(); }
		void SetLinearDamping(float linearDamping) { m_body->SetLinearDamping(linearDamping); }

		float GetAngularDamping() const { return m_body->GetAngularDamping(); }
		void SetAngularDamping(float angularDamping) { m_body->SetAngularDamping(angularDamping); }

		void ApplyForce(float xForce, float yForce);
		void ApplyLinearImpulse(float xImpulse, float yImpulse);

		void SetPosition(b2Vec2 newPosition) override;
		void SetAsTrigger(bool IsTrigger);
		void SetAllowRotation(bool ShouldRotate) { m_body->SetFixedRotation(!ShouldRotate); }

		void TurnOffCollisions();
		#pragma endregion

	protected:
		/// <summary>
		/// Creates the fixture (collisions) for this entity. By default creates
		/// a square collider around the size of the entity.
		/// </summary>
		virtual void CreateFixture();

		/// <summary>
		/// Collisions callback for when this entity starts or stops colliding with another entity.
		/// By default do nothing.
		/// </summary>
		friend WorldContactListener;
		virtual void OnCollisionStart(PhysicsEntity*) {};
		virtual void OnCollisionEnd(PhysicsEntity*) {};

        /// <summary>
        /// This entity's box2d body
        /// </summary>
        b2Body* m_body;

        /// <summary>
        /// The fixture definition to use to create this entity's fixture on init. This
        /// will likely be out of date a runtime after init so don't use it after that.
        /// </summary>
        b2FixtureDef m_startingFixture;

	private:
		/// <summary>
		/// Creates defaults for fixtures and bodies in case we don't initialize from JSON
		/// </summary>
		void InitStartingFixtures();

		void SetCollisionFilter();

		/// <summary>
		/// The starting body type of the entity. Might get out of date after init
		/// </summary>
		std::string m_bodyType = "dynamic";

		/// <summary>
		/// Whether or not this object is a "Trigger", meaning it doesn't have physical
		/// collisions but still gets OnCollision callbacks
		/// </summary>
		int m_isTrigger = 0;

		/// <summary>
		/// Indicates whether or not the body has been initialized so we can avoid trying
		/// to use the body before it is made
		/// </summary>
		bool m_bodyInitialized = false;

		/// <summary>
		/// The body definition to use to create this entity's b2body on init. This
		/// will likely be out of date at runtime after init so don't use it after that.
		/// </summary>
		b2BodyDef m_startingDef;

		ECollisionChannel m_collisionChannel = ECollisionChannel::MAP;

		/// <summary>
		/// Map used to map strings to body types
		/// </summary>
		inline static const std::map<std::string, b2BodyType> m_stringToBodyType =
		{
			{"static", b2BodyType::b2_staticBody},
			{"dynamic", b2BodyType::b2_dynamicBody},
			{"kinematic", b2BodyType::b2_kinematicBody}
		};
	};

	ConcreteFactory(PhysicsEntity, Scope);
}

