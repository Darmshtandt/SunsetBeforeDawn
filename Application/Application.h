#pragma once

#include <Game.h>
#include <Nt/Graphics/FrameBuffer.h>
#include <Nt/Graphics/RenderWindow.h>

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

		void Resize([[maybe_unused]] const Nt::ResizeType& type, [[maybe_unused]] const Nt::Int2D& newSize) override;

		void Close() override;
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
	std::unique_ptr<Game> m_pGame = nullptr;
	Nt::RenderWindow m_Window;
	WindowListener m_WindowListener;
	KeyboardListener m_KeyboardListener;
	Nt::Shader m_Shader;
	Nt::Timer m_FPSTimer;
};
