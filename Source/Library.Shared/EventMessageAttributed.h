#pragma once

#include "Attributed.h"
#include "World.h"

namespace Library
{
	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);

	public:

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor. Takes in a string subtype and a reference to the world this message lives in.
		/// </summary>
		/// <param name="subtype">Indicates the subtype of this event</param>
		/// <param name="world">The world this event lives in</param>
		EventMessageAttributed(const std::string& subtype, World& world);
		/// <summary>
		/// Default constructor with R-value string. Takes in a string subtype and a reference to the world this message lives in.
		/// </summary>
		/// <param name="subtype">Indicates the subtype of this event</param>
		/// <param name="world">The world this event lives in</param>
		EventMessageAttributed(std::string&& subtype, World& world);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The EventMessageAttributed being copied into this one</param>
		EventMessageAttributed(const EventMessageAttributed& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The EventMessageAttributed being moved into this one</param>
		EventMessageAttributed(EventMessageAttributed && rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The EventMessageAttributed being copied into this one</param>
		/// <returns>A reference to this EventMessageAttributed after its been mutated</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed & rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The EventMessageAttributed being copied into this one</param>
		/// <returns>A reference to this EventMessageAttributed after its been mutated</returns>
		EventMessageAttributed& operator=(EventMessageAttributed && rhs) = default;
		/// <summary>
		/// Default destructor
		/// </summary>
		~EventMessageAttributed() = default;

		/// <summary>
		/// Gets the subtype of this event message
		/// </summary>
		/// <returns>A const reference to the string subtype of this message</returns>
		const std::string& Subtype() const;
		/// <summary>
		/// Sets the subtype of this message.
		/// </summary>
		/// <param name="subtype">The subtype this message is being set to.</param>
		void SetSubtype(const std::string& subtype);

		/// <summary>
		/// Returns a reference to the world this event message lives in.
		/// </summary>
		/// <returns>A world reference</returns>
		World& GetWorld();
		/// <summary>
		/// Returns a const reference to the world this event message lives in.
		/// </summary>
		/// <returns>A const world reference</returns>
		const World& GetWorld() const;
		/// <summary>
		/// Sets the world that this event message lives in
		/// </summary>
		/// <param name="world">The world this message lives in</param>
		void SetWorld(World& world);

		/// <summary>
		/// Creates and returns a clone of this EventMessageAttributed.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this EventMessageAttributed</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Returns a string representation of the EventMessageAttributed.
		/// </summary>
		/// <returns>A string representation of the EventMessageAttributed</returns>
		std::string ToString() const override;

		/// <summary>
		/// Checks whether or not two EventMessageAttributed are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the EventMessageAttributed object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	private:
		/// <summary>
		/// A string representation of the type of event message this is.
		/// </summary>
		std::string mSubtype;
		/// <summary>
		/// A pointer to the world this message lives within.
		/// </summary>
		World* mWorld;
	};
}

