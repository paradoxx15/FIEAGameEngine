#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Datum.h"
#include "ToStringSpecializations.h"
#include <algorithm>
#include <gsl/gsl>

#pragma warning( push )
#pragma warning( disable : 4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning( pop )

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;

namespace Library
{
	struct ReserveFunctor
	{
		const size_t operator()(const size_t, const size_t capacity)
		{
			return static_cast<size_t>(capacity * 2);
		}
	};

#pragma region GetHelper

	template<typename T>
	T& GetHelper(Datum&, size_t) { return Datum.GetInt(index); };

	template<>
	int& GetHelper<int>(Datum& datum, size_t index) { return datum.GetInt(index); }

	template<>
	float& GetHelper<float>(Datum& datum, size_t index) { return datum.GetFloat(index); }

	template<>
	glm::vec4& GetHelper<glm::vec4>(Datum& datum, size_t index) { return datum.GetVector(index); }

	template<>
	glm::mat4& GetHelper<glm::mat4>(Datum& datum, size_t index) { return datum.GetMatrix(index); }

	template<>
	std::string& GetHelper<std::string>(Datum& datum, size_t index) { return datum.GetString(index); }

	template<>
	Datum::RTTIPointer& GetHelper<Datum::RTTIPointer>(Datum& datum, size_t index) { return datum.GetPointer(index); }
	
	template<typename T>
	const T& GetHelper(const Datum&, size_t) { return Datum.GetInt(index); };

	template<>
	const int& GetHelper<int>(const Datum& datum, size_t index) { return datum.GetInt(index); }

	template<>
	const float& GetHelper<float>(const Datum& datum, size_t index) { return datum.GetFloat(index); }

	template<>
	const glm::vec4& GetHelper<glm::vec4>(const Datum& datum, size_t index) { return datum.GetVector(index); }

	template<>
	const glm::mat4& GetHelper<glm::mat4>(const Datum& datum, size_t index) { return datum.GetMatrix(index); }

	template<>
	const std::string& GetHelper<std::string>(const Datum& datum, size_t index) { return datum.GetString(index); }

	template<>
	const Datum::RTTIPointer& GetHelper<Datum::RTTIPointer>(const Datum& datum, size_t index) { return datum.GetPointer(index); }

#pragma endregion

	template<typename T>
	void TestScalarConstructor(T data)
	{
		Datum datum(data);
		Assert::AreEqual(datum.Size(), 1_z);
		Assert::AreEqual(datum.Capacity(), 1_z);
		Assert::IsTrue(datum.Type() == Datum::TypeOf<T>());
		Assert::IsFalse(datum.IsExternalStorage());
	}

	template<typename T>
	void TestInitializerListConstructor(std::initializer_list<T> list)
	{
		Datum datum(list);
		Assert::AreEqual(datum.Size(), list.size());
		Assert::AreEqual(datum.Capacity(), list.size());
		Assert::IsTrue(datum.Type() == Datum::TypeOf<T>());
		Assert::IsFalse(datum.IsExternalStorage());
	}

	template<typename T>
	void TestScalarAssignment(T data, T data2)
	{
		Datum datum(data);
		datum = data;
		Assert::IsTrue(datum == data);
		Assert::AreEqual(datum.Size(), 1_z);
		Assert::AreEqual(datum.Capacity(), 1_z);
		Assert::IsTrue(datum.Type() == Datum::TypeOf<T>());
		datum = data2;
		Assert::IsTrue(datum == data2);
		Assert::AreEqual(datum.Size(), 1_z);
		Assert::AreEqual(datum.Capacity(), 1_z);
		Assert::IsTrue(datum.Type() == Datum::TypeOf<T>());
	}

	template<typename T>
	void TestInitializerAssignment(std::initializer_list<T> list)
	{
		Datum datum;
		datum = list;
		Assert::AreEqual(datum.Size(), list.size());
		Assert::AreEqual(datum.Capacity(), list.size());
		Assert::IsTrue(datum.Type() == Datum::TypeOf<T>());
		Assert::IsFalse(datum.IsExternalStorage());
	}

