#include "pch.h"
#include "CppUnitTest.h"
#pragma warning( push )
#pragma warning( disable : 26812)
#include <Json/json.h>
#pragma warning( pop )
#include <iostream>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonCPPTest)
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

		TEST_METHOD(ReadString)
		{
			const std::string rawJson = R"({"Age": 20, "Name": "colin"})";
			const auto rawJsonLength = static_cast<int>(rawJson.length());
			JSONCPP_STRING err;
			Json::Value root;

			Json::CharReaderBuilder builder;
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			Assert::IsTrue(reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err));

			Assert::AreEqual(root["Name"].asString(), "colin"s);
			Assert::AreEqual(root["Age"].asInt(), 20);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonCPPTest::sStartMemState;
}