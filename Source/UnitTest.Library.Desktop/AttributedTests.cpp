#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "TypeManager.h"
#include "Datum.h"
#include "Scope.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			TypeManager::RegisterType(AttributedFoo::TypeIdClass(), Attributed::TypeIdClass(), AttributedFoo::GetSignatures());
			TypeManager::RegisterType(AttributedBar::TypeIdClass(), AttributedFoo::TypeIdClass(), AttributedBar::GetSignatures());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeManager::Clear();

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
			AttributedFoo foo(10);
			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("Data"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalVec"));
			Assert::IsTrue(foo.IsAttribute("ExternalMat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalRTTI"));
			Assert::IsTrue(foo.IsAttribute("Array"));
			Assert::IsFalse(foo.IsAttribute("Vader"));

			Assert::IsTrue(foo["Data"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalFloat"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalVec"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalMat"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalString"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalRTTI"].IsExternalStorage());
			Assert::IsTrue(foo["Array"].IsExternalStorage());

			Assert::AreEqual(foo["this"s].GetPointer(0), reinterpret_cast<RTTI*>(&foo));
			Assert::IsTrue(foo["Data"] == foo.Data());
			Assert::AreEqual(foo["ExternalFloat"].GetFloat(0), foo.mExternalFloat);
			Assert::AreEqual(foo["ExternalVec"].GetVector(0), foo.mExternalVec);
			Assert::AreEqual(foo["ExternalMat"].GetMatrix(0), foo.mExternalMat);
			Assert::AreEqual(foo["ExternalString"].GetString(0), foo.mExternalString);
			Assert::AreEqual(foo["ExternalRTTI"].GetPointer(0), foo.mExternalRTTI);
			Assert::AreEqual(foo["Array"].GetFloat(0), foo.mArray[0]);

			AttributedBar bar;
			Assert::IsTrue(bar.IsAttribute("this"));
			Assert::IsTrue(bar.IsAttribute("Data"));
			Assert::IsTrue(bar.IsAttribute("ExternalFloat"));
			Assert::IsTrue(bar.IsAttribute("ExternalVec"));
			Assert::IsTrue(bar.IsAttribute("ExternalMat"));
			Assert::IsTrue(bar.IsAttribute("ExternalString"));
			Assert::IsTrue(bar.IsAttribute("ExternalRTTI"));
			Assert::IsTrue(bar.IsAttribute("Array"));
			Assert::IsTrue(bar.IsAttribute("ChildData"));
			Assert::IsTrue(bar.IsAttribute("Powers"));
			Assert::IsFalse(bar.IsAttribute("Vader"));
			Assert::AreEqual(bar["this"s].GetPointer(0), reinterpret_cast<RTTI*>(&bar));

			Assert::IsTrue(foo["Data"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalFloat"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalVec"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalMat"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalString"].IsExternalStorage());
			Assert::IsTrue(foo["ExternalRTTI"].IsExternalStorage());
			Assert::IsTrue(foo["Array"].IsExternalStorage());
			Assert::IsTrue(bar["ChildData"].IsExternalStorage());
			Assert::IsFalse(bar["Powers"].IsExternalStorage());

			Assert::IsTrue(bar["Data"] == bar.Data());
			Assert::AreEqual(bar["ExternalFloat"].GetFloat(0), bar.mExternalFloat);
			Assert::AreEqual(bar["ExternalVec"].GetVector(0), bar.mExternalVec);
			Assert::AreEqual(bar["ExternalMat"].GetMatrix(0), bar.mExternalMat);
			Assert::AreEqual(bar["ExternalString"].GetString(0), bar.mExternalString);
			Assert::AreEqual(bar["ExternalRTTI"].GetPointer(0), bar.mExternalRTTI);
			Assert::AreEqual(bar["Array"].GetFloat(0), bar.mArray[0]);
			Assert::AreEqual(bar["Powers"].GetScope(0), bar.Powers());

		}

		TEST_METHOD(IsAndAppend)
		{
			AttributedFoo foo;
			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("Data"));
			Assert::IsTrue(foo.IsAttribute("Array"));
			Assert::IsFalse(foo.IsAttribute("Vader"));

			Assert::IsTrue(foo.IsPrescribedAttribute("Data"));
			Assert::IsTrue(foo.IsPrescribedAttribute("Array"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Vader"));

			foo.AppendAuxilaryAttribute("Younglings");
			Assert::IsTrue(foo.IsAuxiliaryAttribute("Younglings"));
			Assert::IsTrue(foo.IsAttribute("Younglings"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Younglings"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("Data"));

			AttributedBar bar;
			Assert::IsTrue(bar.IsAttribute("Data"));
			Assert::IsTrue(bar.IsAttribute("Array"));
			Assert::IsTrue(bar.IsAttribute("ChildData"));
			Assert::IsTrue(bar.IsAttribute("Powers"));
			Assert::IsFalse(bar.IsAttribute("Vader"));

			Assert::IsTrue(bar.IsPrescribedAttribute("Data"));
			Assert::IsTrue(bar.IsPrescribedAttribute("Array"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ChildData"));
			Assert::IsTrue(bar.IsPrescribedAttribute("Powers"));
			Assert::IsFalse(bar.IsPrescribedAttribute("Vader"));

			bar.AppendAuxilaryAttribute("Younglings");
			Assert::IsTrue(bar.IsAuxiliaryAttribute("Younglings"));
			Assert::IsTrue(bar.IsAttribute("Younglings"));
			Assert::IsFalse(bar.IsPrescribedAttribute("Younglings"));
			Assert::IsFalse(bar.IsAuxiliaryAttribute("Data"));

			Assert::ExpectException<std::runtime_error>([&bar] {bar.AppendAuxilaryAttribute("Data"); });
		}

		TEST_METHOD(CopySemantics)
		{
			AttributedBar bar;
			bar.AppendAuxilaryAttribute("Family");
			bar.AppendAuxilaryAttribute("Younglings");

			{
				AttributedBar copyBar(bar);
				Assert::AreEqual(copyBar.BucketSize(), bar.BucketSize());
				Assert::AreEqual(copyBar.NumAttributes(), bar.NumAttributes());
				Assert::IsTrue(copyBar == bar);
				Assert::AreNotSame(copyBar, bar);

				Assert::IsTrue(bar["Data"] == bar.Data());
				Assert::AreEqual(bar["Array"].GetFloat(0), 1.0f);
				Assert::AreEqual(copyBar["Powers"].GetScope(0), copyBar.Powers());

				Assert::IsTrue(copyBar.Is("AttributedBar"));
			}

			{
				AttributedBar copyBar;
				copyBar = bar;
				Assert::AreEqual(copyBar.BucketSize(), bar.BucketSize());
				Assert::AreEqual(copyBar.NumAttributes(), bar.NumAttributes());
				Assert::IsTrue(copyBar == bar);
				Assert::AreNotSame(copyBar, bar);

				Assert::IsTrue(copyBar["Data"] == bar.Data());
				Assert::AreEqual(copyBar["Array"].GetFloat(0), 1.0f);
				Assert::AreEqual(copyBar["Powers"].GetScope(0), copyBar.Powers());
				Assert::IsTrue(copyBar.Is("AttributedBar"));
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			{
				AttributedBar bar;
				bar.AppendAuxilaryAttribute("Family");
				bar.AppendAuxilaryAttribute("Younglings");

				AttributedBar moveBar(std::move(bar));
				Assert::AreEqual(moveBar.BucketSize(), 11_z);
				Assert::AreEqual(moveBar.NumAttributes(), 12_z);

				Assert::IsTrue(moveBar["Data"] == moveBar.Data());
				Assert::AreEqual(moveBar["ExternalFloat"].GetFloat(0), moveBar.mExternalFloat);
				Assert::AreEqual(moveBar["ExternalVec"].GetVector(0), moveBar.mExternalVec);
				Assert::AreEqual(moveBar["ExternalMat"].GetMatrix(0), moveBar.mExternalMat);
				Assert::AreEqual(moveBar["ExternalString"].GetString(0), moveBar.mExternalString);
				Assert::AreEqual(moveBar["ExternalRTTI"].GetPointer(0), moveBar.mExternalRTTI);
				Assert::AreEqual(moveBar["Array"].GetFloat(0), 1.0f);
				Assert::AreEqual(moveBar["Powers"].GetScope(0), moveBar.Powers());

				Assert::IsTrue(moveBar.Is("AttributedBar"));
			}

			{
				AttributedBar bar;
				bar.AppendAuxilaryAttribute("Family");
				bar.AppendAuxilaryAttribute("Younglings");

				AttributedBar moveBar;
				moveBar = std::move(bar);
				Assert::AreEqual(moveBar.BucketSize(), 11_z);
				Assert::AreEqual(moveBar.NumAttributes(), 12_z);

				Assert::IsTrue(moveBar["Data"] == moveBar.Data());
				Assert::AreEqual(moveBar["ExternalFloat"].GetFloat(0), moveBar.mExternalFloat);
				Assert::AreEqual(moveBar["ExternalVec"].GetVector(0), moveBar.mExternalVec);
				Assert::AreEqual(moveBar["ExternalMat"].GetMatrix(0), moveBar.mExternalMat);
				Assert::AreEqual(moveBar["ExternalString"].GetString(0), moveBar.mExternalString);
				Assert::AreEqual(moveBar["ExternalRTTI"].GetPointer(0), moveBar.mExternalRTTI);
				Assert::AreEqual(moveBar["Array"].GetFloat(0), 1.0f);
				Assert::AreEqual(moveBar["Powers"].GetScope(0), moveBar.Powers());

				Assert::IsTrue(moveBar.Is("AttributedBar"));
			}
		}

		TEST_METHOD(Accessors)
		{
			AttributedBar bar;
			auto attributes = bar.GetAttributes();
			Assert::AreEqual(attributes.Size(), 10_z);
			Assert::AreEqual(attributes[0]->first, "this"s);
			Assert::AreEqual(attributes[1]->first, "Data"s);
			Assert::AreEqual(attributes[2]->first, "ExternalFloat"s);
			Assert::AreEqual(attributes[3]->first, "ExternalVec"s);
			Assert::AreEqual(attributes[4]->first, "ExternalMat"s);
			Assert::AreEqual(attributes[5]->first, "ExternalString"s);
			Assert::AreEqual(attributes[6]->first, "ExternalRTTI"s);
			Assert::AreEqual(attributes[7]->first, "Array"s);
			Assert::AreEqual(attributes[8]->first, "ChildData"s);
			Assert::AreEqual(attributes[9]->first, "Powers"s);
		}

		TEST_METHOD(HelperClassRTTITests)
		{
			AttributedFoo foo;
			AttributedBar bar;

			Assert::IsTrue(foo.Is("Attributed"));
			Assert::IsTrue(foo.Is("Scope"));
			Assert::IsTrue(foo.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(foo.Is(Scope::TypeIdClass()));

			Assert::IsTrue(bar.Is("AttributedFoo"));
			Assert::IsTrue(bar.Is("Attributed"));
			Assert::IsTrue(bar.Is("Scope"));
			Assert::IsTrue(bar.Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(bar.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(bar.Is(Scope::TypeIdClass()));

			foo.Adopt(*(new AttributedFoo()), "Powers");
			bar.Adopt(*(new AttributedBar()), "Powers");

			AttributedFoo fooCopy(foo);
			AttributedBar barCopy(bar);

			Assert::IsTrue(foo.Equals(&foo));
			Assert::IsFalse(fooCopy.Equals(fooCopy["Powers"].GetScope(0)));
			Assert::IsFalse(fooCopy.Equals(nullptr));

			Assert::IsTrue(barCopy.Equals(&barCopy));
			Assert::IsFalse(barCopy.Equals(barCopy["Powers"].GetScope(1)));
			Assert::IsFalse(barCopy.Equals(nullptr));

			Assert::IsTrue(fooCopy["Powers"].GetScope(0)->Is("AttributedFoo"));
			Assert::IsTrue(barCopy["Powers"].GetScope(1)->Is("AttributedBar"));

			Assert::AreEqual(fooCopy.ToString(), "AttributedFoo"s);
			Assert::AreEqual(barCopy.ToString(), "AttributedBar"s);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState AttributedTests::sStartMemState;
}