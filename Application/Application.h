#pragma once

#include <Application/Game.h>
#include <Core/Input/OSInputCollector.h>
#include <Engine/Input/InputSystem.h>
#include <Nt/Graphics/RenderWindow.h>
#include <UI/UISystem.h>
#include <Engine/RenderSystem.h>

class Application {
public:
	class Listener {
		friend Application;

	public:
		explicit Listener(NotNull<Application*> pApplication) noexcept;

	protected:
		Application* m_pApp;
	};

	class WindowListener final : public Listener, public Nt::WindowListener {
	public:
		explicit WindowListener(NotNull<Application*> pApplication) noexcept;

		WindowListener() = delete;
		WindowListener(const WindowListener&) = delete;
		WindowListener(WindowListener&&) = delete;
		virtual ~WindowListener() noexcept = default;

		WindowListener& operator = (const WindowListener&) noexcept = default;
		WindowListener& operator = (WindowListener&&) noexcept = default;

		void Resize(const Nt::ResizeType& type, const Nt::Int2D& size) override;
	};

	class KeyboardListener final : public Listener, public Nt::KeyboardListener {
	public:
		explicit KeyboardListener(NotNull<Application*> pApplication) noexcept;

		KeyboardListener() = delete;
		KeyboardListener(const KeyboardListener&) = delete;
		KeyboardListener(KeyboardListener&&) = delete;
		virtual ~KeyboardListener() noexcept = default;

		KeyboardListener& operator = (const KeyboardListener&) noexcept = default;
		KeyboardListener& operator = (KeyboardListener&&) noexcept = default;

		void KeyReleased(const Nt::Key& key) override;
	};

public:
	Application();
	Application(const Application&) = delete;
	Application(Application&&) noexcept = default;
	~Application();

	Application& operator = (const Application&) = delete;
	Application& operator = (Application&&) noexcept = default;

	void Initialize();
	void Run();

private:
	OSInputCollector m_InputCollector;
	InputSystem m_InputSystem;
	UISystem m_UISystem;
	std::unique_ptr<RenderSystem> m_RenderSystem;
	Game m_Game;
	EventBus* m_pGameDispatcher;

	Nt::RenderWindow m_Window;
	WindowListener m_WindowListener;
	KeyboardListener m_KeyboardListener;
	Nt::Shader m_Shader;
	Nt::Timer m_FPSTimer;

private:
	void _HandleOnAddToScene(const Game::OnAddToScene& event);
	void _HandleOnRemoveFromScene(const Game::OnRemoveFromScene& event);
	void _HandleOnClearScene(const Game::OnClearScene& event);
};
