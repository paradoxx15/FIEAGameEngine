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
	TEST_CLASS(EntityTests)
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
			{
				Entity ent;
				Assert::IsNull(ent.GetSector());
				Assert::IsTrue(ent.Name().empty());
				Assert::AreEqual(ent.Actions().Size(), 0_z);

				const Entity& constEnt = ent;
				Assert::IsNull(constEnt.GetSector());
				Assert::IsTrue(constEnt.Name().empty());
				Assert::AreEqual(constEnt.Actions().Size(), 0_z);
			}

			{
				Entity ent("Paul");
				Assert::IsNull(ent.GetSector());
				Assert::AreEqual(ent.Name(), "Paul"s);
				Assert::AreEqual(ent.Actions().Size(), 0_z);
			}

			{
				Avatar avatar;
				Assert::IsNull(avatar.GetSector());
				Assert::IsTrue(avatar.Name().empty());
				Assert::AreEqual(avatar.mUpdateCount, 0_z);
				Assert::AreEqual(avatar["Health"].GetInt(), 100);
				Assert::AreEqual(avatar.Actions().Size(), 0_z);
			}
		}
		
		TEST_METHOD(ParseAndUpdate)
		{
			ActionIncrementFactory actionIncrementFactory;

			Avatar avatar;
			JsonTableParseHelper::TableSharedData sharedData(&avatar);
			JsonParseMaster parseMaster(sharedData);
			JsonTableParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			parseMaster.ParseFromFile("Content/AvatarEntityTest.json");

			Assert::AreEqual(avatar.Name(), "Paul"s);
			Assert::AreEqual(avatar["Health"].GetInt(), 20);
			Assert::AreEqual(avatar["Actions"].Size(), 1_z);

			WorldState testState;
			Scope* actionIncrement = avatar["Actions"].GetScope();
			Assert::IsNotNull(actionIncrement);
			Assert::AreEqual(avatar.mUpdateCount, 0_z);
			Assert::AreEqual(avatar["Health"].GetInt(), 20);
			avatar.Update(testState);
			Assert::AreEqual(avatar.mUpdateCount, 1_z);
			Assert::AreEqual(avatar["Health"].GetInt(), 21);
			avatar.Update(testState);
			Assert::AreEqual(avatar.mUpdateCount, 2_z);
			Assert::AreEqual(avatar["Health"].GetInt(), 22);
		}

		TEST_METHOD(SetName)
		{
			Entity ent;
			Assert::IsTrue(ent.Name().empty());
			ent.SetName("Tom");
			Assert::AreEqual(ent.Name(), "Tom"s);
		}

		TEST_METHOD(SetSector)
		{
			Sector sector;
			Entity* ent = new Entity;
			Assert::IsNull(ent->GetSector());
			Assert::AreEqual(sector.Entities().Size(), 0_z);

			ent->SetSector(sector);
			Assert::AreEqual(sector.Entities().Size(), 1_z);
			Assert::AreEqual(ent->GetSector(), &sector);
		}

		TEST_METHOD(CreateAction)
		{
			ActionIncrementFactory actionIncrementFactory;

			Entity entity;
			entity.SetName("Paul");
			Scope* scope = entity.CreateAction("ActionIncrement", "Teach");
			Assert::IsNotNull(scope);
			Assert::IsTrue(scope->Is(ActionIncrement::TypeIdClass()));
			ActionIncrement* actionIncrement = static_cast<ActionIncrement*>(scope);
			Assert::AreEqual(actionIncrement->Name(), "Teach"s);
			Assert::IsTrue(actionIncrement->operator[]("Target").GetString().empty());
			Assert::AreEqual(actionIncrement->operator[]("Step").GetInt(), 1);
		}

		TEST_METHOD(RTTITests)
		{
			Entity ent("Tom");
			Entity* ent2 = reinterpret_cast<Entity*>(ent.Clone());
			Assert::AreEqual(ent.Name(), ent2->Name());

			Avatar avatar("Paul"), avatar3(200);
			Assert::IsTrue(avatar.Is(Avatar::TypeIdClass()));
			Assert::IsTrue(avatar.Is(Entity::TypeIdClass()));
			Assert::IsTrue(avatar.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(avatar.Is(Scope::TypeIdClass()));

			Avatar* avatar2 = static_cast<Avatar*>(avatar.Clone());

			Assert::IsTrue(avatar.Equals(reinterpret_cast<RTTI*>(avatar2)));
			Assert::IsFalse(avatar.Equals(reinterpret_cast<RTTI*>(&avatar3)));

			Assert::AreEqual(avatar.ToString(), "Paul"s);

			delete avatar2;
			delete ent2;
		}
		

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EntityTests::sStartMemState;
}