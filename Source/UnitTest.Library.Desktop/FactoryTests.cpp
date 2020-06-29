#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "TypeManager.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTests)
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

		TEST_METHOD(CreateAndRemoveFactories)
		{
			Assert::AreEqual(Factory<Scope>::Factories().Size(), 0_z);
			Assert::IsTrue(Factory<Scope>::Find("AttributedFoo") == nullptr);

			{
				AttributedFooFactory fooFactory;
				Assert::AreEqual(Factory<Scope>::Factories().Size(), 1_z);
				const Factory<Scope>* fooFactoryTest = Factory<Scope>::Find("AttributedFoo");
				Assert::IsTrue(fooFactoryTest != nullptr);
				if (fooFactoryTest != nullptr)
				{
					Assert::AreEqual(fooFactoryTest->ClassName(), "AttributedFoo"s);
				}

				Assert::ExpectException<std::runtime_error>([] { AttributedFooFactory fooFactory2; });

				AttributedBarFactory barFatory;
				Assert::AreEqual(Factory<Scope>::Factories().Size(), 2_z);
				const Factory<Scope>* barFactoryTest = Factory<Scope>::Find("AttributedBar");
				Assert::IsTrue(barFactoryTest != nullptr);
				if (barFactoryTest != nullptr)
				{
					Assert::AreEqual(barFactoryTest->ClassName(), "AttributedBar"s);
				}
			}

			Assert::AreEqual(Factory<Scope>::Factories().Size(), 0_z);
			Assert::IsTrue(Factory<Scope>::Find("AttributedFoo") == nullptr);
		}

		TEST_METHOD(CreateConcrete)
		{
			Assert::IsTrue(Factory<Scope>::Create("AttributedFoo") == nullptr);

			AttributedFooFactory fooFactory;
			Scope* foo = Factory<Scope>::Create("AttributedFoo");
			Assert::IsTrue(foo->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(foo->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(foo->Is(Scope::TypeIdClass()));

			AttributedFoo* attributedFoo = foo->As<AttributedFoo>();
			Assert::IsTrue(attributedFoo->IsAttribute("this"));
			Assert::IsTrue(attributedFoo->IsAttribute("Data"));
			Assert::IsTrue(attributedFoo->IsAttribute("ExternalFloat"));
			Assert::IsTrue(attributedFoo->IsAttribute("ExternalVec"));
			Assert::IsTrue(attributedFoo->IsAttribute("ExternalMat"));
			Assert::IsTrue(attributedFoo->IsAttribute("ExternalString"));
			Assert::IsTrue(attributedFoo->IsAttribute("ExternalRTTI"));
			Assert::IsTrue(attributedFoo->IsAttribute("Array"));

			Assert::IsTrue(attributedFoo->operator[]("Data").IsExternalStorage());
			Assert::IsTrue(attributedFoo->operator[]("ExternalFloat").IsExternalStorage());
			Assert::IsTrue(attributedFoo->operator[]("ExternalVec").IsExternalStorage());
			Assert::IsTrue(attributedFoo->operator[]("ExternalMat").IsExternalStorage());
			Assert::IsTrue(attributedFoo->operator[]("ExternalString").IsExternalStorage());
			Assert::IsTrue(attributedFoo->operator[]("ExternalRTTI").IsExternalStorage());
			Assert::IsTrue(attributedFoo->operator[]("Array").IsExternalStorage());

			Assert::AreEqual(attributedFoo->operator[]("this"s).GetPointer(0), reinterpret_cast<RTTI*>(attributedFoo));
			Assert::IsTrue(attributedFoo->operator[]("Data") == attributedFoo->Data());
			Assert::AreEqual(attributedFoo->operator[]("ExternalFloat").GetFloat(0), attributedFoo->mExternalFloat);
			Assert::AreEqual(attributedFoo->operator[]("ExternalVec").GetVector(0), attributedFoo->mExternalVec);
			Assert::AreEqual(attributedFoo->operator[]("ExternalMat").GetMatrix(0), attributedFoo->mExternalMat);
			Assert::AreEqual(attributedFoo->operator[]("ExternalString").GetString(0), attributedFoo->mExternalString);
			Assert::AreEqual(attributedFoo->operator[]("ExternalRTTI").GetPointer(0), attributedFoo->mExternalRTTI);
			Assert::AreEqual(attributedFoo->operator[]("Array").GetFloat(0), attributedFoo->mArray[0]);

			AttributedBarFactory barFactory;
			Scope* bar = Factory<Scope>::Create("AttributedBar");
			Assert::IsTrue(bar->Is(AttributedBar::TypeIdClass()));
			Assert::IsTrue(bar->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(bar->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(bar->Is(Scope::TypeIdClass()));

			AttributedBar* attributedBar = bar->As<AttributedBar>();
			Assert::IsTrue(attributedBar->IsAttribute("this"));
			Assert::IsTrue(attributedBar->IsAttribute("Data"));
			Assert::IsTrue(attributedBar->IsAttribute("ExternalFloat"));
			Assert::IsTrue(attributedBar->IsAttribute("ExternalVec"));
			Assert::IsTrue(attributedBar->IsAttribute("ExternalMat"));
			Assert::IsTrue(attributedBar->IsAttribute("ExternalString"));
			Assert::IsTrue(attributedBar->IsAttribute("ExternalRTTI"));
			Assert::IsTrue(attributedBar->IsAttribute("Array"));

			Assert::IsTrue(attributedBar->operator[]("Data").IsExternalStorage());
			Assert::IsTrue(attributedBar->operator[]("ExternalFloat").IsExternalStorage());
			Assert::IsTrue(attributedBar->operator[]("ExternalVec").IsExternalStorage());
			Assert::IsTrue(attributedBar->operator[]("ExternalMat").IsExternalStorage());
			Assert::IsTrue(attributedBar->operator[]("ExternalString").IsExternalStorage());
			Assert::IsTrue(attributedBar->operator[]("ExternalRTTI").IsExternalStorage());
			Assert::IsTrue(attributedBar->operator[]("Array").IsExternalStorage());
			Assert::IsTrue(attributedBar->operator[]("ChildData").IsExternalStorage());
			Assert::IsFalse(attributedBar->operator[]("Powers").IsExternalStorage());

			Assert::AreEqual(attributedBar->operator[]("this"s).GetPointer(0), reinterpret_cast<RTTI*>(attributedBar));
			Assert::IsTrue(attributedBar->operator[]("Data") == attributedBar->Data());
			Assert::AreEqual(attributedBar->operator[]("ExternalFloat").GetFloat(0), attributedBar->mExternalFloat);
			Assert::AreEqual(attributedBar->operator[]("ExternalVec").GetVector(0), attributedBar->mExternalVec);
			Assert::AreEqual(attributedBar->operator[]("ExternalMat").GetMatrix(0), attributedBar->mExternalMat);
			Assert::AreEqual(attributedBar->operator[]("ExternalString").GetString(0), attributedBar->mExternalString);
			Assert::AreEqual(attributedBar->operator[]("ExternalRTTI").GetPointer(0), attributedBar->mExternalRTTI);
			Assert::AreEqual(attributedBar->operator[]("Array").GetFloat(0), attributedBar->mArray[0]);
			Assert::AreEqual(attributedBar->operator[]("Powers").GetScope(0), attributedBar->Powers());

			delete attributedFoo;
			delete attributedBar;

			Factory<Scope>::Clear();
			Assert::AreEqual(Factory<Scope>::Factories().Size(), 0_z);
		}

		TEST_METHOD(Resize)
		{
			Assert::AreEqual(Factory<Scope>::Factories().Size(), 0_z);
			Assert::AreEqual(Factory<Scope>::Factories().BucketSize(), 41_z);
			{
				AttributedFooFactory fooFactory;
				Assert::AreEqual(Factory<Scope>::Factories().Size(), 1_z);
				Factory<Scope>::Resize(10);
				Assert::AreEqual(Factory<Scope>::Factories().Size(), 1_z);
				Assert::AreEqual(Factory<Scope>::Factories().BucketSize(), 10_z);
				Assert::IsTrue(Factory<Scope>::Find("AttributedFoo") != nullptr);
			}

			Assert::AreEqual(Factory<Scope>::Factories().Size(), 0_z);
			Assert::AreEqual(Factory<Scope>::Factories().BucketSize(), 10_z);
			Factory<Scope>::Resize(41_z);
			Assert::AreEqual(Factory<Scope>::Factories().BucketSize(), 41_z);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState FactoryTests::sStartMemState;
}