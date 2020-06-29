#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"

using namespace std::string_literals;

namespace Library
{
	RTTI_DEFINITIONS(JsonTableParseHelper);
	RTTI_DEFINITIONS(JsonTableParseHelper::TableSharedData);

	const HashMap<std::string, Datum::DatumTypes> JsonTableParseHelper::ValueTypes = {
		std::pair("integer", Datum::DatumTypes::Integer),
		std::pair("float", Datum::DatumTypes::Float),
		std::pair("vector", Datum::DatumTypes::Vector),
		std::pair("matrix", Datum::DatumTypes::Matrix),
		std::pair("string", Datum::DatumTypes::String),
		std::pair("table", Datum::DatumTypes::Table)
	};

#pragma region TableSharedData

	JsonTableParseHelper::TableSharedData::TableSharedData(Scope* scope) : mScope(scope) {}

	JsonTableParseHelper::TableSharedData* JsonTableParseHelper::TableSharedData::Create()
	{
		return new TableSharedData;
	}

	Scope* JsonTableParseHelper::TableSharedData::GetScope()
	{
		return mScope;
	}

	const Scope* JsonTableParseHelper::TableSharedData::GetScope() const
	{
		return mScope;
	}

	void JsonTableParseHelper::TableSharedData::SetScope(Scope& scope)
	{
		mScope = &scope;
	}

	std::string JsonTableParseHelper::TableSharedData::ToString() const
	{
		return "TableSharedData";
	}

	bool JsonTableParseHelper::TableSharedData::Equals(const RTTI* rhs) const
	{
		const TableSharedData* otherData = rhs->As<TableSharedData>();
		assert(otherData != nullptr);
		return mScope == otherData->mScope;
	}

#pragma endregion TableSharedData

	IJsonParseHelper* JsonTableParseHelper::Create()
	{
		return new JsonTableParseHelper;
	}

	bool JsonTableParseHelper::StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey, Json::Value& object, bool, size_t index)
	{
		TableSharedData* tableSharedData = sharedData.As<TableSharedData>();
		if (tableSharedData == nullptr)
		{
			return false;
		}

		std::string lowercaseKey = jsonKey;
		for (char& c : lowercaseKey)
		{
			c = static_cast<char>(std::tolower(c));
		}

		if (lowercaseKey == "class")
		{
			StackFrame& stackFrame = mStack.Peek();
			Scope* createdScope = Factory<Scope>::Create(object.asString());
			assert(createdScope != nullptr);
			stackFrame.context.Adopt(*createdScope, stackFrame.key);
		}
		else if (ValueTypes.ContainsKey(lowercaseKey))
		{
			StackFrame& stackFrame = mStack.Peek();
			Datum* datum = stackFrame.context.Find(stackFrame.key);
			assert(datum != nullptr);

			datum->SetType(ValueTypes.At(lowercaseKey));
			
			if (datum->Type() == Datum::DatumTypes::Table)
			{
				if (index < datum->Size())
				{
					mStack.Push({ jsonKey, *datum->GetScope(index) });
				}
				else
				{
					mStack.Push({ jsonKey, stackFrame.context.AppendScope(stackFrame.key) });
				}
			}
			else
			{
				if (!datum->IsExternalStorage() && index >= datum->Size())
				{
					datum->Resize(index + 1);
				}

				switch (datum->Type())
				{
				case Datum::DatumTypes::Integer:
					datum->Set(object.asInt(), index);
					break;
				case Datum::DatumTypes::Float:
					datum->Set(object.asFloat(), index);
					break;
				case Datum::DatumTypes::Vector:
				case Datum::DatumTypes::Matrix:
					datum->SetFromString(object.asString(), index);
					break;
				case Datum::DatumTypes::String:
					datum->Set(object.asString(), index);
					break;
				}
			}
		}
		else if (object.isObject())
		{
			Scope& context = (mStack.IsEmpty()) ? *tableSharedData->GetScope() : mStack.Peek().context;
			context.Append(jsonKey);
			mStack.Push({ jsonKey, context });
		}

		return true;
	}

	bool JsonTableParseHelper::EndHandler([[maybe_unused]] JsonParseMaster::SharedData& sharedData, const std::string& jsonKey)
	{
		assert(sharedData.As<TableSharedData>() != nullptr);

		if (!mStack.IsEmpty() && jsonKey == mStack.Peek().key)
		{
			mStack.Pop();
		}

		return true;
	}

	std::string JsonTableParseHelper::ToString() const
	{
		return "JsonTableParseHelper";
	}

	bool JsonTableParseHelper::Equals(const RTTI* rhs) const
	{
		return &mStack == &rhs->As<JsonTableParseHelper>()->mStack;
	}
}