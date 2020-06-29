#include "pch.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"

namespace Library
{
#pragma region SharedData

	RTTI_DEFINITIONS(JsonParseMaster::SharedData);

	JsonParseMaster::SharedData::SharedData(SharedData&& rhs) noexcept :
		mParseMaster(rhs.mParseMaster), mDepth(rhs.mDepth)
	{
		rhs.mParseMaster = nullptr;
		rhs.mDepth = 0;
		mParseMaster->SetSharedData(this);
	}

	JsonParseMaster::SharedData& JsonParseMaster::SharedData::operator=(SharedData&& rhs) noexcept
	{
		if (&rhs != this)
		{
			mParseMaster = rhs.mParseMaster;
			mDepth = rhs.mDepth;
			rhs.mParseMaster = nullptr;
			rhs.mDepth = 0;
			mParseMaster->SetSharedData(this);
		}

		return *this;
	}

	JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster()
	{
		return mParseMaster;
	}

	const JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster() const
	{
		return mParseMaster;
	}

	void JsonParseMaster::SharedData::IncrementDepth()
	{
		mDepth++;
	}

	void JsonParseMaster::SharedData::DecrementDepth()
	{
		assert(mDepth != 0);

		mDepth--;
	}

	size_t JsonParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

	void JsonParseMaster::SharedData::SetJsonParseMaster(JsonParseMaster* parseMaster)
	{
		mParseMaster = parseMaster;
	}

#pragma endregion SharedData

#pragma region JsonParseMaster

	JsonParseMaster::JsonParseMaster(SharedData& sharedData) :
		mSharedData(&sharedData), mIsClone(false)
	{
		mSharedData->SetJsonParseMaster(this);
	}

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& rhs) noexcept :
		mHelpers(std::move(rhs.mHelpers)), mSharedData(rhs.mSharedData), mFile(std::move(rhs.mFile)), mIsClone(false)
	{
		rhs.mSharedData = nullptr;
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& rhs) noexcept
	{
		mHelpers = std::move(rhs.mHelpers);
		mSharedData = rhs.mSharedData;
		mFile = std::move(rhs.mFile);

		return *this;
	}

	JsonParseMaster::~JsonParseMaster()
	{
		if (mIsClone)
		{
			delete mSharedData;

			for (IJsonParseHelper* helper : mHelpers)
			{
				delete helper;
			}
		}
	}

	gsl::owner<JsonParseMaster*> JsonParseMaster::Clone() const
	{
		JsonParseMaster* retMaster = new JsonParseMaster(*mSharedData->Create());

		retMaster->mHelpers.Reserve(mHelpers.Size());
		for (IJsonParseHelper* helper : mHelpers)
		{
			retMaster->AddHelper(*helper->Create());
		}
		retMaster->mIsClone = true;
		return retMaster;
	}

	void JsonParseMaster::AddHelper(IJsonParseHelper& helper)
	{
		if (mIsClone)
		{
			throw std::runtime_error("Can't add helpers to a cloned parse master");
		}

		if (std::find_if(mHelpers.begin(), mHelpers.end(), [&helper](IJsonParseHelper* mHelper) { return mHelper->TypeIdInstance() == helper.TypeIdInstance(); }) == mHelpers.end())
		{
			mHelpers.PushBack(&helper);
		}
		else
		{
			throw std::runtime_error("Can't add duplicate helpers");
		}
	}

	void JsonParseMaster::RemoveHelper(IJsonParseHelper& helper)
	{
		mHelpers.Remove(&helper);

		if (mIsClone)
		{
			delete &helper;
		}
	}

	void JsonParseMaster::Parse(const std::string& rawJson)
	{
		std::istringstream stream(rawJson);
		Parse(stream);
	}

	void JsonParseMaster::ParseFromFile(const std::string& filename)
	{
		std::ifstream streamfile(filename);
		mFile = filename;
		Parse(streamfile);
	}

	void JsonParseMaster::Parse(std::istream& stream)
	{
		Json::Value root;
		stream >> root;

		for (auto& helper : mHelpers)
		{
			helper->Initialize();
		}

		mSharedData->IncrementDepth();
		ParseMembers(root);
		mSharedData->DecrementDepth();
	}

	void JsonParseMaster::ParseMembers(Json::Value& jsonValue, bool isArrayElement, size_t index)
	{
		for (const auto& key : jsonValue.getMemberNames())
		{
			Json::Value& value = jsonValue[key];
			ParseKeyValuePair(key, value, isArrayElement, index);
		}
	}

	void JsonParseMaster::ParseKeyValuePair(const std::string& key, Json::Value& value, bool isArrayElement, size_t index)
	{
		assert(mSharedData != nullptr);

		for (IJsonParseHelper* helper : mHelpers)
		{
			if (value.isArray())
			{
				for (Json::ArrayIndex i = 0; i < value.size(); ++i)
				{
					if (value[i].isObject())
					{
						mSharedData->IncrementDepth();
						ParseMembers(value[i], true, i);
						mSharedData->DecrementDepth();
					}
					else
					{
						ParseKeyValuePair(key, value[i], true, i);
					}
				}
				break;
			}
			else if (value.isObject())
			{
				if (helper->StartHandler(*mSharedData, key, value, isArrayElement, index))
				{
					mSharedData->IncrementDepth();
					ParseMembers(value);
					mSharedData->DecrementDepth();
					helper->EndHandler(*mSharedData, key);
					break;
				}
			}
			else
			{
				if (helper->StartHandler(*mSharedData, key, value, isArrayElement, index))
				{
					helper->EndHandler(*mSharedData, key);
					break;
				}
			}
		}
	}

	const std::string& JsonParseMaster::GetFileName() const
	{
		return mFile;
	}

	const JsonParseMaster::SharedData* JsonParseMaster::GetSharedData() const
	{
		return mSharedData;
	}

	JsonParseMaster::SharedData* JsonParseMaster::GetSharedData()
	{
		return mSharedData;
	}

	void JsonParseMaster::SetSharedData(SharedData* data)
	{
		if (mIsClone)
		{
			throw std::runtime_error("Can't changed the SharedData of a cloned ParseMaster");
		}

		mSharedData = data;
	}

	const Vector<IJsonParseHelper*>& JsonParseMaster::Helpers() const
	{
		return mHelpers;
	}

#pragma endregion JsonParseMaster

}
