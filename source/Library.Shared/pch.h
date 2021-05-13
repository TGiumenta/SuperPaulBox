#pragma once

// Standard Libraries
#include <exception>
#include <stdexcept>

// C++ Guideline Support Library
#include <gsl/gsl>

// Windows
#if defined(_WIN32)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <windows.h>
#endif

// Standard
#include <cstdlib>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// OpenGL
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

