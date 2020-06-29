#include "pch.h"
#include "CppUnitTest.h"
#include "EventQueue.h"
#include "EventSubscriber.h"
#include "EventPublisher.h"
#include "Event.h"
#include "GameClock.h"
#include "GameTime.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include <algorithm>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;

namespace Library
{
	class TestSubscriber : public EventSubscriber
	{
	public:

		TestSubscriber(GameTime* gameTime = nullptr, EventQueue* eventQueue = nullptr, TestSubscriber* otherSubscriber = nullptr) :
			mGameTime(gameTime), mEventQueue(eventQueue), mOtherSubscriber(otherSubscriber)
		{
		}

		void Notify(const EventPublisher& payload) override
		{
			if (payload.Is(Event<Foo>::TypeIdClass()))
			{
				testCondition = 1;
				testFoo = static_cast<const Event<Foo>&>(payload).Message();
			}

			if (payload.Is(Event<std::shared_ptr<Foo>>::TypeIdClass()))
			{
				testCondition = 2;
				testFooPtr = &static_cast<const Event<std::shared_ptr<Foo>>&>(payload).Message();
			}

			if (payload.Is(Event<int>::TypeIdClass()))
			{
				const Event<int>& intEvent = static_cast<const Event<int>&>(payload);

				if (intEvent.Message() == 0)
				{
					std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(Event<Foo>(Foo(10)));
					mEventQueue->Enqueue(fooEvent, *mGameTime, std::chrono::milliseconds(0));
					testCondition = 3;
				}
				else if (intEvent.Message() == 1)
				{
					if (mOtherSubscriber != nullptr)
					{
						Event<int>::Unsubscribe(*mOtherSubscriber);
					}
					testCondition = 4;
				}
				else if (intEvent.Message() == 2)
				{
					Event<int>::Subscribe(*mOtherSubscriber);
					testCondition = 5;
				}
				else if (intEvent.Message() == 3)
				{
					throw std::runtime_error("This is an exception test");
				}
				else
				{
					testCondition = 6;
				}
			}
		}

		int testCondition = 0;
		Foo testFoo;
		const std::shared_ptr<Foo>* testFooPtr = nullptr;
		GameTime* mGameTime;
		EventQueue* mEventQueue;
		TestSubscriber* mOtherSubscriber;
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTests)
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

		TEST_METHOD(EventConstruction)
		{
			Foo a(10);
			Event<Foo> fooEvent(a);
			Assert::AreEqual(fooEvent.Message(), a);
		}

		TEST_METHOD(EventQueueConstruction)
		{
			EventQueue eventQueue;
			Assert::IsTrue(eventQueue.IsEmpty());
			Assert::AreEqual(0_z, eventQueue.Size());
		}

		TEST_METHOD(EventSubscriberConstruction)
		{
			TestSubscriber testSubscriber;
			Assert::AreEqual(0, testSubscriber.testCondition);
			Assert::AreEqual(Foo(), testSubscriber.testFoo);
			Assert::IsNull(testSubscriber.testFooPtr);
		}

		TEST_METHOD(SendEvents)
		{
			EventQueue eventQueue;
			TestSubscriber subscriber;
			Event<Foo>::Subscribe(subscriber);
			Event<std::shared_ptr<Foo>>::Subscribe(subscriber);

			Event<Foo> fooEvent(Foo(10));
			eventQueue.Send(fooEvent);
			Assert::AreEqual(1, subscriber.testCondition);
			Assert::AreEqual(Foo(10), subscriber.testFoo);

			std::shared_ptr<Foo> testPtr = std::make_shared<Foo>(Foo(10));
			Event<std::shared_ptr<Foo>> ptrEvent(testPtr);
			eventQueue.Send(ptrEvent);
			Assert::AreEqual(2, subscriber.testCondition);
			Assert::IsTrue(ptrEvent.Message() == *subscriber.testFooPtr);

			Event<Foo>::UnsubscribeAll();
			Event<std::shared_ptr<Foo>>::UnsubscribeAll();
		}

		TEST_METHOD(QueuedEvents)
		{
			using namespace std::chrono;

			GameClock gameClock;
			GameTime gameTime;
			EventQueue eventQueue;
			TestSubscriber subscriber;
			Event<Foo>::Subscribe(subscriber);
			Event<Foo> fooEvent(Foo(10));

			gameClock.UpdateGameTime(gameTime);
			high_resolution_clock::time_point startTime = gameTime.CurrentTime();
			eventQueue.Enqueue(std::make_shared<Event<Foo>>(fooEvent), gameTime, milliseconds(1000));

			while (subscriber.testCondition == 0)
			{
				gameClock.UpdateGameTime(gameTime);
				eventQueue.Update(gameTime);
			}

			high_resolution_clock::time_point endTime = gameTime.CurrentTime();

			Assert::AreEqual(1, subscriber.testCondition);
			Assert::AreEqual(Foo(10), subscriber.testFoo);
			Assert::IsTrue(duration_cast<std::chrono::milliseconds>(endTime - startTime) >= std::chrono::milliseconds(1000));

			Event<Foo>::UnsubscribeAll();
			Event<Foo>::ShrinkSubscribersToFit();
			eventQueue.Clear();
		}

