#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"
#include "Stack.h"
#include <fstream>
#include <istream>
#include <iostream>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;

namespace Library
{
	class Baz
	{
	public:
		static const int ArraySize = 6;

		int mInt = 0;
		float mFloat = 0;
		std::string mString;
		int mArray[ArraySize] = {0,0,0,0,0,0};
	};

	class BazParseHelper : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(BazParseHelper, IJsonParseHelper);

	public:

		struct StackFrame
		{
			const std::string& key;
			Json::Value& value;
			bool isArrayElement;
			size_t index;
		};

		class BazSharedData : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(BazSharedData, JsonParseMaster::SharedData);

		public:

			BazSharedData(Baz* baz = nullptr) : mBaz(baz) {}
			BazSharedData(const BazSharedData& rhs) = default;
			BazSharedData(BazSharedData&& rhs) noexcept = default;
			BazSharedData& operator=(const BazSharedData& rhs) = default;
			BazSharedData& operator=(BazSharedData&& rhs) noexcept = default;
			~BazSharedData() = default;

			BazSharedData* Create() override
			{
				return new BazSharedData;
			}

			void SetBaz(Baz* baz) { mBaz = baz; }

			Baz* mBaz;
		};

		void Initialize() override { mInitialized = true; }

		IJsonParseHelper* Create() override
		{
			return new BazParseHelper;
		}

		bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey, Json::Value& object, bool isArrayElement, size_t index)
		{
			if (!sharedData.Is(BazSharedData::TypeIdClass()) || (!object.isArray() && !object.isInt() && !object.isDouble() && !object.isString()))
			{
				return false;
			}

			if (jsonKey != "mInt" && jsonKey != "mFloat" && jsonKey != "mArray" && jsonKey != "mString")
			{
				return false;
			}

			mStack.Push({jsonKey, object, isArrayElement, index});

			return true;
		}

		bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& jsonKey) override
		{
			if (!sharedData.Is(BazSharedData::TypeIdClass()))
			{
				return false;
			}

			StackFrame frame = mStack.Pop();
			if (jsonKey != frame.key)
			{
				return false;
			}

			if (frame.key == "mInt")
			{
				sharedData.As<BazSharedData>()->mBaz->mInt = frame.value.asInt();
			}

			if (frame.key == "mFloat")
			{
				sharedData.As<BazSharedData>()->mBaz->mFloat = frame.value.asFloat();
			}

			if (frame.key == "mString")
			{
				sharedData.As<BazSharedData>()->mBaz->mString = frame.value.asString();
			}

			if (frame.key == "mArray")
			{
				if (frame.isArrayElement)
				{
					sharedData.As<BazSharedData>()->mBaz->mArray[frame.index] = frame.value.asInt();
				}
			}

			return true;
		}

		bool mInitialized = false;

	private:
		Stack<StackFrame> mStack;
	};

	class TestParseHelper : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(TestParseHelper, IJsonParseHelper);

	public:

		class TestSharedData : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(TestSharedData, JsonParseMaster::SharedData);

		public:

			TestSharedData* Create() { return new TestSharedData; }

			size_t mStartHandlersCalled = 0;
			size_t mEndHandlersCalled = 0;
			size_t mMaxDepth = 0;
		};

		IJsonParseHelper* Create() override
		{
			return new TestParseHelper;
		}

		bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string&, Json::Value&, bool, size_t)
		{
			if (!sharedData.Is(TestSharedData::TypeIdClass()))
			{
				return false;
			}

			sharedData.As<TestSharedData>()->mStartHandlersCalled++;
			sharedData.As<TestSharedData>()->mMaxDepth = std::max(sharedData.Depth(), sharedData.As<TestSharedData>()->mMaxDepth);

			return true;
		}

		bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string&) override
		{
			sharedData.As<TestSharedData>()->mEndHandlersCalled++;
			return true;
		}
	};

	RTTI_DEFINITIONS(BazParseHelper);

	RTTI_DEFINITIONS(BazParseHelper::BazSharedData);

	RTTI_DEFINITIONS(TestParseHelper);

	RTTI_DEFINITIONS(TestParseHelper::TestSharedData);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonParseMasterTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		// TODO: RTTI tests for shared data

		TEST_METHOD(TestParser)
		{
			TestParseHelper::TestSharedData sharedData;
			TestParseHelper parseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(parseHelper);

			Assert::ExpectException<std::runtime_error>([&parseMaster, &parseHelper] { parseMaster.AddHelper(parseHelper); });

			const std::string rawJson = R"({
				"mInt": 20, 
				"mFloat": 5.5, 
				"mString": "Hello", 
				"mArray": [1, 2, 3, 4, 5, 6], 
				"mObject" : { "mInt" : 30 },
				"mObjectArray": [
					{ "mInt" : 30 },
					{ "mObject" : { "mString" : "Goodbye" } }
				]
			})";

			parseMaster.Parse(rawJson);
			Assert::AreEqual(sharedData.mStartHandlersCalled, 14_z);
			Assert::AreEqual(sharedData.mEndHandlersCalled, 14_z);
			Assert::AreEqual(sharedData.mMaxDepth, 3_z);
			Assert::AreEqual(sharedData.Depth(), 0_z);
		}

		TEST_METHOD(BazParseString)
		{
			Baz baz;
			BazParseHelper::BazSharedData sharedData(&baz);
			JsonParseMaster parseMaster(sharedData);
			BazParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			const BazParseHelper::BazSharedData& constSharedData = sharedData;


			Assert::AreEqual(sharedData.GetJsonParseMaster(), &parseMaster);
			Assert::AreEqual(constSharedData.GetJsonParseMaster(), const_cast<const JsonParseMaster*>(&parseMaster));

			const std::string rawJson = R"({"mInt": 20, "mFloat": 5.5, "mString": "Hello", "mArray": [1, 2, 3, 4, 5, 6]})";

			parseMaster.Parse(rawJson);
			Assert::AreEqual(baz.mInt, 20);
			Assert::AreEqual(baz.mFloat, 5.5f);
			Assert::AreEqual(baz.mString, "Hello"s);
			Assert::AreEqual(sharedData.Depth(), 0_z);


			int testArray[] = { 1, 2, 3, 4, 5, 6 };
			for (size_t i = 0; i < Baz::ArraySize; ++i)
			{
				Assert::AreEqual(baz.mArray[i], testArray[i]);
			}
			
			parseMaster.RemoveHelper(parseHelper);
		}


		TEST_METHOD(BazParseFile)
		{
			std::ofstream file;
			file.open("ParseTest.json");

			if (file.fail())
			{
				throw std::runtime_error("Unable to open file");
			}

			file << R"({"mInt": 20, "mFloat": 5.5, "mString": "Hello", "mArray": [1, 2, 3, 4, 5, 6]})";
			file.close();

			Baz baz;
			BazParseHelper::BazSharedData sharedData(&baz);
			JsonParseMaster parseMaster(sharedData);
			BazParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.ParseFromFile("ParseTest.json");
			Assert::AreEqual(baz.mInt, 20);
			Assert::AreEqual(baz.mFloat, 5.5f);
			Assert::AreEqual(baz.mString, "Hello"s);
			Assert::AreEqual(sharedData.Depth(), 0_z);


			int testArray[] = { 1, 2, 3, 4, 5, 6 };
			for (size_t i = 0; i < Baz::ArraySize; ++i)
			{
				Assert::AreEqual(baz.mArray[i], testArray[i]);
			}

			Assert::AreEqual(parseMaster.GetFileName(), "ParseTest.json"s);
		}

		TEST_METHOD(BazParseStream)
		{
			std::ofstream file;
			file.open("ParseTest.json");
			file << R"({"mInt": 20, "mFloat": 5.5, "mString": "Hello", "mArray": [1, 2, 3, 4, 5, 6]})";
			file.close();

			std::filebuf fb;
			fb.open("ParseTest.json", std::ios::in);

			std::istream s(&fb);

			Baz baz;
			BazParseHelper::BazSharedData sharedData(&baz);
			JsonParseMaster parseMaster(sharedData);
			BazParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.Parse(s);
			Assert::AreEqual(baz.mInt, 20);
			Assert::AreEqual(baz.mFloat, 5.5f);
			Assert::AreEqual(baz.mString, "Hello"s);
			Assert::AreEqual(sharedData.Depth(), 0_z);


			int testArray[] = { 1, 2, 3, 4, 5, 6 };
			for (size_t i = 0; i < Baz::ArraySize; ++i)
			{
				Assert::AreEqual(baz.mArray[i], testArray[i]);
			}
		}

		TEST_METHOD(SharedDataMove)
		{
			{
				Baz baz;
				BazParseHelper::BazSharedData sharedData(&baz);
				JsonParseMaster parseMaster(sharedData);
				BazParseHelper parseHelper;
				parseMaster.AddHelper(parseHelper);
				BazParseHelper::BazSharedData sharedData2(std::move(sharedData));
				Assert::AreEqual(sharedData2.GetJsonParseMaster(), &parseMaster);
				Assert::AreEqual(sharedData2.Depth(), 0_z);
				Assert::AreEqual(parseMaster.GetSharedData(), reinterpret_cast<JsonParseMaster::SharedData*>(&sharedData2));
				Assert::IsTrue(sharedData2.mBaz == &baz);

				const JsonParseMaster& constParseMaster = parseMaster;
				Assert::AreEqual(const_cast<JsonParseMaster::SharedData*>(constParseMaster.GetSharedData()), reinterpret_cast<JsonParseMaster::SharedData*>(&sharedData2));
			}

			{
				Baz baz;
				BazParseHelper::BazSharedData sharedData(&baz);
				JsonParseMaster parseMaster(sharedData);
				BazParseHelper parseHelper;
				parseMaster.AddHelper(parseHelper);
				BazParseHelper::BazSharedData sharedData2;
				sharedData2 = std::move(sharedData);
				Assert::AreEqual(sharedData2.GetJsonParseMaster(), &parseMaster);
				Assert::AreEqual(sharedData2.Depth(), 0_z);
				Assert::AreEqual(parseMaster.GetSharedData(), reinterpret_cast<JsonParseMaster::SharedData*>(&sharedData2));
				Assert::IsTrue(sharedData2.mBaz == &baz);
			}
		}

		TEST_METHOD(ParserMove)
		{
			{
				Baz baz;
				BazParseHelper::BazSharedData sharedData(&baz);
				JsonParseMaster parseMaster(sharedData);
				BazParseHelper parseHelper;
				parseMaster.AddHelper(parseHelper);

				JsonParseMaster parseMaster2(std::move(parseMaster));

				parseMaster2.ParseFromFile("ParseTest.json");
				Assert::AreEqual(baz.mInt, 20);
				Assert::AreEqual(baz.mFloat, 5.5f);
				Assert::AreEqual(baz.mString, "Hello"s);
				Assert::AreEqual(sharedData.Depth(), 0_z);


				int testArray[] = { 1, 2, 3, 4, 5, 6 };
				for (size_t i = 0; i < Baz::ArraySize; ++i)
				{
					Assert::AreEqual(baz.mArray[i], testArray[i]);
				}
			}

			{
				Baz baz;
				BazParseHelper::BazSharedData sharedData(&baz);
				JsonParseMaster parseMaster(sharedData);
				BazParseHelper parseHelper;
				parseMaster.AddHelper(parseHelper);

				JsonParseMaster parseMaster2(sharedData);
				parseMaster2 = std::move(parseMaster);

				parseMaster2.ParseFromFile("ParseTest.json");
				Assert::AreEqual(baz.mInt, 20);
				Assert::AreEqual(baz.mFloat, 5.5f);
				Assert::AreEqual(baz.mString, "Hello"s);
				Assert::AreEqual(sharedData.Depth(), 0_z);


				int testArray[] = { 1, 2, 3, 4, 5, 6 };
				for (size_t i = 0; i < Baz::ArraySize; ++i)
				{
					Assert::AreEqual(baz.mArray[i], testArray[i]);
				}
			}
		}

		TEST_METHOD(Clone)
		{
			Baz baz;
			BazParseHelper::BazSharedData sharedData(&baz);
			JsonParseMaster parseMaster(sharedData);
			BazParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			JsonParseMaster* parseMaster2 = parseMaster.Clone();
			parseMaster2->GetSharedData()->As<BazParseHelper::BazSharedData>()->SetBaz(&baz);

			const std::string rawJson = R"({"mInt": 20, "mFloat": 5.5, "mString": "Hello", "mArray": [1, 2, 3, 4, 5, 6]})";

			parseMaster2->Parse(rawJson);
			Assert::AreEqual(baz.mInt, 20);
			Assert::AreEqual(baz.mFloat, 5.5f);
			Assert::AreEqual(baz.mString, "Hello"s);
			Assert::AreEqual(sharedData.Depth(), 0_z);

			int testArray[] = { 1, 2, 3, 4, 5, 6 };
			for (size_t i = 0; i < Baz::ArraySize; ++i)
			{
				Assert::AreEqual(baz.mArray[i], testArray[i]);
			}

			Assert::ExpectException<std::runtime_error>([&parseMaster2, &sharedData] { parseMaster2->SetSharedData(&sharedData); });
			Assert::ExpectException<std::runtime_error>([&parseMaster2, &parseHelper] { parseMaster2->AddHelper(parseHelper); });

			JsonParseMaster* parseMaster3 = parseMaster.Clone();
			parseMaster2->RemoveHelper(const_cast<IJsonParseHelper&>(*parseMaster2->Helpers().Back()));

			delete parseMaster2;
			delete parseMaster3;
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonParseMasterTests::sStartMemState;
}