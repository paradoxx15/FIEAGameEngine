#pragma once

#include "ActionList.h"
#include "EventSubscriber.h"

namespace Library
{
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);

	public:

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor.
		/// </summary>
		Reaction() = delete;
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The Reaction being copied into this one</param>
		Reaction(const Reaction& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The Reaction being moved into this one</param>
		Reaction(Reaction && rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The Reaction being copied into this one</param>
		/// <returns>A reference to this Reaction after its been mutated</returns>
		Reaction& operator=(const Reaction & rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The Reaction being copied into this one</param>
		/// <returns>A reference to this Reaction after its been mutated</returns>
		Reaction& operator=(Reaction && rhs) = default;
		/// <summary>
		/// Virtual destructor. Defaulted for the base class.
		/// </summary>
		virtual ~Reaction() = default;

		/// <summary>
		/// Overrides the basic ActionList update so that the reactions update does nothing
		/// </summary>
		/// <param name="state">The current world state</param>
		void Update(WorldState& state) override;

	protected:
		/// <summary>
		/// Explicit protected constructor that takes a passed in typeId to be passed to the base Attributed constructor.
		/// </summary>
		/// <param name="typeId">The typeId of the child class</param>
		explicit Reaction(RTTI::IdType typeId);
		/// <summary>
		/// Explicit protected constructor that takes a passed in typeId to be passed to the base Attributed constructor.
		/// </summary>
		/// <param name="typeId">The typeId of the child class</param>
		/// <param name="name">The name of this Reaction</param>
		explicit Reaction(RTTI::IdType typeId, const std::string& name);
	};
}

