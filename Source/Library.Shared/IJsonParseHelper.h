#pragma once

#include "JsonParseMaster.h"

namespace Library
{
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI);

	public:

		/// <summary>
		/// Virtual destructor. Used to make sure polymorphic behavior is maintained when destroying child classes
		/// </summary>
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// Initializes any necessary data before the helper is used
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Virtual create method used to instantiate a new version of a child class of IJsonParseHelper
		/// </summary>
		/// <returns>The address of the created helper</returns>
		virtual IJsonParseHelper* Create() = 0;

		/// <summary>
		/// Checks to make sure that this helper is able to handle the parsed JSON data and convert it into its desired data
		/// </summary>
		/// <param name="sharedData">The reference of the SharedData used to store the information handled by the helper</param>
		/// <param name="jsonKey">The string key of the JSON member this is supposed to handle</param>
		/// <param name="object">The Json::Value object of the data meant to be managed by the handler</param>
		/// <param name="isArrayElement">Tells whether or not the Json::Value is a part of an array of data or not</param>
		/// <returns>True if this handler will be able to handle the passed in data, false otherwise</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey, Json::Value& object, bool isArrayElement = false, size_t index = 0) = 0;
		/// <summary>
		/// Actually handles the parsed JSON data and converts it into its desired form of data
		/// </summary>
		/// <param name="sharedData">The reference of the SharedData used to store the information handled by the helper</param>
		/// <param name="jsonKey">The string key of the JSON member this is supposed to handle, used to get the data saved by the StartHandler</param>
		/// <returns>True if this handler was able to handle the passed in data, false otherwise</returns>
		virtual bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey) = 0;

	};
}

