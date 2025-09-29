#include <NtStdH.h>

#include <Application.h>

#include <Helpers/Profiler.h>
#include <StdH.h>
#include <Nt/Graphics/Geometry/Primitives.h>

Application::Listener::Listener(NotNull<Application*> pApplication) noexcept:
	m_pApp(pApplication)
{
}

Application::WindowListener::WindowListener(NotNull<Application*> pApplication) noexcept:
	Listener(pApplication)
{
}

void Application::WindowListener::Resize(const Nt::ResizeType& type, const Nt::Int2D& newSize) {
	(void)type;
	(void)newSize;

	m_pApp->m_Window.SetPerspectiveProjection(90.f, 0.1f, 32000.f);
}

void Application::WindowListener::Close() {
	m_pApp->m_pGame->Uninitialize();
}

Application::KeyboardListener::KeyboardListener(NotNull<Application*> pApplication) noexcept:
	Listener(pApplication)
{
}

void Application::KeyboardListener::KeyReleased(const Nt::Key& key) {
	if (key == Nt::Key::_ESCAPE)
		m_pApp->m_Window.Close();
}

Application::Application():
	m_Window(true),
	m_WindowListener(this),
	m_KeyboardListener(this)
{
}

Application::~Application() {
	m_Window.UnregisterWindowListener(&m_WindowListener);
	m_Window.UnregisterKeyboardListener(&m_KeyboardListener);

	Nt::ResourceManager::Instance().Clear();
}

void Application::Initialize() {
	Profiler::Instance().Start();
	m_Window.Create(Nt::Int2D(1200, 1000), "Sunset Before Dawn");
	m_Window.SetClearColor(Nt::Colors::LightBlue);
	m_Window.SetCullFace(Nt::CullFace::NONE);
	m_Window.RegisterWindowListener(&m_WindowListener);
	m_Window.RegisterKeyboardListener(&m_KeyboardListener);
	Profiler::Instance().Stop("Window creation time");

	Profiler::Instance().Start();
	m_Shader.Initialize();
	m_Shader.Create();
	m_Shader.CompileFromFile(Nt::Shader::Types::VERTEX, "Shaders/Vert.glsl");
	//m_Shader.CompileFromFile(Nt::Shader::Types::GEOMETRY, "Shaders/Geometry.glsl");
	m_Shader.CompileFromFile(Nt::Shader::Types::FRAGMENT, "Shaders/Frag.glsl");
	m_Shader.Link();
	m_Shader.DisableStrict();
	m_Window.SetShader(&m_Shader);
	Profiler::Instance().Stop("Shader creation time");

	Profiler::Instance().Start();
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::GetTextures() + "Entities\\Player.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::GetTextures() + "Entities\\Enemys\\Golem.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::GetTextures() + "Entities\\NPC\\Elf.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::GetTextures() + "Test.tga");
	Nt::ResourceManager::Instance().Add<Nt::Texture>(PathManager::GetTextures() + "Water.tga");

	Nt::ResourceManager::Instance().Add<Nt::Mesh>(Nt::Primitive::Cube({ 1.f, 2.f, 1.f }, Nt::Colors::White));
	Nt::ResourceManager::Instance().Add<Nt::Mesh>(PathManager::GetModels() + "Charaster.obj");
	Profiler::Instance().Stop("Textures addition in ResourceManager time");

	m_pGame = std::make_unique<Game>();

	EventBus::Instance().Subscribe<Game::OnRegisterInput>(
		[this](const Game::OnRegisterInput& event) {
			event.pController->Register(&m_Window);
		});
	EventBus::Instance().Subscribe<Game::OnUnregisterInput>(
		[this](const Game::OnUnregisterInput& event) {
			event.pController->Unregister(&m_Window);
		});

	m_pGame->Initialize(&m_Shader);

	m_Window.SetCamera(m_pGame->GetCameraPtr());
}

void Application::Run() {
	m_Window.Show();

	while (m_Window.IsOpened()) {
		const Float time = m_Window.GetFrameTimeSec();

		m_Window.PeekMessages();
		m_Window.Clear();

		m_Window.Update();

		m_pGame->Update(time);
		m_pGame->Render(&m_Window);

		m_Shader.SetUniform("cameraPosition", m_Window.GetCamera()->GetPosition());
		m_Shader.SetUniform("lightDirection", Nt::Double3D(cos(m_Window.GetCamera()->GetAngle().y * RAD), 1.0, sin(m_Window.GetCamera()->GetAngle().y * RAD)));
		Nt::Double3D lightDirection = Nt::Double3D(1.0, -1.0, 1.0).GetNormalize();
		m_Shader.SetUniform("lightDirection", lightDirection);

		m_Window.Display();

		if (m_FPSTimer.GetElapsedTimeMs() >= 1000) {
			std::printf("FPS: %u\n", m_Window.GetFPS());
			m_FPSTimer.Restart();
		}
	}
}