#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "TypeManager.h"
#include "ActionIncrement.h"
#include "Avatar.h"
#include "Sector.h"
#include "WorldState.h"
#include "World.h"
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
	TEST_CLASS(WorldTests)
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
			TypeManager::RegisterType(ActionIncrement::TypeIdClass(), Action::TypeIdClass(), ActionIncrement::GetSignatures());
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

		TEST_METHOD(Construction)
		{
			World world;
			Assert::IsTrue(world.Name().empty());
			Assert::AreEqual(world.Sectors().Size(), 0_z);

			WorldState& state = world.GetWorldState();
			Assert::AreEqual(state.World, &world);
			Assert::IsNull(state.Sector);
			Assert::IsNull(state.Entity);
			Assert::IsNull(state.Action);

			const World& constWorld = world;
			Assert::IsTrue(constWorld.Name().empty());
			Assert::AreEqual(constWorld.Sectors().Size(), 0_z);

			const EventQueue& constEventQueue = constWorld.GetEventQueue();
			Assert::IsTrue(constEventQueue.IsEmpty());
		}

		TEST_METHOD(ParseConstruction)
		{
			EntityFactory entityFactory;
			AvatarFactory avatarFactory;
			SectorFactory sectorFactory;
			ActionIncrementFactory actionIncrementFactory;

			World world;
			JsonTableParseHelper::TableSharedData sharedData(&world);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.ParseFromFile("Content/WorldTest.json");

			Assert::AreEqual(world.Name(), "Florida"s);
			Assert::AreEqual(world.Sectors().Size(), 2_z);

			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(0))->Entities().Size(), 2_z);
			Assert::IsTrue(reinterpret_cast<Sector*>(world.Sectors().GetScope(0))->Entities().GetScope(0)->Is(Avatar::TypeIdClass()));
			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(0))->Entities().GetScope(0)->operator[]("Health").GetInt(), 300);
			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(0))->Entities().GetScope(0)->operator[]("Name").GetString(), "Paul"s);

			Assert::IsTrue(reinterpret_cast<Sector*>(world.Sectors().GetScope(0))->Entities().GetScope(1)->Is(Entity::TypeIdClass()));
			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(0))->Entities().GetScope(1)->operator[]("Name").GetString(), "Tom"s);

			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(1))->Entities().Size(), 1_z);
			Assert::IsTrue(reinterpret_cast<Sector*>(world.Sectors().GetScope(1))->Entities().GetScope()->Is(Avatar::TypeIdClass()));
			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(1))->Entities().GetScope()->operator[]("Name").GetString(), "Corona Virus"s);
		}

		TEST_METHOD(Update)
		{
			EntityFactory entityFactory;
			AvatarFactory avatarFactory;
			SectorFactory sectorFactory;
			ActionIncrementFactory actionIncrementFactory;

			World world;
			JsonTableParseHelper::TableSharedData sharedData(&world);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.ParseFromFile("Content/WorldTest.json");

			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(0))->Entities().GetScope(0)->As<Avatar>()->mUpdateCount, 0_z);
			Assert::AreEqual(world["Patients"].GetInt(), 0);
			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(1))->Entities().GetScope(0)->As<Avatar>()->mUpdateCount, 0_z);

			world.Update();

			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(0))->Entities().GetScope(0)->As<Avatar>()->mUpdateCount, 1_z);
			Assert::AreEqual(world["Patients"].GetInt(), 1);
			Assert::AreEqual(reinterpret_cast<Sector*>(world.Sectors().GetScope(1))->Entities().GetScope(0)->As<Avatar>()->mUpdateCount, 1_z);
		}

		TEST_METHOD(SetName)
		{
			World world;
			Assert::IsTrue(world.Name().empty());
			world.SetName("Florida");
			Assert::AreEqual(world.Name(), "Florida"s);
		}

		TEST_METHOD(CreateSector)
		{
			World world;
			world.SetName("Florida");
			Sector* sector = reinterpret_cast<Sector*>(world.CreateSector("FIEA"));
			Assert::IsNotNull(sector);
			Assert::AreEqual(sector->Name(), "FIEA"s);
			Assert::AreEqual(sector->Entities().Size(), 0_z);
			Assert::AreEqual(sector->GetWorld(), &world);
		}

		TEST_METHOD(RTTITests)
		{
			World world;
			world.SetName("Florida");
			Assert::AreEqual(world.ToString(), "Florida"s);

			World* world2 = reinterpret_cast<World*>(world.Clone());
			Assert::AreEqual(world.Name(), world2->Name());

			Assert::IsTrue(world.Is(World::TypeIdClass()));
			Assert::IsTrue(world.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(world.Is(Scope::TypeIdClass()));

			Assert::IsTrue(world.Equals(reinterpret_cast<RTTI*>(world2)));

			delete world2;
		}

		TEST_METHOD(WorldStateTest)
		{
			WorldState worldState;
			GameTime gameTime;
			worldState.SetGameTime(gameTime);
			Assert::IsTrue(worldState.GetGameTime().CurrentTime() == gameTime.CurrentTime());
			Assert::IsTrue(worldState.GetGameTime().ElapsedGameTime() == gameTime.ElapsedGameTime());
			Assert::IsTrue(worldState.GetGameTime().TotalGameTime() == gameTime.TotalGameTime());

			const WorldState& constWorldState = worldState;
			Assert::IsTrue(constWorldState.GetGameTime().TotalGameTime() == gameTime.TotalGameTime());

			const Stack<Scope>& argumentStack = constWorldState.GetArgumentStack();
			Assert::IsTrue(argumentStack.IsEmpty());

		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState WorldTests::sStartMemState;
}