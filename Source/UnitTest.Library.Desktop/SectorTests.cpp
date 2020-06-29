#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "TypeManager.h"
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
	TEST_CLASS(SectorTests)
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
				Sector sector;
				Assert::IsNull(sector.GetWorld());
				Assert::IsTrue(sector.Name().empty());
				Assert::AreEqual(sector.Entities().Size(), 0_z);

				const Sector& constSector = sector;
				Assert::AreEqual(constSector.Entities().Size(), 0_z);
				Assert::IsTrue(constSector.Name().empty());
		}

		TEST_METHOD(ParseConstruction)
		{
			EntityFactory entityFactory;
			AvatarFactory avatarFactory;

			Sector sector;
			JsonTableParseHelper::TableSharedData sharedData(&sector);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.ParseFromFile("Content/SectorTest.json");

			Assert::AreEqual(sector.Name(), "FIEA"s);
			Assert::AreEqual(sector.Entities().Size(), 2_z);

			Assert::IsTrue(sector.Entities().GetScope(0)->Is(Avatar::TypeIdClass()));
			Assert::AreEqual(sector.Entities().GetScope(0)->operator[]("Health").GetInt(), 300);
			Assert::AreEqual(sector.Entities().GetScope(0)->operator[]("Name").GetString(), "Paul"s);

			Assert::IsTrue(sector.Entities().GetScope(1)->Is(Entity::TypeIdClass()));
			Assert::AreEqual(sector.Entities().GetScope(1)->operator[]("Name").GetString(), "Tom"s);
		}

		TEST_METHOD(SetName)
		{
			Sector sector;
			Assert::IsTrue(sector.Name().empty());
			sector.SetName("FIEA");
			Assert::AreEqual(sector.Name(), "FIEA"s);
		}

		TEST_METHOD(SetWorld)
		{
			World world;
			Sector* sector = new Sector;
			Assert::IsNull(sector->GetWorld());
			Assert::AreEqual(world.Sectors().Size(), 0_z);

			sector->SetWorld(world);
			Assert::AreEqual(world.Sectors().Size(), 1_z);
			Assert::AreEqual(sector->GetWorld(), &world);
		}

		TEST_METHOD(UpdateAndCreateEntity)
		{
			EntityFactory entityFactory;
			AvatarFactory avatarFactory;

			WorldState testState;
			Sector sector;
			sector.SetName("FIEA");

			Assert::ExpectException<std::runtime_error>([&sector] { reinterpret_cast<Avatar*>(sector.CreateEntity("AttributedFoo", "Paul")); });

			Avatar* avatar = reinterpret_cast<Avatar*>(sector.CreateEntity("Avatar", "Paul"));
			Assert::IsNotNull(avatar);
			Assert::AreEqual(avatar->Name(), "Paul"s);
			Assert::AreEqual(avatar->Find("Health")->GetInt(), 100);
			Assert::AreEqual(avatar->GetSector(), &sector);

			Assert::AreEqual(avatar->mUpdateCount, 0_z);
			sector.Update(testState);
			Assert::AreEqual(avatar->mUpdateCount, 1_z);
		}

		TEST_METHOD(RTTITests)
		{
			Sector sector;
			sector.SetName("FIEA");
			Assert::AreEqual(sector.ToString(), "FIEA"s);

			Sector* sector2 = reinterpret_cast<Sector*>(sector.Clone());
			Assert::IsNotNull(sector2);

			Assert::AreEqual(sector.Name(), sector2->Name());

			Assert::IsTrue(sector.Is(Sector::TypeIdClass()));
			Assert::IsTrue(sector.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(sector.Is(Scope::TypeIdClass()));

			Assert::IsTrue(sector.Equals(reinterpret_cast<RTTI*>(sector2)));

			delete sector2;
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SectorTests::sStartMemState;
}