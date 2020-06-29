#include "pch.h"
#include "CppUnitTest.h"
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
	TEST_CLASS(TypeManagerTests)
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

		TEST_METHOD(RegisterTypes)
		{
			TypeManager::RegisterType(AttributedFoo::TypeIdClass(), Attributed::TypeIdClass(), AttributedFoo::GetSignatures());
			TypeManager::RegisterType(AttributedBar::TypeIdClass(), AttributedFoo::TypeIdClass(), AttributedBar::GetSignatures());

			{
				auto prescribedSignatures = TypeManager::GetPrescribedSignatures(AttributedFoo::TypeIdClass());
				auto classSignatures = AttributedFoo::GetSignatures();
				Assert::AreEqual(prescribedSignatures.Size(), classSignatures.Size());
				for (size_t i = 0; i < prescribedSignatures.Size(); ++i)
				{
					Assert::AreEqual(prescribedSignatures.At(i).Name, classSignatures.At(i).Name);
				}
			}

			auto prescribedSignatures = TypeManager::GetPrescribedSignatures(AttributedBar::TypeIdClass());
			Assert::AreEqual(prescribedSignatures.Size(), AttributedBar::GetSignatures().Size() + AttributedFoo::GetSignatures().Size());

			TypeManager::UnregisterType(AttributedFoo::TypeIdClass());
			TypeManager::UnregisterType(AttributedBar::TypeIdClass());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState TypeManagerTests::sStartMemState;
}