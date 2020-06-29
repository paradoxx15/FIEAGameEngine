#include "pch.h"
#include "EventQueue.h"
#include <algorithm>
#include <vector>

using namespace std;

namespace Library
{
	EventQueue::EventQueue(const EventQueue& rhs) :
		mEventQueue(rhs.mEventQueue), mMutex()
	{
	}

	EventQueue& EventQueue::operator=(const EventQueue& rhs)
	{
		mEventQueue = rhs.mEventQueue;
		return *this;
	}

	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> event, GameTime& gameTime, Milliseconds delay)
	{
		scoped_lock<mutex> lock(mMutex);

		// Check that this event is not a duplicate
		for (const QueueFrame& frame : mEventQueue)
		{
			if (event == frame.QueuedEvent)
			{
				return;
			}
		}

		mEventQueue.PushBack({ std::move(event), gameTime.CurrentTime() + delay });
	}

	void EventQueue::Send(EventPublisher& event) const
	{
		event.Deliver();
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		vector<future<void>> threads;

		{
			scoped_lock<mutex> lock(mMutex);

			auto expiredIt = std::partition(mEventQueue.begin(), mEventQueue.end(), [&gameTime](QueueFrame frame) { return (gameTime.CurrentTime() < frame.ExpiredTime); });
			auto it = expiredIt;

			while (it != mEventQueue.end())
			{
				QueueFrame& frame = *it;
				threads.emplace_back(async(launch::async, [frame] { frame.QueuedEvent->Deliver(); }));
				++it;
			}

			mEventQueue.Remove(expiredIt, mEventQueue.end());
		}

		std::string exceptionInfo;
		for (auto& thread : threads)
		{
			try
			{
				thread.get();
			}
			catch (std::exception e)
			{
				exceptionInfo += e.what();
			}
		}

		if (!exceptionInfo.empty())
		{
			throw std::runtime_error(exceptionInfo);
		}
	}

	void EventQueue::Clear()
	{
		scoped_lock<mutex> lock(mMutex);
		mEventQueue.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		scoped_lock<mutex> lock(mMutex);
		return mEventQueue.IsEmpty();
	}

	size_t EventQueue::Size() const
	{
		scoped_lock<mutex> lock(mMutex);
		return mEventQueue.Size();
	}
}