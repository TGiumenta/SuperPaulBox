#pragma once
#include <box2d/box2d.h>

namespace FieaGameEngine
{
	class WorldContactListener : public b2ContactListener
	{
		void BeginContact(b2Contact* contact) override;

		void EndContact(b2Contact* contact) override;
	};
}