	template<typename T>
	void TestPushBackAndGet(std::initializer_list<T> list)
	{
		Datum datum;
		
		Assert::ExpectException<std::runtime_error>([&datum] { GetHelper<T>(datum, 0); });

		size_t count = 0;
		for (const T& value : list)
		{
			datum.PushBack(value);
			Assert::AreEqual(datum.Size(), ++count);
			Assert::AreEqual(GetHelper<T>(datum, datum.Size() - 1), value);
		}

		if (Datum::TypeOf<T>() != Datum::DatumTypes::Integer)
		{
			Assert::ExpectException<std::runtime_error>([&datum] { datum.PushBack(5); });
		}

		const Datum& constDatum = datum;
		Assert::AreEqual(GetHelper<T>(constDatum, 0), *list.begin());
	}

	template<typename T>
	void TestResize(std::initializer_list<T> list)
	{
		Datum datum(list);

		datum.Resize(5);
		Assert::AreEqual(datum.Size(), 5_z);
		Assert::AreEqual(datum.Capacity(), std::max(list.size(), 5_z));
		Assert::AreEqual(GetHelper<T>(datum, 3), T());
		Assert::AreEqual(GetHelper<T>(datum, 4), T());

		datum.Resize(1);
		Assert::AreEqual(datum.Size(), 1_z);
		Assert::AreEqual(datum.Capacity(), std::max(list.size(), 5_z));
		Assert::AreEqual(GetHelper<T>(datum, 0), *list.begin());
	}

	template<typename T>
	void TestFind(std::initializer_list<T> list)
	{
		Datum datum(list);

		for (const T& value : list)
		{
			Assert::AreEqual(*datum.Find(value), value);
		}

		T nullTest = T();
		Assert::IsTrue(datum.Find(nullTest) == nullptr);

		const Datum& constDatum = datum;
		Assert::AreEqual(*constDatum.Find(*list.begin()), *list.begin());
	}

	template<typename T>
	void TestRemove(std::initializer_list<T> list)
	{
		Datum datum = list;

		size_t count = 0;
		for (const T& value : list)
		{
			Assert::IsTrue(datum.Remove(value));
			Assert::AreEqual(datum.Size(), list.size() - ++count);
			Assert::IsTrue(datum.Find(value) == nullptr);
		}

		Assert::IsFalse(datum.Remove(*list.begin()));

		datum = list;
		count = 0;
		for (const T& value : list)
		{
			Assert::IsTrue(datum.RemoveAt(0));
			Assert::AreEqual(datum.Size(), list.size() - ++count);
			Assert::IsTrue(datum.Find(value) == nullptr);
		}

		Assert::IsFalse(datum.RemoveAt(0));
	}

	template<typename T>
	void TestSet(std::initializer_list<T> initList, std::initializer_list<T> setList)
	{
		Datum datum = initList;

		size_t count = 0;
		for (const T& value : setList)
		{
			if (count >= datum.Size())
			{
				Assert::ExpectException<std::runtime_error>([&datum, &count, &value] { datum.Set(value, count++); });
			}
			else
			{
				datum.Set(value, count++);
				Assert::AreEqual(value, GetHelper<T>(datum, count - 1));
			}
		}

	}

	template<typename T>
	void TestPopBack(std::initializer_list<T> list)
	{
		Datum datum = list;

		for (size_t i = 0; i < list.size(); ++i)
		{
			T& data = GetHelper<T>(datum, datum.Size() - 1);
			datum.PopBack();
			Assert::AreEqual(datum.Size(), list.size() - i - 1);
			Assert::IsTrue(datum.Find(data) == nullptr);
		}
	}

