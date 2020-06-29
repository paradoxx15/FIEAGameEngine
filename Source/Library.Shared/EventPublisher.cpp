#include "pch.h"
#include "EventPublisher.h"
#include <future>
#include <vector>

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscribers, std::mutex& eventMutex) :
		mSubscribers(&subscribers), mMutex(&eventMutex)
	{
	}

	void EventPublisher::Deliver() const
	{
		vector<future<void>> threads;
		
		{
			scoped_lock<mutex> lock(*mMutex);

			for (EventSubscriber* subscriber : *mSubscribers)
			{
				threads.emplace_back(async(launch::async, [subscriber, this] { subscriber->Notify(*this); }));
			}
		}

		std::string exceptionInfo;
		for (auto& thread : threads)
		{
			try
			{
				thread.get();
			}
			catch (std::exception& e)
			{
				exceptionInfo += e.what();
			}
		}

		if (!exceptionInfo.empty())
		{
			throw std::runtime_error(exceptionInfo);
		}
	}
}