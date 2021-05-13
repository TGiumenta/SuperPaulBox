#pragma once

#include "RTTI.h"
#include "SList.h"
#include <json/json.h>
#include <gsl/gsl>

namespace FieaGameEngine
{
	class IJsonParseHelper;

	/// <summary>
	/// JsonParseCoordinator Class that handles taking in JSON data and distributing
	/// the parsing to its contained helpers
	/// </summary>
	class JsonParseCoordinator : public RTTI
	{
		RTTI_DECLARATIONS(JsonParseCoordinator, RTTI);
	public:

		/// <summary>
		/// Shared data embedded class that handles tracking depth and storing data
		/// between helpers
		/// </summary>
		class SharedData : public RTTI
		{
			friend JsonParseCoordinator;
			RTTI_DECLARATIONS(SharedData, RTTI);

		public:

			/// <summary>
			/// Default constructor
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Default copy constructor
			/// </summary>
			/// <param name=""> The other SharedData to copy from </param>
			SharedData(const SharedData&) = default;

			/// <summary>
			/// Default move constructor
			/// </summary>
			/// <param name=""> The other SharedData to move from </param>
			SharedData(SharedData&&) = default;

			/// <summary>
			/// Default copy assignment
			/// </summary>
			/// <param name=""> The other SharedData to copy from </param>
			/// <returns> A reference to this SharedData </returns>
			SharedData& operator=(const SharedData&) = default;

			/// <summary>
			/// Default move assignment
			/// </summary>
			/// <param name=""> The other SharedData to move from </param>
			/// <returns> A reference to this SharedData </returns>
			SharedData& operator=(SharedData&&) = default;

			/// <summary>
			/// Pure virtual destructor
			/// </summary>
			virtual ~SharedData() = 0 {};

			/// <summary>
			/// A "virtual constructor" that we can use to dynamically create
			/// a SharedData of the correct derived type
			/// </summary>
			/// <returns> A pointer to the newly heap-allocated SharedData </returns>
			virtual gsl::owner<SharedData*> Create() const = 0;

			/// <summary>
			/// Gets the current JsonParseCoordinator associated with this instance
			/// of SharedData.
			/// </summary>
			/// <returns> A pointer to the current JsonParseCoordinator </returns>
			JsonParseCoordinator* GetJsonParseCoordinator();

			/// <summary>
			/// Increments our current depth by 1, indicating we have recursed to a
			/// deeper level
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// Decrements our current depth by 1, indicating we have exited from a 
			/// recursive call
			/// </summary>
			void DecrementDepth();

			/// <summary>
			/// Gets our current recursive depth of our parse call
			/// </summary>
			/// <returns> The current depth </returns>
			size_t Depth();

		private:

			/// <summary>
			/// Sets our current JsonParseCoordinator to be the new one associated
			/// with this SharedData instance
			/// </summary>
			/// <param name="parseCoordinator"> The new JsonParseCoordinator </param>
			void SetJsonParseCoordinator(JsonParseCoordinator* parseCoordinator);
			
			/// <summary>
			/// The JsonParseCoordinator associated with this SharedData instance
			/// </summary>
			JsonParseCoordinator* m_parseCoordinator = nullptr;

			/// <summary>
			/// Our current recursive depth
			/// </summary>
			size_t m_depth = 0;
		};

		/// <summary>
		/// JsonParseCoordinator constructor that creates a coordinator and associates
		/// it with the passed in SharedData
		/// </summary>
		/// <param name="sharedData"> The SharedData instance to associate with this
		/// coordinator </param>
		explicit JsonParseCoordinator(SharedData& sharedData);

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		JsonParseCoordinator(const JsonParseCoordinator&) = delete;

