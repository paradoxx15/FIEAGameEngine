#include "Event.h"

using namespace std;

namespace Library
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>); 

	template<typename T>
	inline void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		scoped_lock<mutex> lock(Mutex);

		if (Subscribers.Find(&subscriber) != Subscribers.end())
		{
			return;
		}
		Subscribers.PushBack(&subscriber);
	}

	template<typename T>
	inline void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		scoped_lock<mutex> lock(Mutex);
		Subscribers.Remove(&subscriber);
	}

	template<typename T>
	inline void Event<T>::UnsubscribeAll()
	{
		scoped_lock<mutex> lock(Mutex);
		Subscribers.Clear();
		Subscribers.ShrinkToFit();
	}

	template<typename T>
	inline void Event<T>::ShrinkSubscribersToFit()
	{
		scoped_lock<mutex> lock(Mutex);
		Subscribers.ShrinkToFit();
	}

	template<typename T>
	inline size_t Event<T>::NumSubscribers()
	{
		scoped_lock<mutex> lock(Mutex);
		return Subscribers.Size();
	}

	template<typename T>
	inline Event<T>::Event(const T& messageObject) : 
		EventPublisher(Subscribers, Mutex), mMessage(messageObject)
	{
	}

	template<typename T>
	inline Event<T>::Event(T&& messageObject) :
		EventPublisher(Subscribers, Mutex), mMessage(std::move(messageObject))
	{
	}

	template<typename T>
	inline const T& Event<T>::Message() const
	{
		return mMessage;
	}

	template<typename T>
	inline std::string Event<T>::ToString() const
	{
		return "Event";
	}

	template<typename T>
	inline bool Event<T>::Equals(const RTTI* rhs) const
	{
		if (!rhs->Is(Event<T>::TypeIdClass()))
		{
			return false;
		}

		return static_cast<const Event<T>*>(rhs)->mMessage == mMessage;
	}


}