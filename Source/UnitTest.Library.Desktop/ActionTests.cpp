#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "TypeManager.h"
#include "Avatar.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionListWhile.h"
#include "Sector.h"
#include "WorldState.h"
#include "World.h"
#include "JsonTableParseHelper.h"
#include "JsonParseMaster.h"
#include "ToStringSpecializations.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTests)
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
			TypeManager::RegisterType(Action::TypeIdClass(), Attributed::TypeIdClass(), Action::GetSignatures());
			TypeManager::RegisterType(ActionList::TypeIdClass(), Action::TypeIdClass(), ActionList::GetSignatures());
			TypeManager::RegisterType(ActionListWhile::TypeIdClass(), ActionList::TypeIdClass(), ActionListWhile::GetSignatures());
			TypeManager::RegisterType(ActionIncrement::TypeIdClass(), Action::TypeIdClass(), ActionIncrement::GetSignatures());
			TypeManager::RegisterType(ActionDestroyAction::TypeIdClass(), Action::TypeIdClass(), ActionDestroyAction::GetSignatures());
			TypeManager::RegisterType(ActionCreateAction::TypeIdClass(), Action::TypeIdClass(), ActionCreateAction::GetSignatures());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeManager::Clear();

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

		TEST_METHOD(ActionListConstruction)
		{
			{
				ActionList actionList;
				Assert::IsTrue(actionList.Name().empty());
				Assert::AreEqual(actionList.Actions().Size(), 0_z);

				const ActionList& constActionList = actionList;
				Assert::AreEqual(constActionList.Actions().Size(), 0_z);
				Assert::IsTrue(constActionList.Name().empty());
			}

			{
				ActionList actionList("Groceries");
				Assert::AreEqual(actionList.Name(), "Groceries"s);
				Assert::AreEqual(actionList.Actions().Size(), 0_z);

				const ActionList& constActionList = actionList;
				Assert::AreEqual(constActionList.Actions().Size(), 0_z);
				Assert::AreEqual(constActionList.Name(), "Groceries"s);
			}
		}

		TEST_METHOD(ActionIncrementConstruction)
		{
			{
				ActionIncrement actionIncrement;
				Assert::IsTrue(actionIncrement.Name().empty());
				Assert::IsTrue(actionIncrement["Target"].GetString().empty());
				Assert::AreEqual(actionIncrement["Step"].GetInt(), 1);

				const ActionIncrement& constActionIncrement = actionIncrement;
				Assert::IsTrue(constActionIncrement.Name().empty());
				Assert::IsTrue(constActionIncrement.Find("Target")->GetString().empty());
				Assert::AreEqual(constActionIncrement.Find("Step")->GetInt(), 1);
			}

			{
				ActionIncrement actionIncrement("Groceries");
				Assert::AreEqual(actionIncrement.Name(), "Groceries"s);
				Assert::IsTrue(actionIncrement["Target"].GetString().empty());
				Assert::AreEqual(actionIncrement["Step"].GetInt(), 1);

				const ActionIncrement& constActionIncrement = actionIncrement;
				Assert::AreEqual(constActionIncrement.Name(), "Groceries"s);
				Assert::IsTrue(constActionIncrement.Find("Target")->GetString().empty());
				Assert::AreEqual(constActionIncrement.Find("Step")->GetInt(), 1);
			}
		}

		TEST_METHOD(ActionCreateActionConstruction)
		{
			{
				ActionCreateAction actionCreate;
				Assert::IsTrue(actionCreate.Name().empty());
				Assert::IsTrue(actionCreate["Prototype"].GetString().empty());
				Assert::IsTrue(actionCreate["ActionName"].GetString().empty());

				const ActionCreateAction& constActionCreate = actionCreate;
				Assert::IsTrue(constActionCreate.Name().empty());
				Assert::IsTrue(constActionCreate.Find("Prototype")->GetString().empty());
				Assert::IsTrue(constActionCreate.Find("ActionName")->GetString().empty());
			}

			{
				ActionCreateAction actionCreate("Groceries");
				Assert::AreEqual(actionCreate.Name(), "Groceries"s);
				Assert::IsTrue(actionCreate["Prototype"].GetString().empty());
				Assert::IsTrue(actionCreate["ActionName"].GetString().empty());

				const ActionCreateAction& constActionCreate = actionCreate;
				Assert::AreEqual(constActionCreate.Name(), "Groceries"s);
				Assert::IsTrue(constActionCreate.Find("Prototype")->GetString().empty());
				Assert::IsTrue(constActionCreate.Find("ActionName")->GetString().empty());
			}
		}

		TEST_METHOD(ActionDestroyActionConstruction)
		{
			{
				ActionDestroyAction actionDestroy;
				Assert::IsTrue(actionDestroy.Name().empty());
				Assert::IsTrue(actionDestroy["Action"].GetString().empty());

				const ActionDestroyAction& constActionDestroy = actionDestroy;
				Assert::IsTrue(constActionDestroy.Name().empty());
				Assert::IsTrue(constActionDestroy.Find("Action")->GetString().empty());
			}

			{
				ActionDestroyAction actionDestroy("Groceries");
				Assert::AreEqual(actionDestroy.Name(), "Groceries"s);
				Assert::IsTrue(actionDestroy["Action"].GetString().empty());

				const ActionDestroyAction& constActionDestroy = actionDestroy;
				Assert::AreEqual(constActionDestroy.Name(), "Groceries"s);
				Assert::IsTrue(constActionDestroy.Find("Action")->GetString().empty());
			}
		}

		TEST_METHOD(ActionListWhileConstruction)
		{
			{
				ActionListWhile actionListWhile;
				Assert::IsTrue(actionListWhile.Name().empty());
				Assert::IsTrue(actionListWhile["Condition"].GetString().empty());

				const ActionListWhile& constActionListWhile = actionListWhile;
				Assert::IsTrue(constActionListWhile.Name().empty());
				Assert::IsTrue(constActionListWhile.Find("Condition")->GetString().empty());
			}

			{
				ActionListWhile actionListWhile("Groceries");
				Assert::AreEqual(actionListWhile.Name(), "Groceries"s);
				Assert::IsTrue(actionListWhile["Condition"].GetString().empty());

				const ActionListWhile& constActionListWhile = actionListWhile;
				Assert::AreEqual(constActionListWhile.Name(), "Groceries"s);
				Assert::IsTrue(constActionListWhile.Find("Condition")->GetString().empty());
			}
		}

		TEST_METHOD(ActionListAndActionIncrementParseAndUpdate)
		{
			ActionListFactory actionListFactory;
			ActionIncrementFactory actionIncrementFactory;

			ActionList actionList;
			JsonTableParseHelper::TableSharedData sharedData(&actionList);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.ParseFromFile("Content/ActionTest.json");

			Assert::AreEqual(actionList.Name(), "Groceries"s);
			Assert::AreEqual(actionList.Actions().Size(), 1_z);

			Scope* scope = actionList.Actions().GetScope(0);
			Assert::IsTrue(scope->Is(ActionIncrement::TypeIdClass()));
			ActionIncrement* actionIncrement = static_cast<ActionIncrement*>(scope);
			Assert::AreEqual(actionIncrement->operator[]("Value").GetInt(), 0);
			Assert::AreEqual(actionIncrement->operator[]("Step").GetInt(), 1);

			WorldState worldState;
			actionList.Update(worldState);
			Assert::AreEqual(actionIncrement->operator[]("Value").GetInt(), 1);
			actionList.Update(worldState);
			Assert::AreEqual(actionIncrement->operator[]("Value").GetInt(), 2);
			actionList.Update(worldState);
			Assert::AreEqual(actionIncrement->operator[]("Value").GetInt(), 3);
		}

		TEST_METHOD(ActionCreateDestroyFunctionality)
		{
			EntityFactory entityFactory;
			SectorFactory sectorFactory;
			ActionListFactory actionListFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionCreateActionFactory actionCreateFactory;
			ActionDestroyActionFactory actionDestroyFactory;

			World world;
			JsonTableParseHelper::TableSharedData sharedData(&world);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.ParseFromFile("Content/ActionCreateDestroyTest.json");

			Assert::AreEqual(world.Sectors().Size(), 1_z);
			Sector* sector = static_cast<Sector*>(world.Sectors().GetScope());
			Assert::AreEqual(sector->Entities().Size(), 1_z);
			Entity* entity = static_cast<Entity*>(sector->Entities().GetScope());
			Assert::AreEqual(entity->Actions().Size(), 3_z);

			Assert::AreEqual(entity->Name(), "Paul"s);
			Assert::AreEqual(entity->Actions().Size(), 3_z);

			{
				Scope* scope = entity->Actions().GetScope(0);
				Assert::IsTrue(scope->Is(ActionCreateAction::TypeIdClass()));
			}

			{
				Scope* scope = entity->Actions().GetScope(1);
				Assert::IsTrue(scope->Is(ActionIncrement::TypeIdClass()));
			}

			{
				Scope* scope = entity->Actions().GetScope(2);
				Assert::IsTrue(scope->Is(ActionDestroyAction::TypeIdClass()));
			}

			world.Update();

			Assert::AreEqual(entity->Actions().Size(), 1_z);
			Scope* scope = entity->Actions().GetScope(0);
			Assert::IsTrue(scope->Is(ActionIncrement::TypeIdClass()));
			ActionIncrement* actionIncrement = static_cast<ActionIncrement*>(scope);
			Assert::AreEqual(actionIncrement->Find("Target")->GetString(), "Value"s);
			int test2 = actionIncrement->Find("Value")->GetInt();
			test2;
			Datum* test = actionIncrement->Find("Value");
			test;
			Assert::AreEqual(actionIncrement->Find("Value")->GetInt(), 2);
			Assert::AreEqual(actionIncrement->Find("Step")->GetInt(), 2);

			world.Update();
			Assert::AreEqual(entity->Actions().Size(), 1_z);
			int test3 = actionIncrement->Find("Value")->GetInt();
			test3;
			Assert::AreEqual(actionIncrement->Find("Value")->GetInt(), 4);
		}

		TEST_METHOD(ActionListWhileFunctionality)
		{
			ActionListFactory actionListFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionListWhileFactory actionListWhileFactory;

			ActionListWhile actionWhile;
			JsonTableParseHelper::TableSharedData sharedData(&actionWhile);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.ParseFromFile("Content/ActionListWhileTest.json");

			Assert::AreEqual(actionWhile.Name(), "While"s);
			Assert::AreEqual(actionWhile.Actions().Size(), 1_z);
			Assert::IsTrue(actionWhile.IsAttribute("Preamble"));
			Assert::IsTrue(actionWhile.IsAttribute("Postamble"));

			Datum& testCondition = actionWhile["TestCondition"];
			Datum& value = actionWhile["Value"];

			WorldState worldState;
			actionWhile.Update(worldState);
			Assert::AreEqual(testCondition.GetInt(), 0);
			Assert::AreEqual(value.GetInt(), 2);
			actionWhile.Update(worldState);
			Assert::AreEqual(testCondition.GetInt(), 0);
			Assert::AreEqual(value.GetInt(), 4);
		}

		TEST_METHOD(SetName)
		{
			ActionList action;
			Assert::IsTrue(action.Name().empty());
			action.SetName("FIEA");
			Assert::AreEqual(action.Name(), "FIEA"s);
		}

		TEST_METHOD(ActionListRTTITests)
		{
			ActionList actionList("Groceries");
			Assert::AreEqual(actionList.ToString(), "Groceries"s);

			ActionList* actionList2 = reinterpret_cast<ActionList*>(actionList.Clone());
			Assert::IsNotNull(actionList2);
			Assert::AreEqual(actionList.Name(), actionList2->Name());

			Assert::IsTrue(actionList.Is(ActionList::TypeIdClass()));
			Assert::IsTrue(actionList.Is(Action::TypeIdClass()));
			Assert::IsTrue(actionList.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(actionList.Is(Scope::TypeIdClass()));

			Assert::IsTrue(actionList.Equals(reinterpret_cast<RTTI*>(actionList2)));

			delete actionList2;
		}

		TEST_METHOD(ActionIncrementRTTITests)
		{
			ActionIncrement actionIncrement("Weapons");
			Assert::AreEqual(actionIncrement.ToString(), "Weapons"s);

			ActionIncrement* actionIncrement2 = reinterpret_cast<ActionIncrement*>(actionIncrement.Clone());
			Assert::IsNotNull(actionIncrement2);
			Assert::AreEqual(actionIncrement.Name(), actionIncrement2->Name());

			Assert::IsTrue(actionIncrement.Is(ActionIncrement::TypeIdClass()));
			Assert::IsTrue(actionIncrement.Is(Action::TypeIdClass()));
			Assert::IsTrue(actionIncrement.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(actionIncrement.Is(Scope::TypeIdClass()));

			Assert::IsTrue(actionIncrement.Equals(reinterpret_cast<RTTI*>(actionIncrement2)));

			delete actionIncrement2;
		}

		TEST_METHOD(ActionCreateActionRTTITests)
		{
			ActionCreateAction actionCreateAction("Weapons");
			Assert::AreEqual(actionCreateAction.ToString(), "Weapons"s);

			ActionCreateAction* actionCreateAction2 = reinterpret_cast<ActionCreateAction*>(actionCreateAction.Clone());
			Assert::IsNotNull(actionCreateAction2);
			Assert::AreEqual(actionCreateAction.Name(), actionCreateAction2->Name());

			Assert::IsTrue(actionCreateAction.Is(ActionCreateAction::TypeIdClass()));
			Assert::IsTrue(actionCreateAction.Is(Action::TypeIdClass()));
			Assert::IsTrue(actionCreateAction.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(actionCreateAction.Is(Scope::TypeIdClass()));

			Assert::IsTrue(actionCreateAction.Equals(reinterpret_cast<RTTI*>(actionCreateAction2)));

			delete actionCreateAction2;
		}

		TEST_METHOD(ActionDestroyActionRTTITests)
		{
			ActionDestroyAction actionDestroyAction("Weapons");
			Assert::AreEqual(actionDestroyAction.ToString(), "Weapons"s);

			ActionDestroyAction* actionDestroyAction2 = reinterpret_cast<ActionDestroyAction*>(actionDestroyAction.Clone());
			Assert::IsNotNull(actionDestroyAction2);
			Assert::AreEqual(actionDestroyAction.Name(), actionDestroyAction2->Name());

			Assert::IsTrue(actionDestroyAction.Is(ActionDestroyAction::TypeIdClass()));
			Assert::IsTrue(actionDestroyAction.Is(Action::TypeIdClass()));
			Assert::IsTrue(actionDestroyAction.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(actionDestroyAction.Is(Scope::TypeIdClass()));

			Assert::IsTrue(actionDestroyAction.Equals(reinterpret_cast<RTTI*>(actionDestroyAction2)));

			delete actionDestroyAction2;
		}

		TEST_METHOD(ActionListWhileRTTITests)
		{
			ActionListWhile actionListWhile("Weapons");
			Assert::AreEqual(actionListWhile.ToString(), "Weapons"s);

			ActionListWhile* actionListWhile2 = reinterpret_cast<ActionListWhile*>(actionListWhile.Clone());
			Assert::IsNotNull(actionListWhile2);
			Assert::AreEqual(actionListWhile.Name(), actionListWhile2->Name());

			Assert::IsTrue(actionListWhile.Is(ActionListWhile::TypeIdClass()));
			Assert::IsTrue(actionListWhile.Is(ActionList::TypeIdClass()));
			Assert::IsTrue(actionListWhile.Is(Action::TypeIdClass()));
			Assert::IsTrue(actionListWhile.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(actionListWhile.Is(Scope::TypeIdClass()));

			Assert::IsTrue(actionListWhile.Equals(reinterpret_cast<RTTI*>(actionListWhile2)));

			delete actionListWhile2;
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionTests::sStartMemState;
}