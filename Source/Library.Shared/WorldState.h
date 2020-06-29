#pragma once
#include "GameTime.h"
#include "EventQueue.h"
#include "Stack.h"
#include "Scope.h"

namespace Library
{
	class WorldState final
	{
	public:

		/// <summary>
		/// Returns the GameTime held within this WorldState
		/// </summary>
		/// <returns>A GameTime reference</returns>
		GameTime& GetGameTime();
		/// <summary>
		/// Returns a const reference to the GameTime held within this WorldState
		/// </summary>
		/// <returns>A const GameTime reference</returns>
		const GameTime& GetGameTime() const;
		/// <summary>
		/// Sets the GameTime for this world state
		/// </summary>
		/// <param name="gameTime">The GameTime that the member variable GameTime is being set to</param>
		void SetGameTime(GameTime& gameTime);

		/// <summary>
		/// Returns the ArgumentStack of scopes.
		/// </summary>
		/// <returns>Reference to the ArgumentStack</returns>
		Stack<Scope>& GetArgumentStack();
		/// <summary>
		/// Returns the ArgumentStack of scopes.
		/// </summary>
		/// <returns>Const reference to the ArgumentStack</returns>
		const Stack<Scope>& GetArgumentStack() const;

		/// <summary>
		/// Holds a pointer to the current World object
		/// </summary>
		class World* World = nullptr;
		/// <summary>
		/// Holds a pointer to the current Sector object
		/// </summary>
		class Sector* Sector = nullptr;
		/// <summary>
		/// Holds a pointer to the current Entity object
		/// </summary>
		class Entity* Entity = nullptr;
		/// <summary>
		/// Holds a pointer to the current Action object
		/// </summary>
		class Action* Action = nullptr;

	private:
		/// <summary>
		/// The GameTime used to manage time within the world
		/// </summary>
		GameTime mGameTime;

		/// <summary>
		/// Stack that holds scopes that contain arguments being passed between attributed objects
		/// </summary>
		Stack<Scope> mArgumentStack;
	};
}

