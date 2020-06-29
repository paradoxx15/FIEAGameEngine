#include "pch.h"
#include "WorldState.h"

namespace Library
{
	GameTime& WorldState::GetGameTime()
	{
		return mGameTime;
	}

	const GameTime& WorldState::GetGameTime() const
	{
		return mGameTime;
	}

	void WorldState::SetGameTime(GameTime& gameTime)
	{
		mGameTime = gameTime;
	}

	Stack<Scope>& WorldState::GetArgumentStack()
	{
		return mArgumentStack;
	}

	const Stack<Scope>& WorldState::GetArgumentStack() const
	{
		return mArgumentStack;
	}
}