	template<typename T>
	void TestMove(std::initializer_list<T> list, T* externalArray, size_t size)
	{
		{
			Datum datum = list;
			Datum moveDatum(std::move(datum));
			Assert::AreEqual(moveDatum.Size(), list.size());
			Assert::AreEqual(moveDatum.Capacity(), list.size());
			Assert::IsFalse(moveDatum.IsExternalStorage());

			for (const T& value : list)
			{
				Assert::AreEqual(*moveDatum.Find(value), value);
			}
		}
		{
			Datum datum = list;
			Datum moveDatum;
			moveDatum = std::move(datum);
			Assert::AreEqual(moveDatum.Size(), list.size());
			Assert::AreEqual(moveDatum.Capacity(), list.size());
			Assert::IsFalse(moveDatum.IsExternalStorage());

			for (const T& value : list)
			{
				Assert::AreEqual(*moveDatum.Find(value), value);
			}
		}
		{
			Datum datum;
			datum.SetStorage(externalArray, size);

			Datum moveDatum = std::move(datum);
			Assert::AreEqual(moveDatum.Size(), list.size());
			Assert::AreEqual(moveDatum.Capacity(), list.size());
			Assert::IsTrue(moveDatum.IsExternalStorage());

			for (size_t i = 0; i < size; ++i)
			{
				Assert::AreEqual(*moveDatum.Find(externalArray[i]), externalArray[i]);
			}
		}
	}

	template<typename T>
	void TestCopy(std::initializer_list<T> list, T* externalArray, size_t size)
	{
		{
			Datum datum = list;
			Datum moveDatum(datum);
			Assert::AreEqual(moveDatum.Size(), list.size());
			Assert::AreEqual(moveDatum.Capacity(), list.size());
			Assert::IsFalse(moveDatum.IsExternalStorage());

			for (const T& value : list)
			{
				Assert::AreEqual(*moveDatum.Find(value), value);
			}
		}
		{
			Datum datum = list;
			Datum moveDatum;
			moveDatum = datum;
			Assert::AreEqual(moveDatum.Size(), list.size());
			Assert::AreEqual(moveDatum.Capacity(), list.size());
			Assert::IsFalse(moveDatum.IsExternalStorage());

			for (const T& value : list)
			{
				Assert::AreEqual(*moveDatum.Find(value), value);
			}
		}
		{
			Datum datum;
			datum.SetStorage(externalArray, size);

			Datum moveDatum = datum;
			Assert::AreEqual(moveDatum.Size(), list.size());
			Assert::AreEqual(moveDatum.Capacity(), list.size());
			Assert::IsTrue(moveDatum.IsExternalStorage());

			for (size_t i = 0; i < size; ++i)
			{
				Assert::AreEqual(*moveDatum.Find(externalArray[i]), externalArray[i]);
			}
		}
	}

	template<typename T>
	void TestEquality(std::initializer_list<T> list1, std::initializer_list<T> list2)
	{
		Datum datum = list1;
		Datum datum2 = list1;
		Datum datum3 = list2;

		Assert::IsTrue(datum == datum2);
		Assert::IsTrue(datum != datum3);
	}

	template<typename T>
	void TestExternal(T* externalArray, size_t size)
	{
		Datum datum;
		datum.SetStorage(externalArray, size);
		Assert::AreEqual(datum.Size(), size);
		Assert::AreEqual(datum.Capacity(), size);
		Assert::IsTrue(datum.IsExternalStorage());

		const Datum& constDatum = datum;

		for (size_t i = 0; i < size; ++i)
		{
			Assert::AreEqual(*datum.Find(externalArray[i]), externalArray[i]);
			Assert::AreEqual(GetHelper<T>(datum, i), externalArray[i]);

			Assert::AreEqual(*constDatum.Find(externalArray[i]), externalArray[i]);
			Assert::AreEqual(GetHelper<T>(constDatum, i), externalArray[i]);
		}

		datum.Set(externalArray[size - 1], 0);
		Assert::AreEqual(GetHelper<T>(datum, 0), externalArray[size - 1]);

		Assert::ExpectException<std::runtime_error>([&datum, &externalArray] { datum.PushBack(externalArray[0]); });
		Assert::ExpectException<std::runtime_error>([&datum, &externalArray] { datum.Remove(externalArray[0]); });
		Assert::ExpectException<std::runtime_error>([&datum] { datum.PopBack(); });
		Assert::ExpectException<std::runtime_error>([&datum] { datum.RemoveAt(0); });
		Assert::ExpectException<std::runtime_error>([&datum] { datum.Reserve(5); });
		Assert::ExpectException<std::runtime_error>([&datum] { datum.Resize(5); });

		datum.Clear();
		Assert::AreEqual(datum.Size(), 0_z);
		Assert::AreEqual(datum.Capacity(), 0_z);
		Assert::IsFalse(datum.IsExternalStorage());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTests)
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
			Datum datum;
			Assert::IsTrue(datum.IsEmpty());
			Assert::AreEqual(datum.Capacity(), 0_z);
			Assert::IsTrue(datum.Type() == Datum::DatumTypes::Unknown);
			Assert::IsFalse(datum.IsExternalStorage());
			
