#pragma once
#include <gsl/gsl>
#include "JsonParseCoordinator.h"

namespace FieaGameEngine
{
    /// <summary>
    /// IJsonParseHelper interface that defines the base of all JsonParseHelpers used by the JsonParseCoordinator
    /// </summary>
    class IJsonParseHelper : public RTTI
    {
    public:

        /// <summary>
        /// Initialize method used to set the starting state of the helper
        /// </summary>
        virtual void Initialize();

        /// <summary>
        /// "Virtual constructor" used to dynamically create the correct derived type of the IJsonParseHelper
        /// </summary>
        /// <returns></returns>
        virtual gsl::owner<IJsonParseHelper*> Create() const = 0;

        /// <summary>
        /// Start handler call that decides if this handler can read the incoming data and then parses it if it's 
        /// valid data
        /// </summary>
        /// <param name="sharedData"> The SharedData instance to store the parsed info in </param>
        /// <param name="key"> The key of the incoming data </param>
        /// <param name="jsonValue"> The value of the incoming data </param>
        /// <param name="isArray"> Whether or not the incoming data is an array </param>
        /// <returns> True if the handler successfully read the data, false otherwise </returns>
        virtual bool StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& jsonValue, bool isArray) = 0;

        /// <summary>
        /// Finishes the handler call. This is a paired call that should only be made after a StartHandler call
        /// has returned true
        /// </summary>
        /// <param name="sharedData"> The SharedData instance to store the parsed info in </param>
        /// <param name="key"> The key of the incoming data </param>
        /// <returns> True if the handler successfully read the data, false otherwise 7</returns>
        virtual bool EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key) = 0;

    };
}