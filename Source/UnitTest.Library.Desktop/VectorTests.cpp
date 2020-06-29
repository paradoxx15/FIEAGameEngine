#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Vector.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTests)
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

#pragma region Iterator Tests
		TEST_METHOD(Iterator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector = {a, b, a, b};

			for (auto it = vector.begin(); it != vector.end(); ++it)
			{
				Assert::AreEqual(*it, a);
				Assert::AreEqual(*(it++), a);
				Assert::AreEqual(*it, b);
			}

			auto it = vector.end();
			--it;
			Assert::AreEqual(*it, b);
			Assert::AreEqual(*(it--), b);
			Assert::AreEqual(*it, a);
		}

		TEST_METHOD(IteratorExceptions)
		{
			Vector<Foo>::Iterator it;
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			Vector<Foo> vector;
			Assert::ExpectException<std::runtime_error>([&vector] { *vector.begin(); });
			Assert::ExpectException<std::runtime_error>([&vector] { vector.begin()++; });
			Assert::ExpectException<std::runtime_error>([&vector] { ++vector.begin(); });
			Assert::ExpectException<std::runtime_error>([&vector] { --vector.begin(); });
			Assert::ExpectException<std::runtime_error>([&vector] { vector.begin()--; });
		}

		TEST_METHOD(ConstIterator)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> v = { a, b, a, b };
			const Vector<Foo>& vector = v;

			for (auto it = vector.begin(); it != vector.end(); ++it)
			{
				Assert::AreEqual(*it, a);
				Assert::AreEqual(*(it++), a);
				Assert::AreEqual(*it, b);
			}

			auto it = vector.end();
			--it;
			Assert::AreEqual(*it, b);
			Assert::AreEqual(*(it--), b);
			Assert::AreEqual(*it, a);
		}

		TEST_METHOD(ConstIteratorExceptions)
		{
			Vector<Foo>::ConstIterator it;
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			const Vector<Foo> vector;
			Assert::ExpectException<std::runtime_error>([&vector] { *vector.begin(); });
			Assert::ExpectException<std::runtime_error>([&vector] { vector.begin()++; });
			Assert::ExpectException<std::runtime_error>([&vector] { ++vector.begin(); });
			Assert::ExpectException<std::runtime_error>([&vector] { --vector.begin(); });
			Assert::ExpectException<std::runtime_error>([&vector] { vector.begin()--; });
		}

		TEST_METHOD(IteratorBeginEnd)
		{
			Vector<Foo> vector;
			const Vector<Foo>& constVector = vector;
			Assert::IsTrue(vector.begin() == vector.end());
			Assert::IsTrue(constVector.begin() == constVector.end());
			vector.PushBack(Foo(10));
			Assert::IsTrue(vector.begin() != vector.end());
			Assert::IsTrue(constVector.begin() != constVector.end());
		}

#pragma endregion

