#include "pch.h"
#include "EventMessageAttributed.h"

namespace Library
{
	RTTI_DEFINITIONS(EventMessageAttributed);

	const Vector<Signature> EventMessageAttributed::GetSignatures()
	{
		return Vector<Signature>();
	}

	EventMessageAttributed::EventMessageAttributed(const std::string& subtype, World& world) :
		Attributed(TypeIdClass()), mSubtype(subtype), mWorld(&world)
	{
	}

	EventMessageAttributed::EventMessageAttributed(std::string&& subtype, World& world) :
		Attributed(TypeIdClass()), mSubtype(std::move(subtype)), mWorld(&world)
	{
	}

	const std::string& EventMessageAttributed::Subtype() const
	{
		return mSubtype;
	}

	void EventMessageAttributed::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	World& EventMessageAttributed::GetWorld()
	{
		return *mWorld;
	}

	const World& EventMessageAttributed::GetWorld() const
	{
		return *mWorld;
	}

	void EventMessageAttributed::SetWorld(World& world)
	{
		mWorld = &world;
	}

	gsl::owner<Scope*> EventMessageAttributed::Clone() const
	{
		return new EventMessageAttributed(*this);
	}

	std::string EventMessageAttributed::ToString() const
	{
		return "EventMessageAttributed";
	}
	bool EventMessageAttributed::Equals(const RTTI* rhs) const
	{
		const EventMessageAttributed* eventMessage = rhs->As<EventMessageAttributed>();
		if (eventMessage == nullptr)
		{
			return false;
		}

		return (this->mSubtype == eventMessage->mSubtype) && (this->mWorld == eventMessage->mWorld) && (*this == *eventMessage);
	}
}