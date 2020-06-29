#pragma once

#include <json/json.h>
#include <gsl/gsl>
#include "Vector.h"
#include "RTTI.h"

namespace Library
{
	class IJsonParseHelper;

	class JsonParseMaster final
	{
	public:

		/// <summary>
		/// The SharedData used by the ParseMasters helpers to record the information handled.
		/// </summary>
		class SharedData : public RTTI
		{
			friend JsonParseMaster;

			RTTI_DECLARATIONS(SharedData, RTTI);

		public:
			/// <summary>
			/// Constructor that defaults all member variables
			/// </summary>
			SharedData() = default;
			/// <summary>
			/// Copy constructor. Copies the data from the rhs SharedData
			/// </summary>
			/// <param name="rhs">The SharedData whose data is being copied into this one</param>
			SharedData(const SharedData& rhs) = default;
			/// <summary>
			/// Move constructor. Moves the data from the rhs SharedData
			/// </summary>
			/// <param name="rhs">The SharedData whose data is being moved into this one</param>
			SharedData(SharedData&& rhs) noexcept;
			/// <summary>
			/// Copy assignment operator. Copies the data from the rhs SharedData
			/// </summary>
			/// <param name="rhs">The SharedData whose data is being copied into this one</param>
			/// <returns>A reference to this SharedData after its been mutated</returns>
			SharedData& operator=(const SharedData& rhs) = default;
			/// <summary>
			/// Move assignment operator. moves the data from the rhs SharedData
			/// </summary>
			/// <param name="rhs">The SharedData whose data is being moved into this one</param>
			/// <returns>A reference to this SharedData after its been mutated</returns>
			SharedData& operator=(SharedData&& rhs) noexcept;
			/// <summary>
			/// Destructor used to free up any allocated memory
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// Virtual create method used to create any child classes of shared data
			/// </summary>
			/// <returns></returns>
			virtual SharedData* Create() = 0;
			
			/// <summary>
			/// Returns a pointer to the JsonParseMaster who is using this SharedData
			/// </summary>
			/// <returns>The address of the JsonParseMaster who is using this SharedData</returns>
			JsonParseMaster* GetJsonParseMaster();
			/// <summary>
			/// Returns a const pointer to the JsonParseMaster who is using this SharedData
			/// </summary>
			/// <returns>The address of the JsonParseMaster who is using this SharedData. Can't be modified</returns>
			const JsonParseMaster* GetJsonParseMaster() const;

			/// <summary>
			/// Returns the depth of how deep this SharedData currently is within the Json data its currently be used to handle
			/// </summary>
			/// <returns></returns>
			size_t Depth() const;

		private:
			/// <summary>
			/// Increases the counter maintaining a counter of how many levels deep this SharedData currently is 
			/// within the Json data currently being handled
			/// </summary>
			void IncrementDepth();
			/// <summary>
			/// Decreases the counter maintaining a counter of how many levels deep this SharedData currently is 
			/// within the Json data currently being handled
			/// </summary>
			void DecrementDepth();

			/// <summary>
			/// Sets the pointer of this SharedData's JsonParseMaster to the master currently using this SharedData
			/// </summary>
			/// <param name="parseMaster"></param>
			void SetJsonParseMaster(JsonParseMaster* parseMaster);

			/// <summary>
			/// The address of the JsonParseMaster using this SharedData
			/// </summary>
			JsonParseMaster* mParseMaster = nullptr;
			/// <summary>
			///  The depth of how deep this SharedData currently is within the Json data its currently be used to handle
			/// </summary>
			size_t mDepth = 0;
		};

		JsonParseMaster() = delete;
		JsonParseMaster(const JsonParseMaster& rhs) = delete;
		JsonParseMaster& operator=(const JsonParseMaster& rhs) = delete;

		/// <summary>
		/// Constructor used to create a JsonParseMaster. Requires a SharedData object so that it can use it to handle JSON data
		/// </summary>
		/// <param name="sharedData">The SharedData used by this parser to handle JSON data</param>
		JsonParseMaster(SharedData& sharedData);
		/// <summary>
		/// Move constructor. Moves all data from the rhs JsonParseMaster into this parser.
		/// </summary>
		/// <param name="rhs">The JsonParseMaster whose data is being moved into this one</param>
		JsonParseMaster(JsonParseMaster&& rhs) noexcept;
		/// <summary>
		/// Move assignment operator. Moves all data from the rhs JsonParseMaster into this parser.
		/// </summary>
		/// <param name="rhs">The JsonParseMaster whose data is being moved into this one</param>
		/// <returns>A reference to this JsonParseMaster after its been mutated</returns>
		JsonParseMaster& operator=(JsonParseMaster&& rhs) noexcept;

