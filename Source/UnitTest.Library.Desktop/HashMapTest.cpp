#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "HashMap.h"
#include "ToStringSpecializations.h"

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
	TEST_CLASS(HashMapTests)
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
			std::pair<const Foo, Foo> a(Foo(0), Foo(10));
			std::pair<const Foo, Foo> b(Foo(1), Foo(20));

			HashMap<Foo, Foo> map(4);
			map.Insert(a);
			map.Insert(b);

			for (auto it = map.begin(); it != map.end(); ++it)
			{
				Assert::AreEqual(*it, a);
				Assert::AreEqual(*(it++), a);
				Assert::AreEqual(*it, b);
			}
		}

		TEST_METHOD(IteratorExceptions)
		{
			HashMap<Foo, Foo>::Iterator it;
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			HashMap<Foo, Foo> map;
			Assert::ExpectException<std::runtime_error>([&map] { *map.begin(); });
			Assert::ExpectException<std::runtime_error>([&map] { map.begin()++; });
			Assert::ExpectException<std::runtime_error>([&map] { ++map.begin(); });
		}

		TEST_METHOD(ConstIterator)
		{
			std::pair<const Foo, Foo> a(Foo(0), Foo(10));
			std::pair<const Foo, Foo> b(Foo(1), Foo(20));

			HashMap<Foo, Foo> m(4);
			m.Insert(a);
			m.Insert(b);
			const HashMap<Foo, Foo>& map = m;

			for (auto it = map.begin(); it != map.end(); ++it)
			{
				Assert::AreEqual(*it, a);
				Assert::AreEqual(*(it++), a);
				Assert::AreEqual(*it, b);
			}
		}

		TEST_METHOD(ConstIteratorExceptions)
		{
			HashMap<Foo, Foo>::ConstIterator it;
			Assert::ExpectException<std::runtime_error>([&it] { *it; });

			const HashMap<Foo, Foo> map;
			Assert::ExpectException<std::runtime_error>([&map] { *map.begin(); });
			Assert::ExpectException<std::runtime_error>([&map] { map.begin()++; });
			Assert::ExpectException<std::runtime_error>([&map] { ++map.begin(); });

		}

		TEST_METHOD(IteratorBeginEnd)
		{
			HashMap<Foo, Foo> map(5);
			std::pair<const Foo, Foo> a(Foo(10), Foo(1));

			const HashMap<Foo, Foo>& constMap = map;
			Assert::AreEqual(map.begin(), map.end());
			Assert::AreEqual(constMap.begin(), constMap.end());
			map.Insert(a);
			Assert::AreEqual(*map.begin(), a);
			Assert::AreNotEqual(map.begin(), map.end());
			Assert::AreNotEqual(constMap.begin(), constMap.end());
		}
