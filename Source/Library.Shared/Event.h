#pragma once
#include "EventPublisher.h"
#include <mutex>
#include <future>
#include <thread>

namespace Library
{
	template <typename T>
	class Event final : public EventPublisher
	{
		friend class ActionEvent;

		RTTI_DECLARATIONS(Event<T>, EventPublisher);

	public:

		/// <summary>
		/// Adds the passed in subscriber to this Events list of subscribers
		/// </summary>
		/// <param name="subscriber">A reference to the subscriber to be added</param>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Removes the passed in subscriber from this Events list of subscribers
		/// </summary>
		/// <param name="subscriber">A reference to the subscriber being removed</param>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Unsubscribes all subscribers from this event.
		/// Note: Can't be called in the middle of an events deliver method.
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Shrinks the subscribers vectors capacity equal to the number of subscribers
		/// </summary>
		static void ShrinkSubscribersToFit();

		/// <summary>
		/// Returns the number of subscribers this event has
		/// </summary>
		/// <returns>The number of subscribers this event has</returns>
		static size_t NumSubscribers();

		/// <summary>
		/// Deleted default compiler constructor
		/// </summary>
		Event() = delete;
		/// <summary>
		/// Constructor that takes a reference to the message object that will be sent.
		/// </summary>
		/// <param name="messageObject">The message object being sent</param>
		Event(const T& messageObject);
		/// <summary>
		/// Constructor that takes a r-value reference to the message object that will be sent.
		/// </summary>
		/// <param name="messageObject">The message object being sent</param>
		Event(T&& messageObject);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The Event being copied</param>
		Event(const Event& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The Event being moved into this one</param>
		Event(Event&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The Event being copied</param>
		/// <returns>A reference to this Event after being mutated</returns>
		Event& operator=(const Event& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The Event being moved into this one</param>
		/// <returns>A reference to this Event after being mutated</returns>
		Event& operator=(Event&& rhs) = default;
		/// <summary>
		/// Default destructor
		/// </summary>
		~Event() = default;

		/// <summary>
		/// Returns a const reference to the message contained within this Event
		/// </summary>
		/// <returns>Const reference to the message contained within this Event</returns>
		const T& Message() const;

		/// <summary>
		/// Returns a string representation of the Event.
		/// </summary>
		/// <returns>A string representation of the Event</returns>
		std::string ToString() const override;

		/// <summary>
		/// Checks whether or not two Events are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the Event object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	private:
		/// <summary>
		/// Static list of subscribers to this type of event
		/// </summary>
		inline static Vector<EventSubscriber*> Subscribers;

		/// <summary>
		/// Mutex that locks access to the shared data of the Event.
		/// </summary>
		inline static std::mutex Mutex;

		/// <summary>
		/// The payload message that will be sent when this event fires off.
		/// </summary>
		T mMessage;
	};
}

#include "Event.inl"