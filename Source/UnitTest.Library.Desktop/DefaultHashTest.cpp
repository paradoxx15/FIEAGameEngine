#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;

namespace Library
{
	inline size_t DefaultHash<Foo>::operator()(const Foo& key) const
	{
		return key.Data();
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DefaultHashTest)
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

		TEST_METHOD(Integer)
		{
			DefaultHash<int> hash;

			int a = 5, b = 5, c = 10;

			Assert::AreEqual(hash(a), hash(a));
			Assert::AreEqual(hash(a), hash(b));
			Assert::AreNotEqual(hash(b), hash(c));
		}

		TEST_METHOD(Double)
		{
			DefaultHash<double> hash;

			double a = 5.2, b = 5.2, c = 10.56;

			Assert::AreEqual(hash(a), hash(a));
			Assert::AreEqual(hash(a), hash(b));
			Assert::AreNotEqual(hash(b), hash(c));
		}

		TEST_METHOD(Float)
		{
			DefaultHash<float> hash;

			float a = 5.2f, b = 5.2f, c = 10.56f;

			Assert::AreEqual(hash(a), hash(a));
			Assert::AreEqual(hash(a), hash(b));
			Assert::AreNotEqual(hash(b), hash(c));
		}

		TEST_METHOD(String)
		{
			{
				DefaultHash<std::string> hash;
				std::string a("hello"), b("hello"), c("goodbye");

				Assert::AreEqual(hash(a), hash(a));
				Assert::AreEqual(hash(a), hash(b));
				Assert::AreNotEqual(hash(b), hash(c));
			}

			{
				DefaultHash<const std::string> hash;
				const std::string a("hello"), b("hello"), c("goodbye");

				Assert::AreEqual(hash(a), hash(a));
				Assert::AreEqual(hash(a), hash(b));
				Assert::AreNotEqual(hash(b), hash(c));
			}
		}

		TEST_METHOD(WString)
		{

			{
				DefaultHash<std::wstring> hash;

				std::wstring a = L"Hello", b(a), c = L"Goodbye";

				Assert::AreEqual(hash(a), hash(a));
				Assert::AreEqual(hash(a), hash(b));
				Assert::AreNotEqual(hash(b), hash(c));
			}

			{
				DefaultHash<const std::wstring> hash;

				const std::wstring a = L"Hello", b(a), c = L"Goodbye";

				Assert::AreEqual(hash(a), hash(a));
				Assert::AreEqual(hash(a), hash(b));
				Assert::AreNotEqual(hash(b), hash(c));
			}
		}

		TEST_METHOD(Char)
		{
			{
				DefaultHash<char*> hash;

				char* a = "Hello";
				char* c = "Goodbye";
				char b[6];
				strcpy_s(b, a);

				Assert::AreEqual(hash(a), hash(a));
				Assert::AreEqual(hash(a), hash(b));
				Assert::AreNotEqual(hash(b), hash(c));
			}

			{
				DefaultHash<char* const> hash;

				char* const a = "Hello";
				char* const c = "Goodbye";
				char d[6];
				strcpy_s(d, a);
				char* const b = d;

				Assert::AreEqual(hash(a), hash(a));
				Assert::AreEqual(hash(a), hash(b));
				Assert::AreNotEqual(hash(b), hash(c));
			}

			{
				DefaultHash<const char*> hash;

				const char* a = "Hello";
				const char* c = "Goodbye";
				char d[6];
				strcpy_s(d, a);
				const char* b = d;

				Assert::AreEqual(hash(a), hash(a));
				Assert::AreEqual(hash(a), hash(b));
				Assert::AreNotEqual(hash(b), hash(c));
			}

			{
				DefaultHash<const char* const> hash;

				const char* const a = "Hello";
				const char* const c = "Goodbye";
				char d[6];
				strcpy_s(d, a);
				const char* const b = d;

				Assert::AreEqual(hash(a), hash(a));
				Assert::AreEqual(hash(a), hash(b));
				Assert::AreNotEqual(hash(b), hash(c));
			}
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DefaultHashTest::sStartMemState;
}