#pragma endregion

		TEST_METHOD(Constructor)
		{
			HashMap<Foo, Foo> map;
			Assert::AreEqual(map.Size(), 0_z);
			Assert::AreEqual(map.begin(), map.end());

			HashMap<Foo, Foo> map2(5);
			Assert::AreEqual(map2.Size(), 0_z);
			Assert::AreEqual(map2.begin(), map2.end());
		}

		TEST_METHOD(Insert)
		{
			std::pair<const Foo, Foo> a(Foo(0), Foo(10));
			std::pair<const Foo, Foo> b(Foo(1), Foo(20));
			std::pair<const Foo, Foo> c(Foo(0), Foo(30));

			HashMap<Foo, Foo> map(2);
			auto insertPair = map.Insert(b);
			Assert::AreEqual(*map.begin(), b);
			Assert::AreEqual(map.begin(), insertPair.first);
			Assert::IsTrue(insertPair.second);
			insertPair = map.Insert(a);
			Assert::AreEqual(*map.begin(), a);
			Assert::AreEqual(map.begin(), insertPair.first);
			Assert::IsTrue(insertPair.second);
			insertPair = map.Insert(c);
			Assert::AreEqual(*insertPair.first, a);
			Assert::IsFalse(insertPair.second);
		}

		TEST_METHOD(Find)
		{
			std::pair<const Foo, Foo> a(Foo(0), Foo(10));
			std::pair<const Foo, Foo> b(Foo(1), Foo(20));
			std::pair<const Foo, Foo> c(Foo(2), Foo(30));

			HashMap<Foo, Foo> map(3);
			HashMap<Foo, Foo>& constMap = map;
			map.Insert(a);
			map.Insert(b);
			map.Insert(c);

			auto it = map.Find(Foo(0));
			Assert::AreEqual(*it, a);

			it = map.Find(Foo(1));
			Assert::AreEqual(*it, b);

			it = map.Find(Foo(10));
			Assert::AreEqual(it, map.end());

			auto constIt = constMap.Find(Foo(0));
			Assert::AreEqual(*constIt, a);

			constIt = constMap.Find(Foo(1));
			Assert::AreEqual(*constIt, b);

			constIt = constMap.Find(Foo(10));
			Assert::AreEqual(constIt, constMap.end());
		}

		TEST_METHOD(At)
		{
			std::pair a(Foo(0), Foo(10));
			std::pair b(Foo(1), Foo(20));
			std::pair c(Foo(2), Foo(30));

			HashMap<Foo, Foo> map(3);
			HashMap<Foo, Foo>& constMap = map;
			map.Insert(a);
			map.Insert(b);
			map.Insert(c);

			{
				Foo& test = map.At(Foo(0));
				Assert::AreEqual(test, Foo(10));
			}

			{
				Foo& test = map.At(Foo(1));
				Assert::AreEqual(test, Foo(20));
				test = Foo(40);
			}

			{
				Foo& test = map.At(Foo(1));
				Assert::AreEqual(test, Foo(40));
			}

			Assert::ExpectException<std::runtime_error>([&map] { map.At(Foo(10)); });

			{
				const Foo& test = constMap.At(Foo(0));
				Assert::AreEqual(test, Foo(10));
			}

			{
				const Foo& test = constMap.At(Foo(1));
				Assert::AreEqual(test, Foo(40));
			}

			Assert::ExpectException<std::runtime_error>([&constMap] { constMap.At(Foo(10)); });
		}

		TEST_METHOD(ContainsKey)
		{
			std::pair a(Foo(0), Foo(10));
			std::pair b(Foo(1), Foo(20));

			HashMap<Foo, Foo> map(2);
			map.Insert(a);
			map.Insert(b);

			Assert::IsTrue(map.ContainsKey(Foo(0)));
			Assert::IsTrue(map.ContainsKey(Foo(1)));
			Assert::IsFalse(map.ContainsKey(Foo(2)));
		}

		TEST_METHOD(Remove)
		{
			std::pair a(Foo(0), Foo(10));
			std::pair b(Foo(1), Foo(20));

			HashMap<Foo, Foo> map(2);
			map.Insert(a);
			map.Insert(b);

			Assert::IsTrue(map.Remove(Foo(0)));
			Assert::AreNotEqual(map.begin(), map.end());
			Assert::AreEqual(1_z, map.Size());
			Assert::IsTrue(map.Remove(Foo(1)));
			Assert::AreEqual(map.begin(), map.end());
			Assert::AreEqual(0_z, map.Size());
			Assert::IsFalse(map.Remove(Foo(0)));
		}

		TEST_METHOD(Clear)
		{
			std::pair a(Foo(0), Foo(10));
			std::pair b(Foo(1), Foo(20));

			HashMap<Foo, Foo> map(2);
			map.Insert(a);
			map.Insert(b);

			Assert::AreNotEqual(map.begin(), map.end());
			map.Clear();
			Assert::AreEqual(map.begin(), map.end());
		}

		TEST_METHOD(CopySemantics)
		{
			std::pair a(Foo(0), Foo(10));
			std::pair b(Foo(1), Foo(20));

			HashMap<Foo, Foo> map(2);
			map.Insert(a);
			map.Insert(b);

			{
				// Test copy constructor
				HashMap<Foo, Foo> mapCopy(map);
				Assert::AreEqual(mapCopy.Size(), map.Size());
				for (HashMap<Foo, Foo>::Iterator it = map.begin(), it2 = mapCopy.begin(); it != map.end() && it2 != mapCopy.end(); ++it, ++it2)
				{
					Assert::AreEqual(*it, *it2);
				}
				Assert::AreNotSame(map, mapCopy);
			}

			{
				// Test assignment operator
				HashMap<Foo, Foo> mapCopy;
				mapCopy = map;
				Assert::AreEqual(mapCopy.Size(), map.Size());
				for (HashMap<Foo, Foo>::Iterator it = map.begin(), it2 = mapCopy.begin(); it != map.end() && it2 != mapCopy.end(); ++it, ++it2)
				{
					Assert::AreEqual(*it, *it2);
				}
				Assert::AreNotSame(map, mapCopy);
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			std::pair<const Foo, Foo> a(Foo(0), Foo(10));
			std::pair<const Foo, Foo> b(Foo(1), Foo(20));

			{
				// Test copy constructor
				HashMap<Foo, Foo> map(2);
				map.Insert(a);
				map.Insert(b);
				HashMap<Foo, Foo> mapCopy(std::move(map));
				Assert::AreEqual(map.Size(), 0_z);
				Assert::AreEqual(mapCopy.Size(), 2_z);
				auto it = mapCopy.begin();
				Assert::AreEqual(a, *it);
				Assert::AreEqual(b, *(++it));
			}

			{
				// Test move assignment operator
				HashMap<Foo, Foo> map(2);
				map.Insert(a);
				map.Insert(b);
				HashMap<Foo, Foo> mapCopy;
				mapCopy = std::move(map);
				Assert::AreEqual(map.Size(), 0_z);
				Assert::AreEqual(mapCopy.Size(), 2_z);
				auto it = mapCopy.begin();
				Assert::AreEqual(a, *it);
				Assert::AreEqual(b, *(++it));
			}
		}

		TEST_METHOD(IndexOperator)
		{
			std::pair a(Foo(0), Foo(10));
			std::pair b(Foo(1), Foo(20));

			HashMap<Foo, Foo> map(3);
			map.Insert(a);
			map.Insert(b);

			Foo test = map[Foo(0)];
			Assert::AreEqual(test, Foo(10));
			test = map[Foo(2)];
			Assert::IsTrue(map.ContainsKey(Foo(2)));

			const HashMap<Foo, Foo>& constMap = map;
			Foo test2 = constMap[Foo(2)];
			Assert::AreEqual(test2, Foo());
			Assert::ExpectException<std::runtime_error>([&constMap] { constMap[Foo(3)]; });
		}

		TEST_METHOD(Resize)
		{
			std::pair<const Foo, Foo> a(Foo(0), Foo(10));
			std::pair<const Foo, Foo> b(Foo(1), Foo(20));
			std::pair<const Foo, Foo> c(Foo(6), Foo(60));

			HashMap<Foo, Foo> map(2);
			map.Insert(a);
			map.Insert(b);
			map.Insert(c);

			{
				auto it = map.begin();
				Assert::AreEqual(*it, a);
				Assert::AreEqual(*(++it), c);
				Assert::AreEqual(*(++it), b);
			}

			{
				map.Resize(4);
				Assert::AreEqual(3_z, map.Size());
				auto it = map.begin();
				Assert::AreEqual(*it, a);
				Assert::AreEqual(*(++it), b);
				Assert::AreEqual(*(++it), c);
			}

			{
				map.Resize(2);
				Assert::AreEqual(3_z, map.Size());
				auto it = map.begin();
				Assert::AreEqual(*it, a);
				Assert::AreEqual(*(++it), c);
				Assert::AreEqual(*(++it), b);
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState HashMapTests::sStartMemState;
}