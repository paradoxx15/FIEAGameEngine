#pragma once

#include <chrono>
#include <mutex>
#include <thread>
#include <future>
#include "RTTI.h"
#include "Vector.h"
#include "EventSubscriber.h"

namespace Library
{
	class EventPublisher : public RTTI
	{
		friend class EventQueue;

		RTTI_DECLARATIONS(EventPublisher, RTTI)
		
	public:

		/// <summary>
		/// Virtual default destructor. 
		/// </summary>
		virtual ~EventPublisher() = default;

	protected:

		/// <summary>
		/// Deleted base constructor.
		/// </summary>
		EventPublisher() = delete;
		/// <summary>
		/// EventPublisher constructor that takes a list of subscribers and an optional bool to determine if the EventPublisher should be deleted after publishing.
		/// </summary>
		/// <param name="subscribers">The list of EventSubscribers</param>
		/// <param name="eventMutex">The mutex for the EventSubscriber to lock async calls</param>
		EventPublisher(Vector<EventSubscriber*>& subscribers, std::mutex& eventMutex);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The EventPublisher being copied</param>
		EventPublisher(const EventPublisher& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The EventPublisher being moved into this one</param>
		EventPublisher(EventPublisher&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The EventPublisher being copied</param>
		/// <returns>A reference to this EventPublisher after being mutated</returns>
		EventPublisher& operator=(const EventPublisher & rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The EventPublisher being moved into this one</param>
		/// <returns>A reference to this EventPublisher after being mutated</returns>
		EventPublisher& operator=(EventPublisher && rhs) = default;

		/// <summary>
		/// Delivers the EventPublishers payload to all subscribers
		/// </summary>
		void Deliver() const;

		/// <summary>
		/// A vector that contains all the subscribers to this EventPublisher.
		/// </summary>
		Vector<EventSubscriber*>* mSubscribers;

		/// <summary>
		/// Mutex used to lock access to the shared resources of the EventPublisher.
		/// </summary>
		mutable std::mutex* mMutex;
	};
}

