#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Datum.h"
#include "Scope.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTests)
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

		TEST_METHOD(Constructor)
		{
			{
				Scope scope;
				Assert::AreEqual(scope.BucketSize(), 11_z);
				Assert::AreEqual(scope.NumAttributes(), 0_z);
				Assert::IsTrue(scope.GetParent() == nullptr);
			}

			{
				Scope scope(50);
				Assert::AreEqual(scope.BucketSize(), 50_z);
				Assert::AreEqual(scope.NumAttributes(), 0_z);
				Assert::IsTrue(scope.GetParent() == nullptr);
			}
		}

		TEST_METHOD(AppendFindIndexOperator)
		{
			Scope scope;

			Assert::ExpectException<std::runtime_error>([&scope] {scope.Append(""s); });

			scope.Append("Powers") = "Flying";
			Assert::AreEqual(scope.NumAttributes(), 1_z);
			Assert::IsTrue(scope.Find("Powers") != nullptr);
			Assert::IsTrue(*scope.Find("Powers") == "Flying"s);
			Assert::IsTrue(scope[0] == "Flying"s);
			scope["DPS"s] = 5;
			Assert::AreEqual(scope.NumAttributes(), 2_z);
			Assert::IsTrue(scope.Find("DPS") != nullptr);
			Assert::IsTrue(*scope.Find("DPS") == 5);

			const Scope& constScope = scope;
			Assert::IsTrue(constScope.Find("Powers") != nullptr);
			Assert::IsTrue(*constScope.Find("Powers") == "Flying"s);
			Assert::IsTrue(constScope.Find("DPS") != nullptr);
			Assert::IsTrue(*constScope.Find("DPS") == 5);
			Assert::IsTrue(constScope[1] == 5);

			Assert::IsTrue(scope.Find("Unknown") == nullptr);
		}

		TEST_METHOD(AppendScope)
		{
			Scope scope;
			Scope& childScope = scope.AppendScope("Powers");
			Assert::AreEqual(scope.NumAttributes(), 1_z);
			Assert::IsTrue(scope[0].Type() == Datum::DatumTypes::Table);
			Assert::AreEqual(childScope.GetParent(), &scope);
			Assert::AreEqual(&childScope, scope[0].GetScope(0));
		}

		TEST_METHOD(AdoptOrphan)
		{
			Scope scope;
			Scope* scopeChild = new Scope;

			Assert::ExpectException<std::runtime_error>([&scope, scopeChild] {scope.Adopt(*scopeChild, ""s); });
			scope.Adopt(*scopeChild, "Child"s);
			Assert::AreEqual(scopeChild->GetParent(), &scope);
			Assert::AreEqual(scope.NumAttributes(), 1_z);
			Assert::AreEqual(scope[0].GetScope(0), scopeChild);

			Scope scope2;
			Scope* scopeChild2 = new Scope;
			scope2.Adopt(*scopeChild2, "Child2"s);
			
			scope.Adopt(*scopeChild2, "AdoptedOrphan");
			Assert::AreEqual(scopeChild2->GetParent(), &scope);
			Assert::AreEqual(scope.NumAttributes(), 2_z);
			Assert::AreEqual(scope[1].GetScope(0), scopeChild2);
		}

		TEST_METHOD(Search)
		{
			Scope scope;
			Scope* scopeChild = new Scope;

			scope.Adopt(*scopeChild, "Child"s);
			scope.Append("Powers");

			{
				Scope* foundScope;
				Datum* foundDatum = scopeChild->Search("Powers", &foundScope);
				Assert::AreEqual(*foundDatum, scope[1]);
				Assert::AreEqual(foundScope, &scope);
			}

			{
				Scope* foundScope;
				const Scope& constScope = scope;
				const Datum* foundDatum = constScope.Search("Powers", &foundScope);
				Assert::AreEqual(*foundDatum, scope[1]);
				Assert::AreEqual(foundScope, &scope);
			}
		}

		TEST_METHOD(FindNestedScope)
		{
			Scope scope;
			Scope* scopeChild = new Scope;
			Scope secondChild;

			scope.Adopt(*scopeChild, "Child"s);

			{
				auto [foundDatum, index] = scope.FindNestedScope(scopeChild);
				Assert::AreEqual(*foundDatum, scope[0]);
				Assert::AreEqual(index, 0_z);
			}

			{
				auto [foundDatum, index] = scope.FindNestedScope(&secondChild);
				Assert::IsTrue(foundDatum == nullptr);
				Assert::AreEqual(index, 0_z);
			}
		}

		TEST_METHOD(Equality)
		{
			Scope scope;
			scope.Append("Family");
			scope.Append("Younglings");

			Scope scope2;
			Assert::IsTrue(scope != scope2);

			scope2.Append("Younglings");
			scope2.Append("Family");
			Assert::IsTrue(scope == scope2);

			Scope scope3;
			scope3.Append("LightSide");
			scope3.Append("DarkSide");
			Assert::IsTrue(scope != scope3);

			Scope* scopeChild1 = new Scope;
			scope.Adopt(*scopeChild1, "Family");
			Assert::IsTrue(scope != scope2);

			Scope* scopeChild2 = new Scope;
			scope2.Adopt(*scopeChild2, "Family");
			Assert::IsTrue(scope == scope2);

			scopeChild2->Append("Vader");
			Assert::IsTrue(scope != scope2);

			scopeChild1->Append("Vader");
			scope.Find("Younglings")->PushBack(5);
			Assert::IsTrue(scope != scope2);
		}

		TEST_METHOD(CopySemantics)
		{
			Scope scope;
			scope.Append("Family");
			scope.Append("Younglings");
			Scope* scopeChild1 = new Scope;
			scope.Adopt(*scopeChild1, "Family");

			{
				Scope copyScope(scope);
				Assert::AreEqual(copyScope.BucketSize(), scope.BucketSize());
				Assert::AreEqual(copyScope.NumAttributes(), scope.NumAttributes());
				Assert::IsTrue(copyScope == scope);
				Assert::AreNotSame(copyScope, scope);
			}

			{
				Scope copyScope;
				copyScope.Append("Vader");
				copyScope = scope;
				Assert::AreEqual(copyScope.BucketSize(), scope.BucketSize());
				Assert::AreEqual(copyScope.NumAttributes(), scope.NumAttributes());
				Assert::IsTrue(copyScope == scope);
				Assert::AreNotSame(copyScope, scope);

				Scope childCopy;
				childCopy.Append("Vader");
				*copyScope[0].GetScope(0) = childCopy;
				Assert::AreEqual(copyScope[0].GetScope(0)->NumAttributes(), 1_z);
				Assert::AreEqual(copyScope[0].GetScope(0)->BucketSize(), 11_z);
				Assert::AreEqual(copyScope[0].GetScope(0)->GetParent(), &copyScope);
			}
		}

		TEST_METHOD(MoveSemantics)
		{
				
			{
				Scope scope;
				scope.AppendScope("Family");
				scope.Append("Younglings");

				Scope movedScope(std::move(scope));
				Assert::AreEqual(movedScope.BucketSize(), 11_z);
				Assert::AreEqual(movedScope.NumAttributes(), 2_z);
				Assert::IsTrue(movedScope.Find("Family") != nullptr);
				Assert::IsTrue(movedScope.Find("Younglings") != nullptr);
			}

			{
				Scope scope;
				scope.AppendScope("Family");
				scope.Append("Younglings");

				Scope movedScope;
				movedScope.Append("Vader");
				movedScope = std::move(scope);
				Assert::AreEqual(movedScope.BucketSize(), 11_z);
				Assert::AreEqual(movedScope.NumAttributes(), 2_z);
				Assert::IsTrue(movedScope.Find("Family") != nullptr);
				Assert::IsTrue(movedScope.Find("Younglings") != nullptr);
			}

			{
				Scope scope;
				scope.Append("Vader");

				Scope scope2;
				scope2.Append("Younglings");

				Scope* scopeChild = new Scope;
				scopeChild->Append("Kills");
				scope.Adopt(*scopeChild, "Vader");

				Scope* scope2Child = new Scope;
				scopeChild->Append("Midichlorians");
				scope2.Adopt(*scope2Child, "Younglings");

				*scopeChild = std::move(*scope2Child);
				Assert::AreEqual(scope.Find("Vader")->Size(), 0_z);
				Assert::AreEqual(scopeChild->GetParent(), &scope2);
				Assert::IsTrue(scope2.FindNestedScope(scopeChild).first != nullptr);

				delete scope2Child;
			}
		}

		TEST_METHOD(RTTIOverloads)
		{
			Scope scope;
			scope.Append("Family");
			scope.Append("Younglings");

			Scope scope2;
			Assert::IsFalse(scope.Equals(&scope2));
			scope2 = scope;
			Assert::IsTrue(scope.Equals(&scope2));

			Foo test;
			Assert::IsFalse(scope.Equals(&test));

			Assert::AreEqual(scope.ToString(), "Scope"s);
		}

		TEST_METHOD(Ancestry)
		{
			Scope scope;
			scope.Append("Family");
			scope.Append("Younglings");

			Scope scope2;
			scope2.Append("LightSide");
			scope2.Append("DarkSide");
			Assert::IsFalse(scope.IsAncestorOf(scope2));
			Assert::IsFalse(scope.IsDescendantOf(scope2));

			Scope* scopeChild1 = new Scope;
			scope.Adopt(*scopeChild1, "Family");
			Assert::IsTrue(scope.IsAncestorOf(*scopeChild1));
			Assert::IsTrue(scopeChild1->IsDescendantOf(scope));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ScopeTests::sStartMemState;
}