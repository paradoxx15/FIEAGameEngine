#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <future>
#include <thread>
#include "EventPublisher.h"
#include "GameTime.h"
#include "Vector.h"

namespace Library
{
	class EventQueue final
	{
	public:

		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using Milliseconds = std::chrono::milliseconds;

		/// <summary>
		/// Default base constructor.
		/// </summary>
		EventQueue() = default;
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The EventQueue being copied</param>
		EventQueue(const EventQueue& rhs);
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The EventQueue being moved into this one</param>
		EventQueue(EventQueue&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The EventQueue being copied</param>
		/// <returns>A reference to this EventQueue after being mutated</returns>
		EventQueue& operator=(const EventQueue & rhs);
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The EventQueue being moved into this one</param>
		/// <returns>A reference to this EventQueue after being mutated</returns>
		EventQueue& operator=(EventQueue && rhs) = default;
		/// <summary>
		/// Virtual default destructor. 
		/// </summary>
		~EventQueue() = default;

		/// <summary>
		/// Adds the passed in event to the EventQueue with a specified delay time before firing off the event.
		/// </summary>
		/// <param name="event">A shared_ptr to the Event being added to the queue</param>
		/// <param name="gameTime">The current game time, used to grab the time in which this event was enqueued</param>
		/// <param name="delay">The ammount of milliseconds from the enqueued time that this event will fire</param>
		void Enqueue(std::shared_ptr<EventPublisher> event, GameTime& gameTime, Milliseconds delay = Milliseconds(0));

		/// <summary>
		/// Given an Event this method will immediately fire off the Event off
		/// </summary>
		/// <param name="event">The Event to be fired off</param>
		void Send(EventPublisher& event) const;

		/// <summary>
		/// Publishes any queued events that have expired.
		/// </summary>
		/// <param name="gameTime">The current game time. Used to determine what events have expired.</param>
		void Update(const GameTime& gameTime);

		/// <summary>
		/// Clears the event queue.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns whether or not the queue is empty.
		/// </summary>
		/// <returns>True if the queue is empty, false otherwise</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Returns the number of events in the queue.
		/// </summary>
		/// <returns>A size_t representing the number of events in the queue</returns>
		size_t Size() const;

	private:

		/// <summary>
		/// The information stored in one frame of the queue. Contains the event and the time in which it will expire.
		/// </summary>
		struct QueueFrame
		{
			std::shared_ptr<EventPublisher> QueuedEvent;
			TimePoint ExpiredTime;
		};

		/// <summary>
		/// Holds all the needed information for each event within the queue.
		/// </summary>
		Vector<QueueFrame> mEventQueue;

		/// <summary>
		/// Mutex used to lock access to the shared resources of the EventQueue
		/// </summary>
		mutable std::mutex mMutex;
	};
}

