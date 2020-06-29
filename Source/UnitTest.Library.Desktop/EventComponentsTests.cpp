#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "TypeManager.h"
#include "ActionIncrement.h"
#include "Entity.h"
#include "Avatar.h"
#include "Sector.h"
#include "WorldState.h"
#include "GameTime.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "JsonTableParseHelper.h"
#include "JsonParseMaster.h"
#include "ToStringSpecializations.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventComponentsTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			TypeManager::RegisterType(Entity::TypeIdClass(), Attributed::TypeIdClass(), Entity::GetSignatures());
			TypeManager::RegisterType(Sector::TypeIdClass(), Attributed::TypeIdClass(), Sector::GetSignatures());
			TypeManager::RegisterType(World::TypeIdClass(), Attributed::TypeIdClass(), World::GetSignatures());
			TypeManager::RegisterType(Avatar::TypeIdClass(), Entity::TypeIdClass(), Avatar::GetSignatures());
			TypeManager::RegisterType(Action::TypeIdClass(), Attributed::TypeIdClass(), Action::GetSignatures());
			TypeManager::RegisterType(ActionList::TypeIdClass(), Action::TypeIdClass(), ActionList::GetSignatures());
			TypeManager::RegisterType(ActionIncrement::TypeIdClass(), Action::TypeIdClass(), ActionIncrement::GetSignatures());
			TypeManager::RegisterType(EventMessageAttributed::TypeIdClass(), Attributed::TypeIdClass(), EventMessageAttributed::GetSignatures());
			TypeManager::RegisterType(Reaction::TypeIdClass(), ActionList::TypeIdClass(), Reaction::GetSignatures());
			TypeManager::RegisterType(ReactionAttributed::TypeIdClass(), Reaction::TypeIdClass(), ReactionAttributed::GetSignatures());
			TypeManager::RegisterType(ActionEvent::TypeIdClass(), Action::TypeIdClass(), ActionEvent::GetSignatures());

		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeManager::Clear();
			Event<EventMessageAttributed>::UnsubscribeAll();

