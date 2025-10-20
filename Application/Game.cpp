#include <Application/Game.h>
#include <GameLogic/Controllers/PlayerController.h>
#include <GameLogic/Controllers/RotatableController.h>

#include <Engine/Physics/PhysicsSystem.h>
#include <Engine/MovementSystem.h>
#include <Engine/AI/AISystem.h>

#include <World/AllBiomes.h>
#include <World/Objects/Player.h>
#include <World/Objects/AllCreatures.h>
#include <World/Objects/Interactives/Interactive.h>
#include <UI/HUD.h>
#include <UI/DebugInfoPanel.h>

#include <World/Arena.h>

Game::Game() :
	m_pPhysicsSystem(std::make_unique<PhysicsSystem>()),
	m_pMovementSystem(std::make_unique<MovementSystem>()),
	m_pAISystem(std::make_unique<AISystem>()),
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

	m_Scene.AddObject<Arena>({ -5.f, -10.f, -5.f });
	m_Scene.AddObject<DebugInfoPanel>({ });
	m_Scene.AddObject<UIHealthBar>({ });
	m_Scene.AddObject<UIArmorBar>({ });
	m_Scene.AddObject<Player>({ -0.f, 1.f, -0.f });
	m_Scene.AddObject<Zombie>({ -20.f, 1.f, -20.f });
	//m_Scene.AddObject<Orc>({ -30.f, 1.f, -30.f });
}

void Game::Update(const Float& deltaTime) {
	for (IController* pController : m_ActiveControllers)
		pController->Update(deltaTime);

	m_pAISystem->Update(deltaTime);
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
	if (event.pObject->GetType() == ObjectType::UI)
		return;

	if (event.pObject->GetType() == ObjectType::Player) {
		Player* pPlayer = static_cast<Player*>(event.pObject);

		AddController<PlayerController, Player>(pPlayer);
		AddController<RotatableController, GameObject>(pPlayer);

		ActivateController<PlayerController>();
		ActivateController<RotatableController>();

		BusLocator::StatisticsQuery().Subscribe<Player::Statistics>(
			[pPlayer] () {
				return pPlayer->GetStatistics();
			});
	}

	GameObject& object = *event.pObject;
	m_pAISystem->RegisterObject(object);
	m_pMovementSystem->RegisterObject(object);
	m_pPhysicsSystem->RegisterObject(object);
}

void Game::_HandleEventOnRemove(const Scene::OnRemove& event) {
	m_Dispatcher.Emmit<OnRemoveFromScene>({ event.pObject });
	if (event.pObject->GetType() == ObjectType::Canvas)
		return;
	if (event.pObject->GetType() == ObjectType::UI)
		return;

	if (event.pObject->GetType() == ObjectType::Player) {
		RemoveController<PlayerController>();
		RemoveController<RotatableController>();

		Player* pPlayer = static_cast<Player*>(event.pObject);
		BusLocator::StatisticsQuery().Unsubscribe<Player::Statistics>();
	}

	GameObject& object = *event.pObject;
	m_pAISystem->UnregisterObject(object);
	m_pMovementSystem->UnregisterObject(object);
	m_pPhysicsSystem->UnregisterObject(object);
}

void Game::_HandleEventOnClear(const Scene::OnClear& event) {
	(void)event;

	BusLocator::StatisticsQuery().Clear();
	m_Dispatcher.Emmit<OnClearScene>({ });

	m_ActiveControllers.clear();
	m_Controllers.clear();
	m_pPhysicsSystem->Clear();
}