		/// <summary>
		/// The destructor. Frees up any allocated memory for this parser.
		/// Note: Only owns its own data if it is a clone of another JsonParseMaster
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		/// Creates a separate clone of the JsonParseMaster. This clone has copied versions of this parser helpers and SharedData.
		/// Note: the clone can not have new helpers added or its SharedData changed since it owns that memory
		/// </summary>
		/// <returns>The address of the newly created clone</returns>
		gsl::owner<JsonParseMaster*> Clone() const;

		/// <summary>
		/// Adds an IJsonParseHelper to this parser's lists of helpers.
		/// </summary>
		/// <param name="helper">The IJsonParseHelper being added to the list of helpers</param>
		void AddHelper(IJsonParseHelper& helper);
		/// <summary>
		/// Removes an IJsonParseHelper to this parser's lists of helpers.
		/// </summary>
		/// <param name="helper">The IJsonParseHelper being relmoved to the list of helpers</param>
		void RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Parses the passed in raw JSON data into its correct c++ object if this JsonParseMaster can handle the passed in data
		/// </summary>
		/// <param name="rawJson">The JSON string to be converted into a runtime object</param>
		void Parse(const std::string& rawJson);
		/// <summary>
		/// Parses the passed in files JSON data into its correct c++ object if this JsonParseMaster can handle the passed in data
		/// </summary>
		/// <param name="rawJson">The file name of the file that contains the JSON data to be converted into a runtime object</param>
		void ParseFromFile(const std::string& filename);
		/// <summary>
		/// Parses the passed in stream of JSON data into its correct c++ object if this JsonParseMaster can handle the passed in data
		/// </summary>
		/// <param name="rawJson">The istream of JSON to be converted into a runtime object</param>
		void Parse(std::istream& stream);

		/// <summary>
		/// Returns the FileName of the file read by ParseFile. Will return an empty string if ParseFile was not called.
		/// </summary>
		/// <returns>std::string reference of the file name used by ParseFile</returns>
		const std::string& GetFileName() const;

		/// <summary>
		/// Returns a const version of the SharedData used by this parser.
		/// </summary>
		/// <returns>The address of the SharedData currently contained within this parser</returns>
		const SharedData* GetSharedData() const;
		/// <summary>
		/// Returns the SharedData used by this parser.
		/// </summary>
		/// <returns>The address of the SharedData currently contained within this parser</returns>
		SharedData* GetSharedData();
		/// <summary>
		/// Sets the SharedData that will be used to contain information read from the JSON parsed by this parser
		/// </summary>
		/// <param name="data">The address of the SharedData to be used by this parser</param>
		void SetSharedData(SharedData* data);

		/// <summary>
		/// Returns a constant Vector of the Helpers contained within the parse master
		/// </summary>
		/// <returns>A const reference to the Helpers inside the parse master</returns>
		const Vector<IJsonParseHelper*>& Helpers() const;

	private:
		/// <summary>
		/// Given a Json::Value it will individual parse each member contained within the object and create the corresponding
		/// c++ object within the SharedData object if it was able to handle the passed in data.
		/// </summary>
		/// <param name="jsonValue">The parsed Json data used to be converted into the correct c++ object information</param>
		void ParseMembers(Json::Value& jsonValue, bool isArrayElement = false, size_t index = 0);
		/// <summary>
		/// Parses an individual member of a Json::Value object. Uses the helper parses to convert the Json data into its corresponding data.
		/// </summary>
		/// <param name="key">The key name of the Json::Value member</param>
		/// <param name="value">The value of the currently parsed Json::Value member</param>
		/// <param name="isArrayElement">A boolean that tells the parser if it is currently parsing an element of an array</param>
		void ParseKeyValuePair(const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0);

		/// <summary>
		/// Vector of IJsonParseHelpers used to handle parsed JSON data
		/// </summary>
		Vector<IJsonParseHelper*> mHelpers;
		/// <summary>
		/// The SharedData used to store the information gathered from parsing the passed in JSON data
		/// </summary>
		SharedData* mSharedData;
		/// <summary>
		/// The name of the file used if this parser was used to parse a file of JSON data
		/// </summary>
		std::string mFile;
		/// <summary>
		/// A flag to represent if this parser is a clone and owns its own memory. If true the destructor must handle freeing up the allocated memory.
		/// </summary>
		bool mIsClone = false;
	};
}

