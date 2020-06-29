#include "pch.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "Event.h"

namespace Library
{
	RTTI_DEFINITIONS(ReactionAttributed);

	const Vector<Signature> ReactionAttributed::GetSignatures()
	{
		return Vector<Signature>
		{ 
			{ "Subtype"s, Datum::DatumTypes::String, true, 1, offsetof(ReactionAttributed, mSubtype) }
		};
	}

	ReactionAttributed::ReactionAttributed() : 
		Reaction(TypeIdClass()) 
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const std::string& name, const std::string& subtype) :
		Reaction(TypeIdClass(), name), mSubtype(subtype)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	void ReactionAttributed::Notify(const EventPublisher& payload)
	{
		if (payload.Is(Event<EventMessageAttributed>::TypeIdClass()))
		{
			const Event<EventMessageAttributed>& attributedEvent = static_cast<const Event<EventMessageAttributed>&>(payload);
			const EventMessageAttributed& eventMessage = attributedEvent.Message();

			if (eventMessage.Subtype() == mSubtype)
			{
				WorldState& worldState = const_cast<WorldState&>(eventMessage.GetWorld().GetWorldState());
				Scope argumentsScope;

				const Vector<Scope::PairType*>& messageAttributeVector = eventMessage.GetOrderVector();
				for (size_t i = 0; i < messageAttributeVector.Size(); ++i)
				{
					argumentsScope.Append(messageAttributeVector[i]->first) = messageAttributeVector[i]->second;
				}

				worldState.GetArgumentStack().Push(std::move(argumentsScope));
				ActionList::Update(worldState);
			}
		}
	}

	gsl::owner<Scope*> ReactionAttributed::Clone() const
	{
		return new ReactionAttributed(*this);
	}

	bool ReactionAttributed::Equals(const RTTI* rhs) const
	{
		const ReactionAttributed* reactionAttributed = rhs->As<ReactionAttributed>();
		if (reactionAttributed == nullptr)
		{
			return false;
		}

		return *this == *reactionAttributed;
	}
}