		/// <summary>
		/// Move constructor that steals the data from the passed in JsonParseCoordinator
		/// </summary>
		/// <param name="other"> The other JsonParseCoordinator to move from </param>
		JsonParseCoordinator(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Deleted copy assignment
		/// </summary>
		JsonParseCoordinator& operator=(const JsonParseCoordinator&) = delete;

		/// <summary>
		/// Move assignment operator that clears this JsonParseCoordinator and then
		/// steals the data from the passed in JsonParseCoordinator
		/// </summary>
		/// <param name="other"> The other JsonParseCoordinator to move from </param>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Destructor. If the coordinator was cloned, this deletes all of its helpers and
		/// its associated SharedData
		/// </summary>
        ~JsonParseCoordinator();

		/// <summary>
		/// Creates a clone of this SharedData. This clone is heap allocated and has a fresh
		/// copy of all of the helpers and the associated SharedData.
		/// </summary>
		/// <returns> A pointer to the newly allocated clone </returns>
		JsonParseCoordinator* Clone() const;

		/// <summary>
		/// Takes the passed in string and then distributes parsing to its helpers to populate
		/// the associated SharedData.
		/// </summary>
		/// <param name="input"> The JSON string to parse </param>
		void Parse(const std::string& input);

        /// <summary>
        /// Takes the passed in istream and then distributes parsing to its helpers to populate
        /// the associated SharedData.
        /// </summary>
        /// <param name="input"> The JSON stream to parse </param>
		void Parse(std::istream& jsonData);

        /// <summary>
        /// Opens the passed in file name, extracts the JSON data and then distributes parsing 
        /// to its helpers to populate the associated SharedData.
        /// </summary>
        /// <param name="input"> The JSON file to parse </param>
		void ParseFromFile(const std::string& filename);

		/// <summary>
		/// Adds a helper to this coordinator's list of helpers
		/// </summary>
		/// <param name="helper"> The helper to add to the list </param>
		void AddHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Removes a helper from the coordinator's list of helpers. If the helper isn't found
		/// then nothing happens.
		/// </summary>
		/// <param name="helper"> The helper to attempt to remove </param>
		void RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Returns the SharedData associated with this coordinator
		/// </summary>
		/// <returns> A pointer to the associated SharedData </returns>
		SharedData* GetSharedData() const;

		/// <summary>
		/// Sets the SharedData associated with this coordinator 
		/// </summary>
		/// <param name="sharedData"> A pointer to the SharedData to associate with this
		/// coordinator </param>
		void SetSharedData(SharedData* sharedData);

		/// <summary>
		/// Returns the filename that this coordinator last parsed. This only works
		/// if parse was called through the overload that takes a filename
		/// </summary>
		/// <returns> The filename that was last parsed </returns>
		std::string GetFileName() const;

		/// <summary>
		/// Initializes all helpers and the associated SharedData
		/// </summary>
		void Initialize();

		/// <summary>
		/// Searches the Json::Value for nested files and replaces them with the contents of the actual
		/// file
		/// </summary>
		/// <param name="jsonVal"> The Json::Value to replace nested files inside </param>
		void ParseNestedFiles(Json::Value& jsonVal);

	private:
		/// <summary>
		/// List of IJsonParseHelpers used for parsing
		/// </summary>
		SList<IJsonParseHelper*> m_helpers;

		/// <summary>
		/// The SharedData instance associated with this coordinator
		/// </summary>
		SharedData* m_sharedData;

		/// <summary>
		/// The name of the file that was last parsed
		/// </summary>
		std::string m_filename;

		/// <summary>
		/// Whether or not this coordinator is a clone
		/// </summary>
		bool m_isClone = false;

		/// <summary>
		/// Gets all the members of a json value and distributes the parsing to the helpers
		/// in this coordinator's helper list
		/// </summary>
		/// <param name="jsonVal"> The json value to parse </param>
		void ParseValue(const Json::Value& jsonVal, bool isArray);

		/// <summary>
		/// Parses a particular key-value pair based on the type of the value
		/// </summary>
		/// <param name="key"> The key of the key-value pair </param>
		/// <param name="jsonVal"> The value of the key-value pair </param>
		void ParseKeyValuePair(const std::string& key, const Json::Value& jsonVal, bool isArray);

		/// <summary>
		/// Helper function that opens nested files and inserts them into the current context
		/// </summary>
		/// <param name="context"> The context to insert the nested file in </param>
		void InsertNestedFile(Json::Value& context);
		
	};
}