			TestScalarConstructor(5);
			TestScalarConstructor(5.0f);
			TestScalarConstructor(glm::vec4(1.0f));
			TestScalarConstructor(glm::mat4(2.0f));
			TestScalarConstructor(std::string("Hello"));

			Foo a(10);
			TestScalarConstructor<Datum::RTTIPointer>(&a);

			TestInitializerListConstructor({ 5, 6, 7 });
			TestInitializerListConstructor({ 5.0f, 6.1f, 7.2f });
			TestInitializerListConstructor({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) });
			TestInitializerListConstructor({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) });
			TestInitializerListConstructor({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday") });

			Foo b(20), c(30);
			TestInitializerListConstructor<Datum::RTTIPointer>({ &a, &b, &c });

		}

		TEST_METHOD(AssignmentAndEquality)
		{
			TestScalarAssignment(5, 6);
			TestScalarAssignment(5.0f, 6.1f);
			TestScalarAssignment(glm::vec4(1.0f), glm::vec4(2.0));
			TestScalarAssignment(glm::mat4(1.0f), glm::mat4(2.0));
			TestScalarAssignment(std::string("Hello"), std::string("Goodbye"));

			Foo a(10), b(20);
			TestScalarAssignment<Datum::RTTIPointer>(&a, &b);

			TestInitializerAssignment({ 5, 6, 7 });
			TestInitializerAssignment({ 5.0f, 6.1f, 7.2f });
			TestInitializerAssignment({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) });
			TestInitializerAssignment({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) });
			TestInitializerAssignment({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday") });

			Foo c(30);
			TestInitializerAssignment<Datum::RTTIPointer>({ &a, &b, &c });
		}

		TEST_METHOD(SetType)
		{
			Datum datum;
			datum.SetType(Datum::DatumTypes::Integer);
			Assert::IsTrue(Datum::DatumTypes::Integer == datum.Type());
			Assert::ExpectException<std::runtime_error>([&datum] { datum.SetType(Datum::DatumTypes::Float); });
		}

		TEST_METHOD(Reserve)
		{
			Datum datum;

			Assert::ExpectException<std::runtime_error>([&datum] { datum.Reserve(5); });
			datum.SetType(Datum::DatumTypes::Integer);
			datum.Reserve(5);
			Assert::IsTrue(datum.IsEmpty());
			Assert::AreEqual(datum.Capacity(), 5_z);

			Datum datum2(ReserveFunctor{});
			datum2.SetType(Datum::DatumTypes::Float);
			datum2.Reserve(5);
			Assert::IsTrue(datum2.IsEmpty());
			Assert::AreEqual(datum2.Capacity(), 5_z);
		}

		TEST_METHOD(Resize)
		{
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Resize(5); });

			TestResize({ 5, 6, 7 });
			TestResize({ 5.0f, 6.1f, 7.2f });
			TestResize({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) });
			TestResize({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) });
			TestResize({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday")});

			Foo a(10), b(20), c(30);
			TestResize<Datum::RTTIPointer>({ &a, &b, &c });
		}

		TEST_METHOD(PushBackAndGet)
		{
			TestPushBackAndGet({ 5, 6 });
			TestPushBackAndGet({ 5.0f, 6.1f });
			TestPushBackAndGet({ glm::vec4(1.0f), glm::vec4(2.0f) });
			TestPushBackAndGet({ glm::mat4(1.0f), glm::mat4(2.0f) });
			TestPushBackAndGet({ std::string("Hello"), std::string("Goodbye") });

			Foo a(10), b(20);
			TestPushBackAndGet<Datum::RTTIPointer>({ &a, &b });
		}

		TEST_METHOD(Find)
		{
			TestFind({ 5, 6, 7 });
			TestFind({ 5.0f, 6.1f, 7.2f });
			TestFind({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) });
			TestFind({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) });
			TestFind({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday") });

			Datum datum;
			Foo a(10), b(20), c(30);
			TestFind<Datum::RTTIPointer>({ &a, &b, &c });
		}

		TEST_METHOD(ToString)
		{
			{
				Datum datum(5);
				Assert::AreEqual(datum.ToString(), std::to_string(5));
				Assert::ExpectException<std::runtime_error>([&datum] { datum.ToString(2); });
			}
			{
				Datum datum(5.0f);
				Assert::AreEqual(datum.ToString(), std::to_string(5.0f));
			}
			{
				Datum datum(glm::vec4(1.0f));
				Assert::AreEqual(datum.ToString(), glm::to_string(glm::vec4(1.0f)));
			}
			{
				Datum datum(glm::mat4(2.0f));
				Assert::AreEqual(datum.ToString(), glm::to_string(glm::mat4(2.0f)));
			}
			{
				Datum datum(std::string("Hello"));
				Assert::AreEqual(datum.ToString(), std::string("Hello"));
			}
			{
				Foo a(10);
				Datum datum(&a);
				Assert::AreEqual(datum.ToString(), a.ToString());
			}
		}

		TEST_METHOD(Remove)
		{
			TestRemove({ 5, 6, 7 });
			TestRemove({ 5.0f, 6.1f, 7.2f });
			TestRemove({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) });
			TestRemove({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) });
			TestRemove({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday") });

			Foo a(10), b(20), c(30);
			TestRemove<Datum::RTTIPointer>({ &a, &b, &c });
		}

		TEST_METHOD(Set)
		{
			TestSet({ 5, 6, 7 }, { 15, 16, 17, 18 });
			TestSet({ 5.0f, 6.1f, 7.2f }, { 15.0f, 16.1f, 17.2f, 18.3f });
			TestSet({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) }, { glm::vec4(11.0f), glm::vec4(12.0f), glm::vec4(13.0f), glm::vec4(15.0f) });
			TestSet({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) }, { glm::mat4(11.0f), glm::mat4(12.0f), glm::mat4(13.0f), glm::mat4(15.0f) });
			TestSet({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday") }, { std::string("HE"), std::string("GO"), std::string("GA"), std::string("GE") });

			Datum datum;
			Foo a(10), b(20), c(30), d(40), e(50);
			TestSet<Datum::RTTIPointer>({ &a, &b, &c }, { &c, &d, &e, &a });
		}

		TEST_METHOD(Pop)
		{
			TestPopBack({ 5, 6, 7 });
			TestPopBack({ 5.0f, 6.1f, 7.2f });
			TestPopBack({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) });
			TestPopBack({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) });
			TestPopBack({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday") });

			Foo a(10), b(20), c(30);
			TestPopBack<Datum::RTTIPointer>({ &a, &b, &c });
		}

		TEST_METHOD(Move)
		{
			{
				int array[3] = { 5,6,7 };
				TestMove({ 5, 6, 7 }, array, 3);
			}
			{
				float array[3] = { 5.0f, 6.1f, 7.2f };
				TestMove({ 5.0f, 6.1f, 7.2f }, array, 3);
			}
			{
				glm::vec4 array[3] = { glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) };
				TestMove({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) }, array, 3);
			}
			{
				glm::mat4 array[3] = { glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) };
				TestMove({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) }, array, 3);
			}
			{
				std::string array[3] = { std::string("Hello"), std::string("Goodbye"), std::string("Gooday") };
				TestMove({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday") }, array, 3);
			}
			{
				Foo a(10), b(20), c(30);
				Datum::RTTIPointer array[3] = { &a, &b, &c };
				TestMove<Datum::RTTIPointer>({ &a, &b, &c }, array, 3);
			}
		}

		TEST_METHOD(Copy)
		{
			{
				int array[3] = { 5,6,7 };
				TestCopy({ 5, 6, 7 }, array, 3);
			}
			{
				float array[3] = { 5.0f, 6.1f, 7.2f };
				TestCopy({ 5.0f, 6.1f, 7.2f }, array, 3);
			}
			{
				glm::vec4 array[3] = { glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) };
				TestCopy({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) }, array, 3);
			}
			{
				glm::mat4 array[3] = { glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) };
				TestCopy({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) }, array, 3);
			}
			{
				std::string array[3] = { std::string("Hello"), std::string("Goodbye"), std::string("Gooday") };
				TestCopy({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday") }, array, 3);
			}
			{
				Foo a(10), b(20), c(30);
				Datum::RTTIPointer array[3] = { &a, &b, &c };
				TestCopy<Datum::RTTIPointer>({ &a, &b, &c }, array, 3);
			}
		}

		TEST_METHOD(Equality)
		{
			TestEquality({ 5, 6, 7 }, { 15, 16, 17, 18 });
			TestEquality({ 5.0f, 6.1f, 7.2f }, { 15.0f, 16.1f, 17.2f, 18.3f });
			TestEquality({ glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) }, { glm::vec4(11.0f), glm::vec4(12.0f), glm::vec4(13.0f), glm::vec4(15.0f) });
			TestEquality({ glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) }, { glm::mat4(11.0f), glm::mat4(12.0f), glm::mat4(13.0f), glm::mat4(15.0f) });
			TestEquality({ std::string("Hello"), std::string("Goodbye"), std::string("Gooday") }, { std::string("HE"), std::string("GO"), std::string("GA"), std::string("GE") });

			// Test RTTI* since it didn't work with the template
			Datum datum;
			Foo a(10), b(20), c(30), d(40), e(50);
			TestEquality<Datum::RTTIPointer>({ &a, &b, &c }, { &c, &d, &e, &a });
		}

		TEST_METHOD(ExternalStorage)
		{
			{
				int array[3] = { 5,6,7 };
				TestExternal(array, 3);
			}
			{
				float array[3] = { 5.0f, 6.1f, 7.2f };
				TestExternal(array, 3);
			}
			{
				glm::vec4 array[3] = { glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) };
				TestExternal(array, 3);
			}
			{
				glm::mat4 array[3] = { glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) };
				TestExternal(array, 3);
			}
			{
				std::string array[3] = { std::string("Hello"), std::string("Goodbye"), std::string("Gooday") };
				TestExternal(array, 3);
			}
			{
				Foo a(10), b(20), c(30);
				Datum::RTTIPointer array[3] = { &a, &b, &c };
				TestExternal<Datum::RTTIPointer>(array, 3);
			}
		}

		TEST_METHOD(SetFromString)
		{
			{
				Datum datum = {5, 6, 7};
				datum.SetFromString(datum.ToString(2), 0);
				Assert::AreEqual(datum.GetInt(0), datum.GetInt(2));
				Assert::ExpectException<std::runtime_error>([&datum] { datum.SetFromString(datum.ToString(2), 5); });
			}

			{
				Datum datum = { 5.0f, 6.1f, 7.2f };
				datum.SetFromString(datum.ToString(2), 0);
				Assert::AreEqual(datum.GetFloat(0), datum.GetFloat(2));
			}

			{
				Datum datum = { glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f) };
				datum.SetFromString(datum.ToString(2), 0);
				Assert::AreEqual(datum.GetVector(0), datum.GetVector(2));
			}

			{
				Datum datum = { glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) };
				datum.SetFromString(datum.ToString(2), 0);
				Assert::AreEqual(datum.GetMatrix(0), datum.GetMatrix(2));
			}

			{
				Datum datum = { std::string("Hello"), std::string("Goodbye"), std::string("Gooday") };
				datum.SetFromString(datum.ToString(2), 0);
				Assert::AreEqual(datum.GetString(0), datum.GetString(2));
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DatumTests::sStartMemState;
}