		TEST_METHOD(MidAddToQueue)
		{
			using namespace std::chrono;

			GameClock gameClock;
			GameTime gameTime;
			EventQueue eventQueue;
			TestSubscriber subscriber(&gameTime, &eventQueue);
			Event<int>::Subscribe(subscriber);
			Event<Foo>::Subscribe(subscriber);
			std::shared_ptr<Event<int>> intEvent = std::make_shared<Event<int>>(Event<int>(0));

			gameClock.UpdateGameTime(gameTime);
			eventQueue.Enqueue(intEvent, gameTime);

			eventQueue.Update(gameTime);
			
			Assert::AreEqual(3, subscriber.testCondition);

			eventQueue.Update(gameTime);

			Assert::AreEqual(1, subscriber.testCondition);
			Assert::AreEqual(Foo(10), subscriber.testFoo);

			Event<Foo>::UnsubscribeAll();
			Event<int>::UnsubscribeAll();
			eventQueue.Clear();
		}

		TEST_METHOD(MidSubscribeUnsubscribe)
		{
			using namespace std::chrono;

			GameClock gameClock;
			GameTime gameTime;
			EventQueue eventQueue;
			TestSubscriber subscriber(&gameTime, &eventQueue);
			TestSubscriber subscriber2(&gameTime, &eventQueue, &subscriber);
			Event<int>::Subscribe(subscriber2);
			std::shared_ptr<Event<int>> intEvent = std::make_shared<Event<int>>(Event<int>(2));

			gameClock.UpdateGameTime(gameTime);
			eventQueue.Enqueue(intEvent, gameTime);

			eventQueue.Update(gameTime);

			Assert::AreEqual(5, subscriber2.testCondition);
			Assert::AreEqual(0, subscriber.testCondition);

			intEvent = std::make_shared<Event<int>>(Event<int>(6));
			eventQueue.Enqueue(intEvent, gameTime);
			eventQueue.Update(gameTime);

			Assert::AreEqual(6, subscriber.testCondition);
			Assert::AreEqual(6, subscriber2.testCondition);

			intEvent = std::make_shared<Event<int>>(Event<int>(1));
			eventQueue.Enqueue(intEvent, gameTime);
			eventQueue.Update(gameTime);

			Assert::AreEqual(4, subscriber.testCondition);
			Assert::AreEqual(4, subscriber2.testCondition);

			intEvent = std::make_shared<Event<int>>(Event<int>(6));
			eventQueue.Enqueue(intEvent, gameTime);
			eventQueue.Update(gameTime);

			Assert::AreEqual(4, subscriber.testCondition);
			Assert::AreEqual(6, subscriber2.testCondition);

			Event<Foo>::UnsubscribeAll();
			Event<int>::UnsubscribeAll();
			eventQueue.Clear();
		}

		TEST_METHOD(AsyncEnqueueStressTest)
		{
			using namespace std::chrono;

			GameClock gameClock;
			GameTime gameTime;
			EventQueue eventQueue;
			const size_t numSubscribers = 200;

			TestSubscriber subscribers[numSubscribers];

			for (size_t i = 0; i < numSubscribers; ++i)
			{
				subscribers[i].mGameTime = &gameTime;
				subscribers[i].mEventQueue = &eventQueue;
				Event<int>::Subscribe(subscribers[i]);
				Event<Foo>::Subscribe(subscribers[i]);
			}

			std::shared_ptr<Event<int>> intEvent = std::make_shared<Event<int>>(Event<int>(0));

			gameClock.UpdateGameTime(gameTime);
			eventQueue.Enqueue(intEvent, gameTime);

			Assert::AreEqual(1_z, eventQueue.Size());

			eventQueue.Update(gameTime);

			Assert::AreEqual(numSubscribers, eventQueue.Size());

			for (size_t i = 0; i < numSubscribers; ++i)
			{
				Assert::AreEqual(3, subscribers[i].testCondition);
			}

			eventQueue.Update(gameTime);

			for (size_t i = 0; i < numSubscribers; ++i)
			{
				Assert::AreEqual(1, subscribers[i].testCondition);
				Assert::AreEqual(Foo(10), subscribers[i].testFoo);
			}

			Event<Foo>::UnsubscribeAll();
			Event<int>::UnsubscribeAll();
			eventQueue.Clear();
		}

