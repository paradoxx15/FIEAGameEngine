#pragma once

#include "IJsonParseHelper.h"
#include "Scope.h"
#include "Datum.h"
#include "Stack.h"
#include "HashMap.h"

namespace Library
{
	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper);

	public:

		class TableSharedData : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(TableSharedData, JsonParseMaster::SharedData);

		public:

			/// <summary>
			/// Default constructor. Creates a TableSharedData and sets the internal Scope pointer if an address is passed in.
			/// </summary>
			/// <param name="scope">The address of the scope that this shared data should be referencing</param>
			TableSharedData(Scope* scope = nullptr);
			/// <summary>
			/// Default copy constructor. Will copy the scope pointer of the the rhs TableSharedData into this one.
			/// </summary>
			/// <param name="rhs">The TableSharedData being copied into this one</param>
			TableSharedData(const TableSharedData& rhs) = default;
			/// <summary>
			/// Default move constructor. Will move the scope pointer of the the rhs TableSharedData into this one.
			/// </summary>
			/// <param name="rhs">The TableSharedData being moved into this one</param>
			TableSharedData(TableSharedData&& rhs) noexcept = default;
			/// <summary>
			/// Default copy assignment operator. Will copy the scope pointer of the the rhs TableSharedData into this one.
			/// </summary>
			/// <param name="rhs">The TableSharedData being copied into this one</param>
			/// <returns>A reference to this TableSharedData after being mutated</returns>
			TableSharedData& operator=(const TableSharedData& rhs) = default;
			/// <summary>
			/// Default move assignment operator. Will move the scope pointer of the the rhs TableSharedData into this one.
			/// </summary>
			/// <param name="rhs">The TableSharedData being moved into this one</param>
			/// <returns>A reference to this TableSharedData after being mutated</returns>
			TableSharedData& operator=(TableSharedData&& rhs) noexcept = default;
			/// <summary>
			/// Default destructor. Will destroy any allocated memory.
			/// </summary>
			~TableSharedData() = default;

			/// <summary>
			/// Virtual constructor that will create and return the address of a new TableSharedData.
			/// Note: When calling this function the user now owns the memory of the created TableSharedData.
			/// </summary>
			/// <returns>A pointer to a newly created TableSharedData on the heap</returns>
			TableSharedData* Create() override;

			/// <summary>
			/// Returns the pointer of the scope being contained within this TableSharedData
			/// </summary>
			/// <returns>The address of the Scope this TableSharedData references</returns>
			Scope* GetScope();
			/// <summary>
			/// Returns a const pointer of the scope being contained within this TableSharedData
			/// </summary>
			/// <returns>A const pointer to the Scope this TableSharedData references</returns>
			const Scope* GetScope() const;
			/// <summary>
			/// Sets the Scope pointer inside this TableSharedData to the address of the passed in scope
			/// </summary>
			/// <param name="scope">The Scope which this TableSharedData should reference</param>
			void SetScope(Scope& scope);

			/// <summary>
			/// RTTI ToString() override;
			/// </summary>
			/// <returns>A std::string repsresentation of this TableSharedData</returns>
			std::string ToString() const override;
			/// <summary>
			/// RTTI Equals override. Checks two TableSharedDatas for equality by checking if they are pointing to the same Scope.
			/// </summary>
			/// <param name="rhs">The pointer of the TableSharedData this one should be compared to.</param>
			/// <returns>True if this is equal to rhs, false otherwise</returns>
			bool Equals(const RTTI* rhs) const override;

		private:

			/// <summary>
			/// Pointer to the Scope that this TableSharedData is used for to populate
			/// </summary>
			Scope* mScope = nullptr;
		};

		/// <summary>
		/// Static HashMap that pairs strings to DatumTypes. Used to convert a JSON strings that represent DatumTypes into actual DatumTypes.
		/// </summary>
		const static HashMap<std::string, Datum::DatumTypes> ValueTypes;

		/// <summary>
		/// Defaulted constructor that default constructs the contained stack.
		/// </summary>
		JsonTableParseHelper() = default;
		/// <summary>
		/// Default copy constructor. Will copy the stack of the the rhs ParseHelper into this one.
		/// </summary>
		/// <param name="rhs">The JsonTableParseHelper being copied into this one</param>
		JsonTableParseHelper(const JsonTableParseHelper& rhs) = default;
		/// <summary>
		/// Default move constructor. Will move the stack of the rhs ParseHelper into this one.
		/// </summary>
		/// <param name="rhs">The JsonTableParseHelper being moved into this one</param>
		JsonTableParseHelper(JsonTableParseHelper&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator. Will copy the stack of the the rhs ParseHelper into this one.
		/// </summary>
		/// <param name="rhs">The JsonTableParseHelper being copied into this one</param>
		/// <returns>A reference to this ParseHelper after being mutated</returns>
		JsonTableParseHelper& operator=(const JsonTableParseHelper& rhs) = default;
		/// <summary>
		/// Default move assignment operator. Will move the stack of the the rhs ParseHelper into this one.
		/// </summary>
		/// <param name="rhs">The JsonTableParseHelper being moved into this one</param>
		/// <returns>A reference to this ParseHelper after being mutated</returns>
		JsonTableParseHelper& operator=(JsonTableParseHelper&& rhs) = default;
		/// <summary>
		/// Default destructor. Will destruct the contained stack.
		/// </summary>
		~JsonTableParseHelper() = default;

		/// <summary>
		/// Virtual constructor that will create and return the address of a new JsonTableParseHelper.
		/// Note: When calling this function the user now owns the memory of the created JsonTableParseHelper.
		/// </summary>
		/// <returns>A pointer to a newly created JsonTableParseHelper on the heap</returns>
		IJsonParseHelper* Create() override;

		/// <summary>
		/// Checks whether or not this helper can handle the passed in JSON data. 
		/// If it can handle the passed in data it will populate the Scope contained within the passed in SharedData with the correct information based on what is passed to the handler.
		/// Uses a stack to hold data if the JSON has nested objects.
		/// </summary>
		/// <param name="sharedData">Must be a TableSharedData with a valid pointer to a Scope. Used to populate said Scope with its correct information</param>
		/// <param name="jsonKey">The JSON key of the current object being analyzed</param>
		/// <param name="object">The Json::Value data being analyzed to discover what needs to be put inside the Scope contained within SharedData</param>
		/// <param name="isArrayElement">Tells whether or not the passed in data is an element of an array or a standalone value</param>
		/// <param name="index">The index of what element inside the array the object is supposed to be. If it is not an array element index will be zero.</param>
		/// <returns>True if this Helper was able to handle the passed in data. False otherwise</returns>
		bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey, Json::Value& object, bool isArrayElement = false, size_t index = 0) override;

		/// <summary>
		/// After the StartHandler is called this method does cleanup. First it makes sure that the sharedData somehow didn't get messed up. 
		/// Second if this handler is moving out of a nested object within the JSON it will pop off the stack the current stack frame to move back up the JSON.
		/// </summary>
		/// <param name="sharedData">The TableSharedData that contains the Scope being created</param>
		/// <param name="jsonKey">The key of the current frame. Used to determine whether or not the stack needs to be popped.</param>
		/// <returns>True if the EndHandler was succsseful, false if something went wrong</returns>
		bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey) override;

		/// <summary>
		/// RTTI ToString() override. Returns the string "JsonTableParseHelper"
		/// </summary>
		/// <returns>A std::string of the name of this class</returns>
		std::string ToString() const override;
		/// <summary>
		/// RTTI Equals() override. Checks if two JsonTableParseHelpers are equal if they have the same stack.
		/// </summary>
		/// <param name="rhs">The JsonTableParseHelper being compared to this one.</param>
		/// <returns>True if they are equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	private:
		/// <summary>
		/// Struct used to contain the necessary data on the stack
		/// </summary>
		struct StackFrame
		{
			/// <summary>
			/// The key being analyzed currently in this frame
			/// </summary>
			const std::string& key;
			/// <summary>
			/// The scope being modified by the current frame. Used to help maintain what scope is being modified when going into nested objects
			/// </summary>
			Scope& context;
		};

		/// <summary>
		/// A stack that holds StackFrame information as the JSON descends into deeper levels.
		/// </summary>
		Stack<StackFrame> mStack;
	};
}

