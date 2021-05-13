#include "pch.h"
#include "Input.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(KeyboardEvent);

    void InputManager::Update()
    {
        for (int heldKey : m_heldKeys)
        {
            KeyboardEvent keyboardEvent{ heldKey, KEY_REPEAT };
            Event<KeyboardEvent> event(keyboardEvent);
            event.Deliver();
        }
    }

    void InputManager::Inititalize(GLFWwindow* window)
    {
        glfwSetKeyCallback(window, &InputManager::KeyboardEventCallback);
        glfwSetMouseButtonCallback(window, &InputManager::MouseButtonCallback);
    }

    void InputManager::KeyboardEventCallback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
    {

        if (action == GLFW_PRESS)
        {
            m_heldKeys.PushBack(key);
        }
        else if (action == GLFW_RELEASE)
        {
            m_heldKeys.Remove(key);
        }

        // create event based on key (and action?)
        KeyboardEvent keyboardEvent{ key, action };
        Event<KeyboardEvent> event(keyboardEvent);
        event.Deliver();
    }

    void InputManager::MouseButtonCallback(GLFWwindow*, int button, int action, int)
    {
        if (action == GLFW_PRESS)
        {
            m_heldKeys.PushBack(button);
        }
        else if (action == GLFW_RELEASE)
        {
            m_heldKeys.Remove(button);
        }

        // create event based on key (and action?)
        KeyboardEvent keyboardEvent{ button, action };
        Event<KeyboardEvent> event(keyboardEvent);
        event.Deliver();
    }



}