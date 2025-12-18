#pragma once

#include <UI/Canvas.h>

class UIImage;

class UIHealthBar final : public Canvas {
public:
	UIHealthBar();
	UIHealthBar(const UIHealthBar&) noexcept = default;
	UIHealthBar(UIHealthBar&&) noexcept = default;
	~UIHealthBar() noexcept override = default;

	UIHealthBar& operator = (const UIHealthBar&) noexcept = default;
	UIHealthBar& operator = (UIHealthBar&&) noexcept = default;

private:
	UIImage* m_pHealthBarLine;

	inline static ObjectRegistrar<UIHealthBar> g_Registrar { "UIHealthBar" };
};

class UIArmorBar final : public Canvas {
public:
	UIArmorBar();
	UIArmorBar(const UIArmorBar&) noexcept = default;
	UIArmorBar(UIArmorBar&&) noexcept = default;
	~UIArmorBar() noexcept override = default;

	UIArmorBar& operator = (const UIArmorBar&) noexcept = default;
	UIArmorBar& operator = (UIArmorBar&&) noexcept = default;

private:
	inline static ObjectRegistrar<UIArmorBar> g_Registrar { "UIArmorBar" };
};

class UICrosshair final : public Canvas {
public:
	UICrosshair();
	UICrosshair(const UICrosshair&) noexcept = default;
	UICrosshair(UICrosshair&&) noexcept = default;
	~UICrosshair() noexcept override = default;

	UICrosshair& operator = (const UICrosshair&) noexcept = default;
	UICrosshair& operator = (UICrosshair&&) noexcept = default;

private:
	inline static ObjectRegistrar<UICrosshair> g_Registrar { "UICrosshair" };
};