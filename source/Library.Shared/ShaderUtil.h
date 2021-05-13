#pragma once

#include <string>
#include <vector>

namespace FieaGameEngine
{
    class ShaderUtil
    {
    public:
        ShaderUtil() = delete;
        ShaderUtil(const ShaderUtil&) = delete;
        ShaderUtil& operator=(const ShaderUtil&) = delete;
        ShaderUtil(ShaderUtil&&) = delete;
        ShaderUtil& operator=(ShaderUtil&&) = delete;
        ~ShaderUtil() = default;

        static void LoadBinaryFile(const std::string& filename, std::vector<char>& data);
    };
}
