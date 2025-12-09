#include <NtStdH.h>

#include <Application/Application.h>
#include <Core/Helpers/Profiler.h>
#include <Core/StdH.h>

#include <Nt/Graphics/Geometry/Primitives.h>
#include <Nt/Graphics/Resources/ResourceManager.h>
#include <Core/BusLocator.h>

Application::Listener::Listener(NotNull<Application*> pApplication) noexcept :
	m_pApp(pApplication)
{
}

Application::WindowListener::WindowListener(NotNull<Application*> pApplication) noexcept :
	Listener(pApplication)
{
}

void Application::WindowListener::Resize(const Nt::ResizeType& type, const Nt::Int2D& size) {
	(void)type;
	(void)size;

	const Nt::FloatRect viewport = m_pApp->m_Window.GetViewportRect();

	m_pApp->m_RenderSystem->Resize(viewport.RightBottom);
	m_pApp->m_UISystem.Resize(viewport.RightBottom);
}

Application::KeyboardListener::KeyboardListener(NotNull<Application*> pApplication) noexcept :
	Listener(pApplication)
{
}

void Application::KeyboardListener::KeyReleased(const Nt::Key& key) {
	if (key == Nt::KEY_ESCAPE) {
		m_pApp->m_InputCollector.Unregister(&m_pApp->m_Window);
		m_pApp->m_Window.Close();
	}
}

Application::Application() :
	m_Window(true),
	m_WindowListener(this),
	m_KeyboardListener(this),
	m_Game(&m_GameDispatcher)
{
	m_GameDispatcher.Subscribe<Game::OnAddToScene>(
		[this] (const Game::OnAddToScene& event) {
			_HandleOnAddToScene(event);
		});

	m_GameDispatcher.Subscribe<Game::OnRemoveFromScene>(
		[this] (const Game::OnRemoveFromScene& event) {
			_HandleOnRemoveFromScene(event);
		});

	m_GameDispatcher.Subscribe<Game::OnClearScene>(
		[this] (const Game::OnClearScene& event) {
			_HandleOnClearScene(event);
		});
}

Application::~Application() {
	m_Window.UnregisterWindowListener(&m_WindowListener);
	m_Window.UnregisterKeyboardListener(&m_KeyboardListener);

	Nt::ResourceManager::Instance().Clear();
}

void Application::Initialize() {
	Profiler::Instance().Start();
	m_Window.Create(Nt::Int2D(1200, 1000), "Sunset Before Dawn");
	m_Window.SetCullFace(Nt::CullFace::NONE);
	m_Window.SetClearColor(Nt::Colors::LightBlue);
	Profiler::Instance().Stop("Window creation time");

	m_Window.RegisterWindowListener(&m_WindowListener);
	m_Window.RegisterKeyboardListener(&m_KeyboardListener);

	m_InputCollector.Register(&m_Window);
	m_InputSystem.LoadKeyBinds();

	m_RenderSystem = std::make_unique<RenderSystem>(&m_Window);

	Profiler::Instance().Start();
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::Textures() + "Entities\\Player.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::Textures() + "Entities\\Enemys\\Golem.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::Textures() + "Entities\\NPC\\Elf.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::Textures() + "Test.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::Textures() + "Water.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::Textures() + "UI\\HealthBar.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::Textures() + "UI\\HealthBar_Line.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::Textures() + "UI\\ArmorBar.tga");

	Nt::ResourceManager::Instance().Add<Nt::Mesh>(Nt::Primitive::Cube({ 1.f, 2.f, 1.f }, Nt::Colors::White));
	Nt::ResourceManager::Instance().Add<Nt::Mesh>(Nt::Primitive::Quad({ 1.f, 1.f }, Nt::Colors::White));
	Nt::ResourceManager::Instance().Add<Nt::Mesh>(PathManager::Models() + "Character.obj");
	Nt::ResourceManager::Instance().Add<Nt::Mesh>(PathManager::Models() + "Bomb.obj");
	Profiler::Instance().Stop("Textures addition in ResourceManager time");

	m_Game.Initialize();
}

void Application::Run() {
	m_Window.Show();

	Nt::_ShowCursor(false);
	while (m_Window.IsOpened()) {
		const Float deltaTime = m_Window.GetFrameTimeSec();
		m_RenderSystem->ClearScreen();

		m_InputSystem.TranslateInput(m_InputCollector.Poll());

		m_Game.Update(deltaTime);
		m_UISystem.Update(deltaTime);

		m_RenderSystem->UpdateCamera();
		m_RenderSystem->RenderScene();
		m_RenderSystem->RenderUI();

		m_Window.Display();
		m_Window.PeekMessages();

		if (m_FPSTimer.GetElapsedTimeMs() >= 1000) {
			BusLocator::DebugDispatcher()
				.Emmit<Debug::Event::OnChangedFPS>({ m_Window.GetFPS() });
			m_FPSTimer.Restart();
		}
	}
}

void Application::_HandleOnAddToScene(const Game::OnAddToScene& event) {
	if (event.pObject->GetType() == ObjectType::Canvas) {
		Canvas* pCanvas = static_cast<Canvas*>(event.pObject);
		m_UISystem.RegisterCanvas(*pCanvas);
	}

	m_RenderSystem->RegisterObject(*event.pObject);
}

void Application::_HandleOnRemoveFromScene(const Game::OnRemoveFromScene& event) {
	if (event.pObject->GetType() == ObjectType::Canvas) {
		Canvas* pCanvas = static_cast<Canvas*>(event.pObject);
		m_UISystem.UnregisterCanvas(*pCanvas);
	}

	m_RenderSystem->UnregisterObject(*event.pObject);
}

void Application::_HandleOnClearScene(const Game::OnClearScene& event) {
	m_RenderSystem->ClearObjects();
	m_UISystem.Clear();
}
