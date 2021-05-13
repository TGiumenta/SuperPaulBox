#pragma once
#include "IJsonParseHelper.h"
#include "Scope.h"
#include "Datum.h"
#include "Vector.h"
#include "Stack.h"

namespace FieaGameEngine
{
	/// <summary>
	/// JSON parse helper that can handle parsing our particular grammar
	/// and populating a Scope within SharedData. 
	/// </summary>
	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper);

	public:
		/// <summary>
		/// Embedded SharedData class that keeps track of the root scope that 
		/// we are populating during the parsing process
		/// </summary>
		class SharedData final : public JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseCoordinator::SharedData);

		public:
			/// <summary>
			/// Constructor that takes in the scope to populate
			/// </summary>
			/// <param name="rootScope"> A reference to the Scope to populate </param>
			SharedData(Scope& rootScope);

			/// <summary>
			/// Overridden "virtual constructor" that creates a SharedData of this type and
			/// returns a pointer to it
			/// </summary>
			/// <returns> A pointer to the heap allocated SharedData </returns>
			virtual gsl::owner<SharedData*> Create() const override;

			/// <summary>
			/// The Scope we are populating with the data that we parse using
			/// the handler
			/// </summary>
			Scope* rootScope;
		};

		/// <summary>
		/// Override "virtual constructor" that creates a JsonTableParseHelper
		/// </summary>
		/// <returns> A pointer to the heap allocated JsonTableParseHelper </returns>
        virtual gsl::owner<IJsonParseHelper*> Create() const override;

		/// <summary>
		/// Attempts to parse the incoming key, value pair using this handler. If the passed
		/// in information doesn't fit this handler's grammar, the call will fail and return
		/// false.
		/// </summary>
		/// <param name="sharedData"> The SharedData to use for storing parse information </param>
		/// <param name="key"> The incoming key </param>
		/// <param name="jsonValue"> The incoming value </param>
		/// <param name="isArray"> Whether or not this value is an array </param>
		/// <returns> True if the handler successfully parsed the incoming data,
		/// false otherwise </returns>
        virtual bool StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& jsonValue, bool isArray) override;
        
		/// <summary>
        /// Finishes the handler call. This is a paired call that should only be made after a StartHandler call
        /// has returned true. In this helper, this handles popping stack frames off the stack
		/// </summary>
		/// <param name="sharedData"> The SharedData to use for storing parse information </param>
		/// <param name="key"> The incoming key </param>
        /// <returns> True if the handler successfully parsed the incoming data,
        /// false otherwise </returns>
		virtual bool EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key) override;

	private:
		/// <summary>
		/// Private StackFrame struct used to bundle data on the stack
		/// </summary>
		struct StackFrame
		{
			/// <summary>
			/// The key associated with this stack frame
			/// </summary>
			const std::string& m_key;

			/// <summary>
			/// The Datum type associated with this stack frame
			/// </summary>
			Datum::DatumType m_type = Datum::DatumType::Unknown;

			std::string m_className;

			/// <summary>
			/// The context associated with this stack frame
			/// </summary>
			Scope* m_context = nullptr;

			size_t m_arrayIndex = 0;

			/// <summary>
			/// StackFrame constructor
			/// </summary>
			/// <param name="key"> The key to associated with this StackFrame </param>
			/// <param name="context"> The context to associate with this StackFrame </param>
            StackFrame(const std::string& key, Scope* context)
                : m_key(key), m_context(context) {}
		};

		/// <summary>
		/// Our current stack of StackFrames
		/// </summary>
		Stack<StackFrame> m_stack;

		#pragma region JsonIntoDatumFunctions
        /// <summary>
        /// FromString functions used to create elements from strings. 
        /// </summary>
        using JsonToDatumFunction = void(JsonTableParseHelper::*)(Datum& datum, const Json::Value& jsonValue, size_t index);
        void JsonIntToDatum(Datum& datum, const Json::Value& jsonValue, size_t index);
		void JsonFloatToDatum(Datum& datum, const Json::Value& jsonValue, size_t index);
		void JsonStringToDatum(Datum& datum, const Json::Value& jsonValue, size_t index);
		void JsonGenericToDatum(Datum& datum, const Json::Value& jsonValue, size_t index);

        inline static const JsonToDatumFunction m_JSONToDatumFunctions[] =
        {
            &JsonTableParseHelper::JsonIntToDatum,
            &JsonTableParseHelper::JsonStringToDatum,
            &JsonTableParseHelper::JsonFloatToDatum,
            &JsonTableParseHelper::JsonGenericToDatum,
            &JsonTableParseHelper::JsonGenericToDatum,
            nullptr,
            nullptr
        };
		#pragma endregion
	};
}

