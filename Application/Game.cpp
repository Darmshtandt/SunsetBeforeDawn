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
