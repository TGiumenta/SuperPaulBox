#pragma once
#include "Event.h"
#include "Vector.h"
#include "HashMap.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	enum KeyEventTypes
	{
		KEY_PRESS = GLFW_PRESS,
		KEY_RELEASE = GLFW_RELEASE,
		KEY_DONTUSE = GLFW_REPEAT,
		KEY_REPEAT
	};

	struct KeyboardEvent : public RTTI
	{
		RTTI_DECLARATIONS(KeyboardEvent, RTTI);

	public:


		int key;
		int action;

		KeyboardEvent(int inKey, int inAction)
			:key(inKey), action(inAction)
		{}

		bool operator==(const KeyboardEvent& other) const
		{
			return (key == other.key) && (action == other.action);
		}
	};

	class InputManager
	{
	public:
		static void Update();

		static void Inititalize(GLFWwindow* window);

		static void KeyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	private:
		inline static Vector<int> m_heldKeys;

	};

}