#pragma region Vector Tests
		TEST_METHOD(Constructor)
		{
			Vector<Foo> vector;
			vector;
			Assert::AreEqual(vector.Size(), 0_z);
			Assert::AreEqual(vector.Capacity(), 0_z);

			const Vector<Foo> fooVector;
			fooVector;
			Assert::AreEqual(fooVector.Size(), 0_z);
			Assert::AreEqual(fooVector.Capacity(), 0_z);
		}

		TEST_METHOD(IsEmpty)
		{
			Vector<Foo> vector;
			Assert::IsTrue(vector.IsEmpty());

			vector.PushBack(Foo(10));
			Assert::IsFalse(vector.IsEmpty());
		}

		TEST_METHOD(PushBack)
		{
			Vector<Foo> vector;

			Foo a(10);
			Foo b(20);
			Foo c(30);

			vector.Reserve(2);

			vector.PushBack(a);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(a, vector.Back());
			Assert::AreEqual(vector.Size(), 1_z);
			Assert::AreEqual(vector.Capacity(), 2_z);
			vector.PushBack(b);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(b, vector.Back());
			Assert::AreEqual(vector.Size(), 2_z);
			Assert::AreEqual(vector.Capacity(), 2_z);
			vector.PushBack(c);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.Back());
			Assert::AreEqual(vector.Size(), 3_z);
			Assert::AreEqual(vector.Capacity(), 3_z);
		}

		TEST_METHOD(Front)
		{
			Vector<Foo> vector;
			const Vector<Foo>& constVector = vector;

			Assert::ExpectException<std::runtime_error>([&vector] { vector.Front(); });
			Assert::ExpectException<std::runtime_error>([&constVector] { constVector.Front(); });

			Foo a(10);
			Foo b(20);

			vector.PushBack(a);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(a, constVector.Front());
			vector.PushBack(b);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(a, constVector.Front());
		}

		TEST_METHOD(Back)
		{
			Vector<Foo> vector;
			const Vector<Foo>& constvector = vector;

			Assert::ExpectException<std::runtime_error>([&vector] { vector.Back(); });
			Assert::ExpectException<std::runtime_error>([&constvector] { constvector.Back(); });

			Foo a(10);
			Foo b(20);

			vector.PushBack(a);
			Assert::AreEqual(a, vector.Back());
			Assert::AreEqual(a, constvector.Back());
			vector.PushBack(b);
			Assert::AreEqual(b, vector.Back());
			Assert::AreEqual(b, constvector.Back());
		}

		TEST_METHOD(PopBack)
		{
			Vector<Foo> vector;
			vector.PopBack();
			Assert::AreEqual(vector.Size(), 0_z);
			Assert::AreEqual(vector.Capacity(), 0_z);

			vector.Reserve(3);
			vector.PopBack();
			Assert::AreEqual(vector.Size(), 0_z);
			Assert::AreEqual(vector.Capacity(), 3_z);

			Foo a(10);
			Foo b(20);
			Foo c(30);

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			vector.PopBack();
			Assert::AreEqual(vector.Size(), 2_z);
			Assert::AreEqual(vector.Capacity(), 3_z);
			Assert::AreEqual(a, vector.At(0));
			Assert::AreEqual(b, vector.At(vector.Size() - 1));
			vector.PopBack();
			Assert::AreEqual(vector.Size(), 1_z);
			Assert::AreEqual(vector.Capacity(), 3_z);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(a, vector.Back());
			vector.PopBack();
			Assert::IsTrue(vector.IsEmpty());
			Assert::AreEqual(vector.Capacity(), 3_z);
		}

		TEST_METHOD(CopySemantics)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);

			Vector<Foo> vector = { a, b, c };

			// Test copy constructor
			Vector<Foo> vectorCopy(vector);
			Assert::AreEqual(vectorCopy.Size(), vector.Size());
			Assert::AreEqual(vectorCopy.Capacity(), vector.Capacity());
			Assert::AreEqual(a, vectorCopy.Front());
			Assert::AreEqual(c, vectorCopy.Back());
			vectorCopy.PopBack();
			Assert::AreEqual(vectorCopy[vectorCopy.Size() - 1], b);

			// Test assignment operator
			Vector<Foo> vectorCopy2;
			vectorCopy2 = vector;
			Assert::AreEqual(vectorCopy2.Size(), vector.Size());
			Assert::AreEqual(vectorCopy2.Capacity(), vector.Capacity());
			Assert::AreEqual(a, vectorCopy2.Front());
			Assert::AreEqual(c, vectorCopy2.Back());
			vectorCopy2.PopBack();
			Assert::AreEqual(vectorCopy2[vectorCopy2.Size() - 1], b);
		}

		TEST_METHOD(Clear)
		{
			Vector<Foo> vector = { Foo(10), Foo(20), Foo(30) };

			vector.Clear();
			Assert::IsTrue(vector.IsEmpty());
			Assert::AreEqual(vector.Capacity(), 3_z);
			Assert::ExpectException<std::runtime_error>([&vector] { vector.Back(); });
			Assert::ExpectException<std::runtime_error>([&vector] { vector.Front(); });
		}

		TEST_METHOD(MoveSemantics)
		{
			Foo a(10);
			Foo b(20);

			Vector<Foo> vector = { a, b };

			// Test move constructor
			Vector<Foo> moveVector(std::move(vector));
			Assert::AreEqual(a, moveVector.Front());
			Assert::AreEqual(b, moveVector.Back());
			Assert::AreEqual(moveVector.Size(), 2_z);
			Assert::AreEqual(moveVector.Capacity(), 2_z);

			Assert::ExpectException<std::runtime_error>([&vector] { vector.Back(); });
			Assert::ExpectException<std::runtime_error>([&vector] { vector.Front(); });
			Assert::IsTrue(vector.IsEmpty());

			// Test move assignment operator
			Vector<Foo> vector2 = { a, b };

			Vector<Foo> moveVector2;
			moveVector2 = std::move(vector2);
			Assert::AreEqual(a, moveVector2.Front());
			Assert::AreEqual(b, moveVector2.Back());
			Assert::AreEqual(moveVector2.Size(), 2_z);
			Assert::AreEqual(moveVector2.Capacity(), 2_z);

			Assert::ExpectException<std::runtime_error>([&vector2] { vector2.Back(); });
			Assert::ExpectException<std::runtime_error>([&vector2] { vector2.Front(); });
			Assert::IsTrue(vector2.IsEmpty());
		}

		TEST_METHOD(Find)
		{
			Foo a(10);
			Foo b(20);
			Vector<Foo> vector = { a, b, a, b };
			const Vector<Foo>& constvector = vector;

			auto it = vector.Find(a);
			Assert::AreEqual(*it, a);

			it = vector.Find(b);
			Assert::AreEqual(*it, b);

			it = vector.Find(Foo(30));
			Assert::IsTrue(it == vector.end());

			auto constIt = constvector.Find(a);
			Assert::AreEqual(*constIt, a);

			constIt = constvector.Find(b);
			Assert::AreEqual(*constIt, b);

			constIt = constvector.Find(Foo(30));
			Assert::IsTrue(constIt == vector.end());

		}

		TEST_METHOD(Remove)
		{
			Vector<Foo> vector{ Foo(10), Foo(10) };

			Assert::ExpectException<std::runtime_error>([&vector] { vector.Remove(Vector<Foo>::Iterator()); });

			// Check that removing an iterator pointing to the last node of the vector works
			auto it = vector.PushBack(Foo(20));
			Assert::IsTrue(vector.Remove(it));
			Assert::AreEqual(vector.Size(), 2_z);
			Assert::AreEqual(vector.Capacity(), 3_z);

			// Check that removing something not in the vector returns false
			it = vector.PushBack(Foo(20));
			vector.PopBack();
			Assert::IsFalse(vector.Remove(Foo(20)));
			Assert::AreEqual(vector.Size(), 2_z);
			Assert::AreEqual(vector.Capacity(), 3_z);

			// Check that removing something in the vector with remove by value returns true
			vector.PushBack(Foo(30));
			Assert::IsTrue(vector.Remove(Foo(30)));
			Assert::AreEqual(vector.Size(), 2_z);
			Assert::AreEqual(vector.Capacity(), 3_z);

			// Check that removing the beginning of the vector works
			Assert::IsTrue(vector.Remove(vector.begin()));
			Assert::AreEqual(vector.Size(), 1_z);
			Assert::AreEqual(vector.Capacity(), 3_z);

			// Check that removing an iterator in the middle of the vector works
			it = vector.PushBack(Foo(20));
			vector.PushBack(Foo(10));
			Assert::IsTrue(vector.Remove(it));
			Assert::AreEqual(vector.Size(), 2_z);
			Assert::AreEqual(vector.Capacity(), 3_z);

			// Check that trying to remove from an empty vector returns false
			vector.Clear();
			Assert::IsFalse(vector.Remove(vector.begin()));
			Assert::AreEqual(vector.Size(), 0_z);
			Assert::AreEqual(vector.Capacity(), 3_z);
		}

		TEST_METHOD(RemoveRange)
		{
			{
				Vector<Foo> vector{ Foo(10), Foo(20), Foo(30), Foo(40) };

				Assert::ExpectException<std::runtime_error>([&vector] { vector.Remove(Vector<Foo>::Iterator(), Vector<Foo>::Iterator()); });

				Assert::IsFalse(vector.Remove(vector.end(), vector.end()));

				auto startIt = vector.Find(Foo(10));
				auto endIt = vector.Find(Foo(30));

				Assert::AreEqual(4_z, vector.Size());
				Assert::AreNotEqual(vector.Find(Foo(10)), vector.end());
				Assert::AreNotEqual(vector.Find(Foo(20)), vector.end());
				Assert::IsTrue(vector.Remove(startIt, endIt));
				Assert::AreEqual(2_z, vector.Size());
				Assert::AreEqual(vector.Find(Foo(10)), vector.end());
				Assert::AreEqual(vector.Find(Foo(20)), vector.end());
			}

			{
				Vector<Foo> vector{ Foo(10), Foo(20), Foo(30), Foo(40) };

				auto startIt = vector.Find(Foo(20));

				Assert::AreEqual(4_z, vector.Size());
				Assert::AreNotEqual(vector.Find(Foo(20)), vector.end());
				Assert::AreNotEqual(vector.Find(Foo(30)), vector.end());
				Assert::AreNotEqual(vector.Find(Foo(40)), vector.end());
				Assert::IsTrue(vector.Remove(startIt, vector.end()));
				Assert::AreEqual(1_z, vector.Size());
				Assert::AreEqual(vector.Find(Foo(20)), vector.end());
				Assert::AreEqual(vector.Find(Foo(30)), vector.end());
				Assert::AreEqual(vector.Find(Foo(40)), vector.end());
			}
		}

		TEST_METHOD(Resize)
		{
			Vector<Foo> vector = { Foo(10), Foo(20), Foo(30) };
			vector.Resize(5);
			Assert::AreEqual(vector.Size(), 5_z);
			Assert::AreEqual(vector.Capacity(), 5_z);
			vector.Resize(1);
			Assert::AreEqual(vector.Size(), 1_z);
			Assert::AreEqual(vector.Capacity(), 5_z);
		}

		TEST_METHOD(AccessExceptions)
		{
			Vector<Foo> vector;
			const Vector<Foo>& constVector = vector;

			Assert::ExpectException<std::runtime_error>([&vector] { vector[2]; });
			Assert::ExpectException<std::runtime_error>([&constVector] { constVector[2]; });
			Assert::ExpectException<std::runtime_error>([&vector] { vector.At(2); });
			Assert::ExpectException<std::runtime_error>([&constVector] { constVector.At(2); });

			vector.PushBack(Foo(10));
			Assert::ExpectException<std::runtime_error>([&vector] { vector[2]; });
			Assert::ExpectException<std::runtime_error>([&constVector] { constVector[2]; });
			Assert::ExpectException<std::runtime_error>([&vector] { vector.At(2); });
			Assert::ExpectException<std::runtime_error>([&constVector] { constVector.At(2); });

			Assert::AreEqual(Foo(10), constVector.At(0));
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTests::sStartMemState;
}