		TEST_METHOD(AsyncSubscribeStressTest)
		{
			using namespace std::chrono;

			GameClock gameClock;
			GameTime gameTime;
			EventQueue eventQueue;
			const size_t numSubscribers = 100;

			TestSubscriber subscribers[numSubscribers];
			TestSubscriber secondSubscribers[numSubscribers];

			for (size_t i = 0; i < numSubscribers; ++i)
			{
				secondSubscribers[i].mGameTime = &gameTime;
				secondSubscribers[i].mEventQueue = &eventQueue;

				subscribers[i].mGameTime = &gameTime;
				subscribers[i].mEventQueue = &eventQueue;
				subscribers[i].mOtherSubscriber = &secondSubscribers[i];
				Event<int>::Subscribe(subscribers[i]);
			}
			std::shared_ptr<Event<int>> intEvent = std::make_shared<Event<int>>(Event<int>(2));

			gameClock.UpdateGameTime(gameTime);
			eventQueue.Enqueue(intEvent, gameTime);

			eventQueue.Update(gameTime);

			for (size_t i = 0; i < numSubscribers; ++i)
			{
				Assert::AreEqual(5, subscribers[i].testCondition);
				Assert::AreEqual(0, secondSubscribers[i].testCondition);
			}

			intEvent = std::make_shared<Event<int>>(Event<int>(6));
			eventQueue.Enqueue(intEvent, gameTime);
			eventQueue.Update(gameTime);

			for (size_t i = 0; i < numSubscribers; ++i)
			{
				Assert::AreEqual(6, subscribers[i].testCondition);
				Assert::AreEqual(6, secondSubscribers[i].testCondition);
			}

			intEvent = std::make_shared<Event<int>>(Event<int>(1));
			eventQueue.Enqueue(intEvent, gameTime);
			eventQueue.Update(gameTime);

			for (size_t i = 0; i < numSubscribers; ++i)
			{
				Assert::AreEqual(4, subscribers[i].testCondition);
				Assert::AreEqual(4, secondSubscribers[i].testCondition);
			}

			intEvent = std::make_shared<Event<int>>(Event<int>(6));
			eventQueue.Enqueue(intEvent, gameTime);
			eventQueue.Update(gameTime);

			for (size_t i = 0; i < numSubscribers; ++i)
			{
				Assert::AreEqual(6, subscribers[i].testCondition);
				Assert::AreEqual(4, secondSubscribers[i].testCondition);
			}

			Event<Foo>::UnsubscribeAll();
			Event<int>::UnsubscribeAll();
			eventQueue.Clear();
		}

		TEST_METHOD(AsyncExceptionTest)
		{
			using namespace std::chrono;

			GameClock gameClock;
			GameTime gameTime;
			EventQueue eventQueue;
			const size_t numSubscribers = 200;

			TestSubscriber subscribers[numSubscribers];

			for (size_t i = 0; i < numSubscribers; ++i)
			{
				subscribers[i].mGameTime = &gameTime;
				subscribers[i].mEventQueue = &eventQueue;
				Event<int>::Subscribe(subscribers[i]);
			}

			std::shared_ptr<Event<int>> intEvent = std::make_shared<Event<int>>(Event<int>(3));

			gameClock.UpdateGameTime(gameTime);
			eventQueue.Enqueue(intEvent, gameTime);

			Assert::AreEqual(1_z, eventQueue.Size());

			Assert::ExpectException<std::runtime_error>([&eventQueue, &gameTime] {eventQueue.Update(gameTime); });

			Event<int>::UnsubscribeAll();
			eventQueue.Clear();
		}

		TEST_METHOD(DuplicateTests)
		{
			using namespace std::chrono;

			GameClock gameClock;
			GameTime gameTime;
			EventQueue eventQueue;
			TestSubscriber subscriber(&gameTime, &eventQueue);
			Event<int>::Subscribe(subscriber);
			Event<int> intEvent(6);

			eventQueue.Send(intEvent);

			Assert::AreEqual(1_z, Event<int>::NumSubscribers());

			Event<int>::Subscribe(subscriber);
			eventQueue.Send(intEvent);

			Assert::AreEqual(1_z, Event<int>::NumSubscribers());

			std::shared_ptr<Event<int>> intEvent2 = std::make_shared<Event<int>>(Event<int>(2));

			gameClock.UpdateGameTime(gameTime);
			eventQueue.Enqueue(intEvent2, gameTime, milliseconds(100));
			Assert::AreEqual(1_z, eventQueue.Size());
			eventQueue.Enqueue(intEvent2, gameTime, milliseconds(100));
			Assert::AreEqual(1_z, eventQueue.Size());

			eventQueue.Update(gameTime);

			Assert::AreEqual(1_z, eventQueue.Size());

			eventQueue.Enqueue(intEvent2, gameTime);
			eventQueue.Update(gameTime);
			Assert::AreEqual(1_z, eventQueue.Size());

			EventQueue copyQueue;
			copyQueue = eventQueue;
			Assert::AreEqual(1_z, eventQueue.Size());

			Event<int>::UnsubscribeAll();
		}

		TEST_METHOD(RTTITests)
		{
			{
				Foo a(10);
				Event<Foo> fooEvent(a);
				Foo b(10);
				Event<Foo> fooEvent2(b);
				Foo c(20);
				Event<Foo> fooEvent3(c);
				Assert::IsTrue(fooEvent.Is(Event<Foo>::TypeIdClass()));
				Assert::IsTrue(fooEvent.Is(EventPublisher::TypeIdClass()));

				Assert::AreEqual("Event"s, fooEvent.ToString());
				Assert::IsTrue(fooEvent.Equals(&fooEvent2));
				Assert::IsFalse(fooEvent.Equals(&fooEvent3));
				Assert::IsFalse(fooEvent.Equals(&a));
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventTests::sStartMemState;
}