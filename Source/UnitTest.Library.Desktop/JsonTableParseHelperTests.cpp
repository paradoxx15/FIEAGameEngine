#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "Stack.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "TypeManager.h"
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
	class TestSharedData : public JsonParseMaster::SharedData
	{
		RTTI_DECLARATIONS(TestSharedData, JsonParseMaster::SharedData);

	public:

		TestSharedData* Create() override
		{
			return new TestSharedData;
		}
	};

	RTTI_DEFINITIONS(TestSharedData);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonTableParseHelperTests)
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

		TEST_METHOD(RTTITests)
		{
			Scope scope;
			JsonTableParseHelper::TableSharedData sharedData(&scope);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;

			Assert::IsTrue(sharedData.Is(JsonParseMaster::SharedData::TypeIdClass()));
			Assert::IsTrue(parseHelper.Is(IJsonParseHelper::TypeIdClass()));

			Assert::IsTrue(sharedData.As<JsonParseMaster::SharedData>() == &sharedData);
			Assert::IsTrue(parseHelper.As<IJsonParseHelper>() == &parseHelper);

			Assert::AreEqual(sharedData.ToString(), "TableSharedData"s);
			Assert::AreEqual(parseHelper.ToString(), "JsonTableParseHelper"s);

			Assert::IsTrue(sharedData.Equals(&sharedData));
			Assert::IsTrue(parseHelper.Equals(&parseHelper));
		}

		TEST_METHOD(BadSharedData)
		{
			Scope scope;
			TestSharedData sharedData;
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			Assert::AreEqual(sharedData.GetJsonParseMaster(), &parseMaster);

			const std::string rawJson = R"ESC(
				{
					"Pointers": { "pointer": 5 },
					"Health": { "integer": 10 }, 
					"DPS": { "float": 5.5 }, 
					"Name": { "string": "Vader" }, 
					"Location": { "vector": "vec4(1.0, 1.0, 1.0, 1.0)" }, 
					"Transform": { "matrix": "mat4x4((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))" }, 
					"Numbers": { "integer": [1, 2, 3, 4, 5, 6] },
					"Floats": { "float": [1.0, 2.0, 3.0, 4.0, 5.0, 6.0] },
					"Strings": { "string": ["hello", "goodbye", "adios"] },
					"Vectors": { "vector": ["vec4(1.0, 1.0, 1.0, 1.0)", "vec4(2.0, 2.0, 2.0, 2.0)", "vec4(3.0, 3.0, 3.0, 3.0)"] },
					"Matrices": { "matrix": ["mat4x4((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))", "mat4x4((2.0, 0.0, 0.0, 0.0), (0.0, 2.0, 0.0, 0.0), (0.0, 0.0, 2.0, 0.0), (0.0, 0.0, 0.0, 2.0))"] }
				})ESC";

			parseMaster.Parse(rawJson);
			Assert::AreEqual(scope.NumAttributes(), 0_z);
		}

		TEST_METHOD(ParseScopePrimitive)
		{
			Scope scope;
			JsonTableParseHelper::TableSharedData sharedData(&scope);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			Assert::AreEqual(sharedData.GetJsonParseMaster(), &parseMaster);

			const std::string rawJson = R"ESC(
				{
					"Pointers": { "pointer": 5 },
					"Health": { "integer": 10 }, 
					"DPS": { "float": 5.5 }, 
					"Name": { "string": "Vader" }, 
					"Location": { "vector": "vec4(1.0, 1.0, 1.0, 1.0)" }, 
					"Transform": { "matrix": "mat4x4((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))" }, 
					"Numbers": { "integer": [1, 2, 3, 4, 5, 6] },
					"Floats": { "float": [1.0, 2.0, 3.0, 4.0, 5.0, 6.0] },
					"Strings": { "string": ["hello", "goodbye", "adios"] },
					"Vectors": { "vector": ["vec4(1.0, 1.0, 1.0, 1.0)", "vec4(2.0, 2.0, 2.0, 2.0)", "vec4(3.0, 3.0, 3.0, 3.0)"] },
					"Matrices": { "matrix": ["mat4x4((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))", "mat4x4((2.0, 0.0, 0.0, 0.0), (0.0, 2.0, 0.0, 0.0), (0.0, 0.0, 2.0, 0.0), (0.0, 0.0, 0.0, 2.0))"] }
				})ESC";

			parseMaster.Parse(rawJson);
			Assert::AreEqual(scope["Health"].GetInt(), 10);
			Assert::AreEqual(scope["DPS"].GetFloat(), 5.5f);
			Assert::AreEqual(scope["Name"].GetString(), "Vader"s);
			Assert::AreEqual(scope["Location"].GetVector(), glm::vec4(1.0));
			Assert::AreEqual(scope["Transform"].GetMatrix(), glm::mat4(1.0));
			Assert::AreEqual(scope["Numbers"].Size(), 6_z);
			Assert::AreEqual(scope["Floats"].Size(), 6_z);
			Assert::AreEqual(scope["Strings"].Size(), 3_z);
			Assert::AreEqual(scope["Vectors"].Size(), 3_z);
			Assert::AreEqual(scope["Matrices"].Size(), 2_z);

			// Used to make sure that unsupported types don't do anything
			Assert::IsTrue(scope.Find("Pointers")->Type() == Datum::DatumTypes::Unknown);

			for (int i = 0; i < 6; i++)
			{
				Assert::AreEqual(scope["Numbers"].GetInt(i), (i + 1));
			}

			for (int i = 0; i < 6; i++)
			{
				Assert::AreEqual(scope["Floats"].GetFloat(i), (i + 1.0f));
			}

			Assert::AreEqual(scope["Strings"].GetString(0), "hello"s);
			Assert::AreEqual(scope["Strings"].GetString(1), "goodbye"s);
			Assert::AreEqual(scope["Strings"].GetString(2), "adios"s);

			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(scope["Vectors"].GetVector(i), glm::vec4(static_cast<float>(i + 1)));
			}

			for (int i = 0; i < 2; i++)
			{
				Assert::AreEqual(scope["Matrices"].GetMatrix(i), glm::mat4(static_cast<float>(i + 1)));
			}
		}

		TEST_METHOD(ParseScopeObjects)
		{
			Scope scope;
			JsonTableParseHelper::TableSharedData sharedData(&scope);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			Assert::AreEqual(sharedData.GetJsonParseMaster(), &parseMaster);

			scope.AppendScope("Resources");

			const std::string rawJson = R"(
				{
					"Resources": 
					{ 
						"table": 
						{ 
							"Health": { "integer": 10 },
							"Mana": { "float": 5.5 } 
						} 
					}, 
					"Powers":
					{ 
						"table":
						[ 
							{ 
								"table": 
								{
									"Flight": { "integer": 1 }
								}
							},
							{ 
								"table": 
								{
									"Strength": 
									{ 
										"table": 
										{ 
											"Super": { "integer": 2 },
											"Normal": { "integer": 1 }
										}
									}
								}
							}
						]
					} 
				})";

			parseMaster.Parse(rawJson);

			Assert::AreEqual(scope["Resources"].GetScope()->operator[]("Health").GetInt(), 10);
			Assert::AreEqual(scope["Resources"].GetScope()->operator[]("Mana").GetFloat(), 5.5f);
			Assert::AreEqual(scope["Powers"].Size(), 2_z);
			Assert::AreEqual(scope["Powers"].GetScope(0)->operator[]("Flight").GetInt(), 1);
			Assert::AreEqual(scope["Powers"].GetScope(1)->operator[]("Strength").GetScope()->operator[]("Super").GetInt(), 2);
			Assert::AreEqual(scope["Powers"].GetScope(1)->operator[]("Strength").GetScope()->operator[]("Normal").GetInt(), 1);
		}

		TEST_METHOD(ParseClassedObject)
		{
			TypeManager::RegisterType(AttributedFoo::TypeIdClass(), Attributed::TypeIdClass(), AttributedFoo::GetSignatures());
			TypeManager::RegisterType(AttributedBar::TypeIdClass(), AttributedFoo::TypeIdClass(), AttributedBar::GetSignatures());
			AttributedFooFactory fooFactory;
			AttributedBarFactory barFactory;

			Scope scope;
			JsonTableParseHelper::TableSharedData sharedData(&scope);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			const std::string rawJson = R"(
				{
					"AttributedFoo": 
					{ 
						"class": "AttributedFoo",
						"table": 
						{ 
							"Data": { "integer": 10 },
							"ExternalFloat": { "float": 5.5 } 
						} 
					}, 
					"AttributedBar": 
					{ 
						"class": "AttributedBar",
						"table": 
						{ 
							"ChildData": { "integer": 20 },
							"ExternalFloat": { "float": 10.5 } 
						} 
					} 
				})";

			parseMaster.Parse(rawJson);

			Assert::IsTrue(scope["AttributedFoo"].GetScope()->Is(AttributedFoo::TypeIdClass()));
			Assert::AreEqual(scope["AttributedFoo"].GetScope()->operator[]("Data").GetInt(), 10);
			Assert::AreEqual(scope["AttributedFoo"].GetScope()->operator[]("ExternalFloat").GetFloat(), 5.5f);

			Assert::IsTrue(scope["AttributedBar"].GetScope()->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(scope["AttributedBar"].GetScope()->Is(AttributedBar::TypeIdClass()));
			Assert::AreEqual(scope["AttributedBar"].GetScope()->operator[]("ChildData").GetInt(), 20);
			Assert::AreEqual(scope["AttributedBar"].GetScope()->operator[]("ExternalFloat").GetFloat(), 10.5f);
		
			TypeManager::Clear();
		}

		TEST_METHOD(ParseAttributedFoo)
		{
			TypeManager::RegisterType(AttributedFoo::TypeIdClass(), Attributed::TypeIdClass(), AttributedFoo::GetSignatures());

			AttributedFoo foo;
			JsonTableParseHelper::TableSharedData sharedData(&foo);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			Assert::AreEqual(sharedData.GetJsonParseMaster(), &parseMaster);

			const std::string rawJson = R"ESC(
				{
					"Data": { "integer": 10 }, 
					"ExternalFloat": { "float": 5.5 }, 
					"ExternalVec": { "vector": "vec4(1.0, 1.0, 1.0, 1.0)" }, 
					"ExternalMat": { "matrix": "mat4x4((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))" }, 
					"ExternalString": { "string": "Vader" }, 
					"Array": { "float": [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]}
				})ESC";

			parseMaster.Parse(rawJson);

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("Data"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalVec"));
			Assert::IsTrue(foo.IsAttribute("ExternalMat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalRTTI"));
			Assert::IsTrue(foo.IsAttribute("Array"));

			Assert::IsTrue(foo["Data"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalFloat"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalVec"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalMat"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalString"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalRTTI"].IsExternalStorage());
			Assert::IsTrue(foo["Array"].IsExternalStorage());

			Assert::IsTrue(foo["Data"] == foo.Data());
			Assert::AreEqual(foo["ExternalFloat"].GetFloat(), foo.mExternalFloat);
			Assert::AreEqual(foo["ExternalVec"].GetVector(), foo.mExternalVec);
			Assert::AreEqual(foo["ExternalMat"].GetMatrix(), foo.mExternalMat);
			Assert::AreEqual(foo["ExternalString"].GetString(), foo.mExternalString);
			Assert::AreEqual(foo["ExternalRTTI"].GetPointer(), foo.mExternalRTTI);
			Assert::AreEqual(foo["Array"].GetFloat(0), foo.mArray[0]);

			Assert::AreEqual(foo["Data"].GetInt(), 10);
			Assert::AreEqual(foo["ExternalFloat"].GetFloat(), 5.5f);
			Assert::AreEqual(foo["ExternalVec"].GetVector(), glm::vec4(1.0f));
			Assert::AreEqual(foo["ExternalMat"].GetMatrix(), glm::mat4(1.0f));
			Assert::AreEqual(foo["ExternalString"].GetString(), "Vader"s);

			for (size_t i = 0; i < 6; i++)
			{
				Assert::AreEqual(foo["Array"].GetFloat(i), (i+1.0f));
			}

			TypeManager::Clear();
		}

		TEST_METHOD(GetAndCreate)
		{
			Scope scope;
			Scope scope2;
			JsonTableParseHelper::TableSharedData sharedData(&scope);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			const JsonTableParseHelper::TableSharedData& constSharedData = sharedData;

			sharedData.SetScope(scope2);
			Assert::AreEqual(const_cast<Scope*>(constSharedData.GetScope()), &scope2);

			JsonTableParseHelper::TableSharedData* createdSharedData = sharedData.Create();
			Assert::IsTrue(createdSharedData->GetScope() == nullptr);

			IJsonParseHelper* createdParseHelper = parseHelper.Create();
			Assert::IsFalse(createdParseHelper == &parseHelper);
			Assert::IsTrue(createdParseHelper->TypeIdInstance() == parseHelper.TypeIdInstance());

			delete createdSharedData;
			delete createdParseHelper;
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonTableParseHelperTests::sStartMemState;
}