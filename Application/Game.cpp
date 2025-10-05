#include <Game.h>
#include <Controllers/PlayerController.h>
#include <Controllers/RotatableController.h>

#include <Objects/NPC/Behaviors/BehaviorSystem.h>
#include <Objects/NPC/TargetingSystem.h>
#include <Physics/PhysicsSystem.h>
#include <MovementSystem.h>

#include <World/Biomes/AllBiomes.h>
#include <Objects/Actors/Player.h>
#include <Objects/Creatures/AllCreatures.h>
#include <Objects/Interactives/Interactive.h>
#include <UI/HUD.h>

Game::Game() :
	m_pPhysicsSystem(std::make_unique<PhysicsSystem>()),
	m_pMovementSystem(std::make_unique<MovementSystem>()),
	m_pBehaviorSystem(std::make_unique<BehaviorSystem>()),
	m_pTargetingSystem(std::make_unique<TargetingSystem>()),
	m_pSceneDispatcher(m_Scene.GetDispatcherPtr())
{
}

Game::~Game() noexcept = default;

void Game::Initialize() {
	m_pSceneDispatcher->Subscribe<Scene::OnAdd>(
		[this] (const Scene::OnAdd& event) {
			_HandleEventOnAdd(event);
		});

	m_pSceneDispatcher->Subscribe<Scene::OnRemove>(
		[this] (const Scene::OnRemove& event) {
			_HandleEventOnRemove(event);
		});

	m_pSceneDispatcher->Subscribe<Scene::OnClear>(
		[this] (const Scene::OnClear& event) {
			_HandleEventOnClear(event);
		});

	m_Scene.Initialize();
	m_Scene.Generate();

	m_Scene.AddObject<HealthBar>({ });
	m_Scene.AddObject<ArmorBar>({ });
	m_Scene.AddObject<Player>({ -5.f, 1.f, -5.f });
	m_Scene.AddObject<Zombie>({ -20.f, 1.f, -20.f });
	m_Scene.AddObject<Orc>({ -30.f, 1.f, -30.f });
}

void Game::Update(const Float& deltaTime) {
	for (IController* pController : m_ActiveControllers)
		pController->Update(deltaTime);

	m_pTargetingSystem->Update(deltaTime);
	m_Scene.Update(deltaTime);
	m_pMovementSystem->Update(deltaTime);
	m_pPhysicsSystem->Update(deltaTime);
}

Scene& Game::GetScene() noexcept {
	return m_Scene;
}

EventBus& Game::GetDispatcher() noexcept {
	return m_Dispatcher;
}

void Game::ActivateController(const ControllerID& id) {
	ControllerPtr& pController = _GetController(id);
	m_ActiveControllers.insert(pController.get());
}

void Game::DeativateController(const ControllerID& id) {
	IController* pController = _GetController(id).get();
		
	const auto it = m_ActiveControllers.find(pController);
	if (it != m_ActiveControllers.cend())
		m_ActiveControllers.erase(pController);
}

void Game::ToggleController(const ControllerID& id) {
	IController* pController = _GetController(id).get();

	const auto it = m_ActiveControllers.find(pController);
	if (it != m_ActiveControllers.cend())
		m_ActiveControllers.erase(pController);
	else
		m_ActiveControllers.insert(pController);
}

ControllerPtr& Game::_GetController(const ControllerID& id) {
	auto it = m_Controllers.find(id);
	Assert(it != m_Controllers.cend(), "Unknown controller");
	return it->second;
}

void Game::_HandleEventOnAdd(const Scene::OnAdd& event) {
	m_Dispatcher.Emmit<OnAddToScene>({ event.pObject });
	if (event.pObject->GetType() == ObjectType::Canvas)
		return;

	if (event.pObject->GetType() == ObjectType::Player) {
		Player* pPlayer = static_cast<Player*>(event.pObject);

		AddController<PlayerController, Player>(pPlayer);
		AddController<RotatableController, GameObject>(pPlayer);

		ActivateController<PlayerController>();
		ActivateController<RotatableController>();

		Bus::g_StatisticsQuery.Subscribe<Player::Statistics>(
			[&pPlayer] () {
				return pPlayer->GetStatistics();
			});
	}
	else if (event.pObject->GetType() == ObjectType::Creature) {
		Creature* pCreature = static_cast<Creature*>(event.pObject);

		m_pBehaviorSystem->EvaluateBehavior(pCreature);
	}

	GameObject& object = *event.pObject;
	m_pTargetingSystem->RegisterObject(object);
	m_pMovementSystem->RegisterObject(object);
	m_pPhysicsSystem->RegisterObject(object);
}

void Game::_HandleEventOnRemove(const Scene::OnRemove& event) {
	m_Dispatcher.Emmit<OnRemoveFromScene>({ event.pObject });
	if (event.pObject->GetType() == ObjectType::Canvas)
		return;

	if (event.pObject->GetType() == ObjectType::Player) {
		RemoveController<PlayerController>();
		RemoveController<RotatableController>();

		Player* pPlayer = static_cast<Player*>(event.pObject);
		Bus::g_StatisticsQuery.Unsubscribe<Player::Statistics>();
	}

	GameObject& object = *event.pObject;
	m_pTargetingSystem->UnregisterObject(object);
	m_pMovementSystem->UnregisterObject(object);
	m_pPhysicsSystem->UnregisterObject(object);
}

void Game::_HandleEventOnClear(const Scene::OnClear& event) {
	(void)event;

	Bus::g_StatisticsQuery.Clear();
	m_Dispatcher.Emmit<OnClearScene>({ });

	m_ActiveControllers.clear();
	m_Controllers.clear();
	m_pPhysicsSystem->Clear();
}