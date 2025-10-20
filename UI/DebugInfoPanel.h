#pragma once

#include <UI/Canvas.h>
#include <UI/UIText.h>
#include <Core/BusLocator.h>

class DebugInfoPanel final : public Canvas {
public:
	DebugInfoPanel() : Canvas(Class<DebugInfoPanel>::ID()) {
		m_pFPSCounter = AddObject<UIText>();
		m_pFPSCounter->pText->TextString = "FPS: 120";
		m_pFPSCounter->pLayout->Padding = { 20.f, 0.f };
		m_pFPSCounter->pScale->Position({ 0.f, 1.f });
		m_pFPSCounter->pScale->LocalPosition({ 0.f, 3.f });
		m_pFPSCounter->pScale->LocalSize({ 0.5f, 0.5f });

		AddComponent<Handler>()->Function = [this] (const Float& deltaTime) {
			(void)deltaTime;

			if (!BusLocator::DebugQuery().Has<Debug::Event::FPS>())
				return;

			m_pFPSCounter->pText->TextString = "FPS: " + std::to_string(
				BusLocator::DebugQuery().Request<Debug::Event::FPS>().Count);
			};
	}
	DebugInfoPanel(const DebugInfoPanel&) noexcept = default;
	DebugInfoPanel(DebugInfoPanel&&) noexcept = default;
	~DebugInfoPanel() noexcept override = default;

	DebugInfoPanel& operator = (const DebugInfoPanel&) noexcept = default;
	DebugInfoPanel& operator = (DebugInfoPanel&&) noexcept = default;

private:
	UIText* m_pFPSCounter;

	inline static ObjectRegistrar<DebugInfoPanel> g_Registrar { "DebugInfoPanel" };
};