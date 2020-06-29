#pragma once

namespace Library
{
	class EventSubscriber
	{
	public:

		/// <summary>
		/// Virtual default destructor. 
		/// </summary>
		virtual ~EventSubscriber() = default;

		/// <summary>
		/// The method used by an EventPublisher to notify this EventSubscriber with an event payload.
		/// </summary>
		/// <param name="payload">The payload message of the Event that was fired</param>
		virtual void Notify(const class EventPublisher& payload) = 0;

	protected:
		/// <summary>
		/// Default base constructor.
		/// </summary>
		EventSubscriber() = default;
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The EventSubscriber being copied</param>
		EventSubscriber(const EventSubscriber & rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The EventSubscriber being moved into this one</param>
		EventSubscriber(EventSubscriber && rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The EventSubscriber being copied</param>
		/// <returns>A reference to this EventSubscriber after being mutated</returns>
		EventSubscriber& operator=(const EventSubscriber & rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The EventSubscriber being moved into this one</param>
		/// <returns>A reference to this EventSubscriber after being mutated</returns>
		EventSubscriber& operator=(EventSubscriber && rhs) = default;
	};
}

