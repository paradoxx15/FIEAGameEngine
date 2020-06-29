#pragma once
#include "Reaction.h"
#include "Factory.h"

namespace Library
{
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

	public:

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor.
		/// </summary>
		ReactionAttributed();
		/// <summary>
		/// Constructor that sets prescribed attributes.
		/// </summary>
		/// <param name="name">The name of this ReactionAttributed</param>
		/// <param name="subtype">The string subtype of this ReactionAttributed</param>
		explicit ReactionAttributed(const std::string& name, const std::string& subtype);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The ReactionAttributed being copied into this one</param>
		ReactionAttributed(const ReactionAttributed& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The ReactionAttributed being moved into this one</param>
		ReactionAttributed(ReactionAttributed && rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The ReactionAttributed being copied into this one</param>
		/// <returns>A reference to this ReactionAttributed after its been mutated</returns>
		ReactionAttributed& operator=(const ReactionAttributed & rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The ReactionAttributed being copied into this one</param>
		/// <returns>A reference to this ReactionAttributed after its been mutated</returns>
		ReactionAttributed& operator=(ReactionAttributed && rhs) = default;
		/// <summary>
		/// Default destructor. Unsubscribes itself from Event<EventMessageAttributed>
		/// </summary>
		~ReactionAttributed();

		/// <summary>
		/// Accepts AttributedEvents. If the event subtype matches the reaction subtype it will run this 
		/// ReactionAttributes ActionList update using the parameters stored on the passed in AttributedEvent
		/// </summary>
		/// <param name="payload">AttributedEvent reference that contains an event message and subtype</param>
		void Notify(const class EventPublisher& payload) override;

		/// <summary>
		/// Creates and returns a clone of this ReactionAttributed.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this ReactionAttributed</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Checks whether or not two ReactionAttributed are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the ReactionAttributed object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	private:
		/// <summary>
		/// A string representation of the subtype of AttributedEvents this reactions accepts
		/// </summary>
		std::string mSubtype;
	};

	ConcreteFactory(ReactionAttributed, Scope);
}