#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(EventMessageAttributedTest)
		{
			World testWorld;
			EventMessageAttributed testMessage("TestType", testWorld);
			Assert::AreEqual(testMessage.Subtype(), "TestType"s);
			Assert::IsTrue(&testMessage.GetWorld() == &testWorld);

			testMessage.SetSubtype("TestType2");
			Assert::AreEqual(testMessage.Subtype(), "TestType2"s);

			World testWorld2;
			testMessage.SetWorld(testWorld2);
			Assert::IsTrue(&testMessage.GetWorld() == &testWorld2);
		}

		TEST_METHOD(ReactionAttributedConstuction)
		{
			ReactionAttributed reaction("Test", "TestType");
			Assert::IsTrue(reaction.IsPrescribedAttribute("Subtype"));
			Assert::AreEqual(reaction["Subtype"].GetString(), "TestType"s);
		}

		TEST_METHOD(ActionEventConstruction)
		{
			ActionEvent actionEvent("Test", "TestType", 30);
			Assert::AreEqual(actionEvent["Name"].GetString(), "Test"s);
			Assert::AreEqual(actionEvent["Subtype"].GetString(), "TestType"s);
			Assert::AreEqual(actionEvent["Delay"].GetInt(), 30);
		}

		TEST_METHOD(ActionEventFunctionality)
		{
			EntityFactory entityFactory;
			SectorFactory sectorFactory;
			ActionListFactory actionListFactory;
			ActionIncrementFactory actionIncrementFactory;
			ReactionAttributedFactory reactionAttributedFactory;
			ActionEventFactory actionEventFactory;

			World world;
			world.Update();
			JsonTableParseHelper::TableSharedData sharedData(&world);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.ParseFromFile("Content/ReactionTest.json");

			Assert::AreEqual(world.Sectors().Size(), 1_z);
			Sector* sector = static_cast<Sector*>(world.Sectors().GetScope());
			Assert::AreEqual(sector->Entities().Size(), 1_z);
			Entity* entity = static_cast<Entity*>(sector->Entities().GetScope());
			Assert::AreEqual(entity->Actions().Size(), 3_z);

			{
				Scope* scope = entity->Actions().GetScope(0);
				Assert::IsTrue(scope->Is(ActionEvent::TypeIdClass()));
			}

			{
				Scope* scope = entity->Actions().GetScope(1);
				Assert::IsTrue(scope->Is(ActionEvent::TypeIdClass()));
			}

			Assert::IsTrue(entity->IsAuxiliaryAttribute("Reaction"));

			Scope* scope = entity->operator[]("Reaction").GetScope(0);
			Assert::IsTrue(scope->Is(ReactionAttributed::TypeIdClass()));
			ReactionAttributed* reactionAttributed = static_cast<ReactionAttributed*>(scope);

			Scope* actionScope = reactionAttributed->Actions().GetScope(0);
			Assert::IsTrue(actionScope->Is(ActionIncrement::TypeIdClass()));
			ActionIncrement* actionIncrement = static_cast<ActionIncrement*>(actionScope);
			int& value = actionIncrement->operator[]("Value").GetInt();
			Assert::AreEqual(0, value);

			world.Update();

			Assert::AreEqual(entity->Actions().Size(), 1_z);
			Assert::AreEqual(0, value);

			world.Update();

			Assert::AreEqual(3, value);
		}

		TEST_METHOD(RTTITests)
		{
			{
				World testWorld;
				EventMessageAttributed eventMessage("TestType", testWorld);
				EventMessageAttributed* eventMessage2 = reinterpret_cast<EventMessageAttributed*>(eventMessage.Clone());
				EventMessageAttributed eventMessage3("FakeType", testWorld);

				Assert::IsTrue(eventMessage.Is(EventMessageAttributed::TypeIdClass()));
				Assert::IsTrue(eventMessage.Is(Attributed::TypeIdClass()));
				Assert::IsTrue(eventMessage.Is(Scope::TypeIdClass()));

				Assert::IsTrue(eventMessage.Equals(reinterpret_cast<RTTI*>(eventMessage2)));
				Assert::IsFalse(eventMessage.Equals(reinterpret_cast<RTTI*>(&eventMessage3)));

				Assert::AreEqual(eventMessage.ToString(), "EventMessageAttributed"s);

				delete eventMessage2;
			}

			{
				ActionEvent actionEvent;
				ActionEvent* actionEvent2 = reinterpret_cast<ActionEvent*>(actionEvent.Clone());
				ActionEvent actionEvent3("Test", "Test", 40);

				Assert::IsTrue(actionEvent.Is(ActionEvent::TypeIdClass()));
				Assert::IsTrue(actionEvent.Is(Action::TypeIdClass()));
				Assert::IsTrue(actionEvent.Is(Attributed::TypeIdClass()));
				Assert::IsTrue(actionEvent.Is(Scope::TypeIdClass()));

				Assert::IsTrue(actionEvent.Equals(reinterpret_cast<RTTI*>(actionEvent2)));
				Assert::IsFalse(actionEvent.Equals(reinterpret_cast<RTTI*>(&actionEvent3)));

				delete actionEvent2;
			}

			{
				ReactionAttributed reactionAttributed;
				ReactionAttributed* reactionAttributed2 = reinterpret_cast<ReactionAttributed*>(reactionAttributed.Clone());
				ReactionAttributed reactionAttributed3("Test", "TestType");

				Assert::IsTrue(reactionAttributed.Is(ReactionAttributed::TypeIdClass()));
				Assert::IsTrue(reactionAttributed.Is(Reaction::TypeIdClass()));
				Assert::IsTrue(reactionAttributed.Is(ActionList::TypeIdClass()));
				Assert::IsTrue(reactionAttributed.Is(Action::TypeIdClass()));
				Assert::IsTrue(reactionAttributed.Is(Attributed::TypeIdClass()));
				Assert::IsTrue(reactionAttributed.Is(Scope::TypeIdClass()));

				Assert::IsTrue(reactionAttributed.Equals(reinterpret_cast<RTTI*>(reactionAttributed2)));
				Assert::IsFalse(reactionAttributed.Equals(reinterpret_cast<RTTI*>(&reactionAttributed3)));

				delete reactionAttributed2;
			}
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventComponentsTests::sStartMemState;
}