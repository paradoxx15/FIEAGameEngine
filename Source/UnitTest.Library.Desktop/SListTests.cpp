#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTests)
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

#pragma region SList Tests
		TEST_METHOD(Constructor)
		{
			SList<Foo> list;
			list;
			Assert::AreEqual(list.Size(), 0_z);

			const SList<Foo> fooList;
			fooList;
			Assert::AreEqual(fooList.Size(), 0_z);
		}

		TEST_METHOD(IsEmpty)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			int data = 10;
			list.PushBack(Foo(data));
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			SList<Foo> list2;
			Assert::AreEqual(list2.Size(), 0_z);
			list2.PushFront(Foo(0));
			Assert::AreEqual(list2.Size(), 1_z);
			list2.PushFront(Foo(1));
			Assert::AreEqual(list2.Size(), 2_z);
			list2.PushFront(Foo(2));
			Assert::AreEqual(list2.Size(), 3_z);
		}

		TEST_METHOD(Front)
		{
			SList<Foo> list;
			const SList<Foo>& constList = list;

			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&constList] { constList.Front(); });

			Foo a(10);
			Foo b(20);

			list.PushFront(a);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, constList.Front());
			list.PushFront(b);
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(b, constList.Front());
 		}

		TEST_METHOD(Back)
		{
			SList<Foo> list;
			const SList<Foo>& constList = list;

			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });
			Assert::ExpectException<std::runtime_error>([&constList] { constList.Back(); });

			Foo a(10);
			Foo b(20);

			list.PushBack(a);
			Assert::AreEqual(a, list.Back());
			Assert::AreEqual(a, constList.Back());
			list.PushBack(b);
			Assert::AreEqual(b, list.Back());
			Assert::AreEqual(b, constList.Back());
		}

		TEST_METHOD(PushFront)
		{
			SList<Foo> list;

			Foo a(10);
			Foo b(20);
			Foo c(30);

			list.PushFront(a);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			Assert::AreEqual(list.Size(), 1_z);
			list.PushFront(b);
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(a, list.Back());
			Assert::AreEqual(list.Size(), 2_z);
			list.PushFront(c);
			Assert::AreEqual(c, list.Front());
			Assert::AreEqual(a, list.Back());
			Assert::AreEqual(list.Size(), 3_z);
		}

		TEST_METHOD(PushBack)
		{
			SList<Foo> list;

			Foo a(10);
			Foo b(20);
			Foo c(30);

			list.PushBack(a);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			Assert::AreEqual(list.Size(), 1_z);
			list.PushBack(b);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(b, list.Back());
			Assert::AreEqual(list.Size(), 2_z);
			list.PushBack(c);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());
			Assert::AreEqual(list.Size(), 3_z);
		}

		TEST_METHOD(PopFront)
		{
			SList<Foo> list;
			list.PopFront();

			Foo a(10);
			Foo b(20);
			Foo c(30);

			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);

			list.PopFront();
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(a, list.Back());
			list.PopFront();
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			list.PopFront();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(PopBack)
		{
			SList<Foo> list;
			list.PopBack();

			Foo a(10);
			Foo b(20);
			Foo c(30);

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			list.PopBack();
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(b, list.Back());
			list.PopBack();
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			list.PopBack();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(CopySemantics)
		{
			SList<Foo> list;
			Foo a(10);
			Foo b(20);
			Foo c(30);

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			// Test copy constructor
			SList<Foo> listCopy(list);
			Assert::AreEqual(a, listCopy.Front());
			Assert::AreEqual(c, listCopy.Back());
			listCopy.PopFront();
			Assert::AreEqual(b, listCopy.Front());

			// Test assignment operator
			SList<Foo> listCopy2;
			listCopy2 = list;
			Assert::AreEqual(a, listCopy2.Front());
			Assert::AreEqual(c, listCopy2.Back());
			listCopy2.PopFront();
			Assert::AreEqual(b, listCopy2.Front());
		}

		TEST_METHOD(Clear)
		{
			SList<Foo> list;
			list.PushFront(Foo(10));
			list.PushFront(Foo(20));
			list.PushFront(Foo(30));

			list.Clear();
			Assert::IsTrue(list.IsEmpty());
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
		}

		TEST_METHOD(MoveSemantics)
		{
			Foo a(10);
			Foo b(20);

			SList<Foo> list;
			list.PushFront(a);
			list.PushFront(b);

			// Test move constructor
			SList<Foo> moveList(std::move(list));
			Assert::AreEqual(b, moveList.Front());
			Assert::AreEqual(a, moveList.Back());
			Assert::AreEqual(moveList.Size(), 2_z);

			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::IsTrue(list.IsEmpty());

			// Test move assignment operator
			SList<Foo> list2;
			list2.PushFront(a);
			list2.PushFront(b);

			SList<Foo> moveList2;
			moveList2 = std::move(list2);
			Assert::AreEqual(b, moveList2.Front());
			Assert::AreEqual(a, moveList2.Back());
			Assert::AreEqual(moveList2.Size(), 2_z);

			Assert::ExpectException<std::runtime_error>([&list2] { list2.Back(); });
			Assert::ExpectException<std::runtime_error>([&list2] { list2.Front(); });
			Assert::IsTrue(list2.IsEmpty());
		}

		TEST_METHOD(IteratorBeginEnd)
		{
			SList<Foo> list;
			const SList<Foo>& constList = list;
			Assert::AreEqual(list.begin(), list.end());
			Assert::AreEqual(constList.begin(), constList.end());
			list.PushFront(Foo(10));
			Assert::AreNotEqual(list.begin(), list.end());
			Assert::AreNotEqual(constList.begin(), constList.end());
		}

		TEST_METHOD(InsertAfter)
		{
			SList<Foo> list;
			Foo a(10);
			Foo b(20);

			auto it = list.InsertAfter(a, list.begin());
			Assert::AreEqual(list.Back(), *it);
			Assert::AreEqual(list.Front(), *it);
			Assert::AreEqual(list.Size(), 1_z);

			list.PushBack(a);

			it = list.InsertAfter(b, it);
			Assert::AreEqual(b, *it);
			Assert::AreEqual(list.Size(), 3_z);

			it = it = list.InsertAfter(b, list.end());
			Assert::AreEqual(list.Back(), *it);
			Assert::AreEqual(list.Size(), 4_z);

			SList<Foo>::Iterator fakeIt;
			Assert::ExpectException<std::runtime_error>([&list, &b, &fakeIt] { list.InsertAfter(b, fakeIt); });
		}

		TEST_METHOD(Find)
		{
			SList<Foo> list;
			Foo a(10);
			Foo b(20);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(a);
			list.PushBack(b);
			const SList<Foo>& constList = list;

			auto it = list.Find(a);
			Assert::AreEqual(*it, a);

			it = list.Find(b);
			Assert::AreEqual(*it, b);

			it = list.Find(Foo(30));
			Assert::AreEqual(it, list.end());

			auto constIt = constList.Find(a);
			Assert::AreEqual(*constIt, a);

			constIt = constList.Find(b);
			Assert::AreEqual(*constIt, b);

			constIt = constList.Find(Foo(30));
			Assert::AreEqual(constIt, constList.end());
		}

		TEST_METHOD(Remove)
		{
			SList<Foo> list;
			list.PushBack(Foo(10));
			list.PushBack(Foo(10));

			Assert::ExpectException<std::runtime_error>([&list] { list.Remove(SList<Foo>::Iterator()); });

			// Check that removing an iterator pointing to the last node of the list works
			auto it = list.InsertAfter(Foo(20), list.end());
			Assert::IsTrue(list.Remove(it));
			Assert::AreEqual(list.Size(), 2_z);

			// Check that removing something not in the list returns false
			it = list.InsertAfter(Foo(20), list.end());
			list.PopBack();
			Assert::IsFalse(list.Remove(Foo(20)));
			Assert::AreEqual(list.Size(), 2_z);

			// Check that removing something in the list with remove by value returns true
			list.PushBack(Foo(30));
			Assert::IsTrue(list.Remove(Foo(30)));
			Assert::AreEqual(list.Size(), 2_z);

			// Check that removing the beginning of the list works
			Assert::IsTrue(list.Remove(list.begin()));
			Assert::AreEqual(list.Size(), 1_z);

			// Check that removing an iterator in the middle of the list works
			it = list.InsertAfter(Foo(20), list.end());
			list.PushBack(Foo(10));
			Assert::IsTrue(list.Remove(it));
			Assert::AreEqual(list.Size(), 2_z);

			// Check that trying to remove from an empty list returns false
			list.Clear();
			Assert::IsFalse(list.Remove(list.begin()));
		}
#pragma endregion

#pragma region Iterator Tests
		TEST_METHOD(Iterator)
		{
			SList<Foo> list;
			Foo a(10);
			Foo b(20);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(a);
			list.PushBack(b);

			for (auto it = list.begin(); it != list.end(); ++it)
			{
				Assert::AreEqual(*it, a);
				Assert::AreEqual(*(it++), a);
				Assert::AreEqual(*it, b);
			}
		}

		TEST_METHOD(IteratorExceptions)
		{
			SList<Foo>::Iterator it;
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			SList<Foo> list;
			Assert::ExpectException<std::runtime_error>([&list] { *list.begin(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.begin()++; });
			Assert::ExpectException<std::runtime_error>([&list] { ++list.begin(); });
		}

		TEST_METHOD(ConstIterator)
		{
			SList<Foo> list;
			Foo a(10);
			Foo b(20);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(a);
			list.PushBack(b);
			const SList<Foo>& constList = list;

			for (auto it = constList.begin(); it != constList.end(); ++it)
			{
				Assert::AreEqual(*it, a);
				Assert::AreEqual(*(it++), a);
				Assert::AreEqual(*it, b);
			}

			SList<Foo>::ConstIterator it(list.begin());
			Assert::AreEqual(*it, *list.begin());

			// Test that the == operator returns false properly
			Assert::AreNotEqual(constList.begin(), SList<Foo>::ConstIterator());
		}

		TEST_METHOD(ConstIteratorExceptions)
		{
			SList<Foo>::ConstIterator it;
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			const SList<Foo> list;
			Assert::ExpectException<std::runtime_error>([&list] { *list.begin(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.begin()++; });
			Assert::ExpectException<std::runtime_error>([&list] { ++list.begin(); });
		}

		

#pragma endregion

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTests::sStartMemState;
}