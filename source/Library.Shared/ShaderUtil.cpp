#include "pch.h"
#include "ShaderUtil.h"
#include <ios>
#include <iostream>
#include <fstream>


using namespace std;

namespace FieaGameEngine
{
    void ShaderUtil::LoadBinaryFile(const string& filename, vector<char>& data)
    {
        ifstream file(filename.c_str(), ios::binary);
        if (!file.good())
        {
            throw runtime_error("Could not open file.");
        }

        file.seekg(0, ios::end);
        size_t size = static_cast<size_t>(file.tellg());

        if (size > 0)
        {
            data.resize(size);
            file.seekg(0, ios::beg);
            file.read(&data.front(), size);
        }

        file.close();
    }
}