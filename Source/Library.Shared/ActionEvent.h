#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionEvent final : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);

	public:
		/// <summary>
		/// The starting key for auxiliary attributes to this ActionEvent
		/// </summary>
		inline const static size_t AuxillaryKey = 5;

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor.
		/// </summary>
		ActionEvent();
		/// <summary>
		/// Constructor that sets prescribed attributes.
		/// </summary>
		/// <param name="name">The name of this ActionEvent</param>
		/// <param name="subtype">The string subtype of this ActionEvent</param>
		/// <param name="delay">The delay in milliseconds before the created event fires</param>
		explicit ActionEvent(const std::string& name, const std::string& subtype, int delay);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The ActionEvent being copied into this one</param>
		ActionEvent(const ActionEvent& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The ActionEvent being moved into this one</param>
		ActionEvent(ActionEvent && rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The ActionEvent being copied into this one</param>
		/// <returns>A reference to this ActionEvent after its been mutated</returns>
		ActionEvent& operator=(const ActionEvent & rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The ActionEvent being copied into this one</param>
		/// <returns>A reference to this ActionEvent after its been mutated</returns>
		ActionEvent& operator=(ActionEvent && rhs) = default;
		/// <summary>
		/// Default destructor
		/// </summary>
		~ActionEvent() = default;

		/// <summary>
		/// Creates an Attributed which it enqueues into the EventQueue to be fired after the given delay. 
		/// </summary>
		/// <param name="state">The current state of the world</param>
		void Update(WorldState& state) override;

		/// <summary>
		/// Creates and returns a clone of this ActionEvent.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this ActionEvent</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Checks whether or not two ActionEvent are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the ActionEvent object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	private:
		std::string mSubtype;
		int mDelay = 0;
	};

	ConcreteFactory(ActionEvent, Scope);
}

