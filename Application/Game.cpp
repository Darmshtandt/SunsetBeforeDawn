#include <Game.h>

Game::Game() :
	m_Camera(nullptr)
{
	EventBus::Instance().Subscribe<IController::OnActivateController>(
		[this] (const IController::OnActivateController& event) {
			ActivateController(event.ID);
		});

	EventBus::Instance().Subscribe<IController::OnDeativateController>(
		[this] (const IController::OnDeativateController& event) {
			DeativateController(event.ID);
		});

	EventBus::Instance().Subscribe<IController::OnToggleController>(
		[this] (const IController::OnToggleController& event) {
			ToggleController(event.ID);
		});
}

Game::~Game() {
	Uninitialize();
}

void Game::Initialize(Nt::Shader* pShader) {
	EventBus::Instance().Subscribe<Scene::OnAdd>(
		[this] (const Scene::OnAdd& event) {
			_HandleEventOnAdd(event);
		});

	EventBus::Instance().Subscribe<Scene::OnRemove>(
		[this] (const Scene::OnRemove& event) {
			_HandleEventOnRemove(event);
		});

	EventBus::Instance().Subscribe<Scene::OnClear>(
		[this] (const Scene::OnClear& event) {
			_HandleEventOnClear(event);
		});

	m_Scene.Initialize(pShader);
	m_Scene.Generate();

	m_Scene.Add<Player>({ -5.f, 1.f, -5.f });
	m_Scene.Add<Zombie>({ -20.f, 1.f, -20.f });
	m_Scene.Add<Orc>({ -30.f, 1.f, -30.f });
}

void Game::Uninitialize() {
	m_ActiveControllers.clear();

	for (auto it = m_Controllers.cbegin(); it != m_Controllers.cend();) {
		EventBus::Instance().Emmit<OnUnregisterInput>({ it->second.get() });
		it = m_Controllers.erase(it);
	}
}

void Game::Update(const Float& deltaTime) {
	for (IController* pController : m_ActiveControllers)
		pController->Update(deltaTime);

	m_TargetingSystem.Update(deltaTime);
	m_Scene.Update(deltaTime);
	m_Camera.Update();
	m_PhysicsSystem.Update(deltaTime);
}

void Game::Render(Nt::Renderer* pRenderer) const {
	m_Scene.Render(pRenderer);
}

CameraObject* Game::GetCameraPtr() noexcept {
	return &m_Camera;
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
	if (event.pObject->GetType() == ObjectType::Player) {
		Player* pPlayer = static_cast<Player*>(event.pObject);

		AddController<MovableController, Movable>(pPlayer);
		AddController<RotatableController, Movable>(pPlayer);

		ActivateController<MovableController>();
		ActivateController<RotatableController>();
		m_Camera.SetTarget(pPlayer);
	}
	else if (event.pObject->GetType() == ObjectType::Creature) {
		Creature* pCreature = static_cast<Creature*>(event.pObject);

		m_BehaviorSystem.EvaluateBehavior(pCreature);
		m_TargetingSystem.RegisterCreature(m_Scene, *pCreature);
	}

	m_PhysicsSystem.AddObject(event.pObject);		
}

void Game::_HandleEventOnRemove(const Scene::OnRemove& event) {
	if (event.pObject->GetType() == ObjectType::Player) {
		RemoveController<MovableController>();
		RemoveController<RotatableController>();
		m_Camera.SetTarget(nullptr);
	}

	m_PhysicsSystem.RemoveObject(event.pObject);
}

void Game::_HandleEventOnClear(const Scene::OnClear& event) {
	(void)event;

	m_ActiveControllers.clear();
	m_Controllers.clear();
	m_PhysicsSystem.Clear();
	m_Camera.SetTarget(nullptr);		
}
