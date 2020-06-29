#include "pch.h"
#include "ActionEvent.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include <memory>


using namespace std::chrono;

namespace Library
{
	RTTI_DEFINITIONS(ActionEvent);

	const Vector<Signature> ActionEvent::GetSignatures()
	{
		return Vector<Signature>
		{
			{ "Subtype"s, Datum::DatumTypes::String, true, 1, offsetof(ActionEvent, mSubtype) },
			{ "Delay"s, Datum::DatumTypes::Integer, true, 1, offsetof(ActionEvent, mDelay) }
		};
	}

	ActionEvent::ActionEvent() : Action(TypeIdClass()) {}

	ActionEvent::ActionEvent(const std::string& name, const std::string& subtype, int delay) :
		Action(TypeIdClass(), name), mSubtype(subtype), mDelay(delay)
	{
	}

	void ActionEvent::Update(WorldState& state)
	{
		if (state.World != nullptr)
		{
			std::shared_ptr<Event<EventMessageAttributed>> attributedEvent = std::make_shared<Event<EventMessageAttributed>>(EventMessageAttributed(mSubtype, *state.World));

			for (size_t i = AuxillaryKey; i < mOrderVector.Size(); ++i)
			{
				attributedEvent->mMessage.Append(mOrderVector[i]->first) = mOrderVector[i]->second;
			}

			state.World->GetEventQueue().Enqueue(attributedEvent, state.GetGameTime(), milliseconds(mDelay));
			state.World->MarkScopeForDelete(*this);
		}
	}

	gsl::owner<Scope*> ActionEvent::Clone() const
	{
		return new ActionEvent(*this);
	}

	bool ActionEvent::Equals(const RTTI* rhs) const
	{
		const ActionEvent* actionEvent = rhs->As<ActionEvent>();
		if (actionEvent == nullptr)
		{
			return false;
		}

		return *this == *actionEvent;
	}
}