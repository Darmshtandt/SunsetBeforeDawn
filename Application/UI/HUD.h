#pragma once

#include <UI/Canvas.h>

class HealthBar final : public Canvas {
public:
	HealthBar();
	HealthBar(const HealthBar&) noexcept = default;
	HealthBar(HealthBar&&) noexcept = default;
	~HealthBar() noexcept override = default;

	HealthBar& operator = (const HealthBar&) noexcept = default;
	HealthBar& operator = (HealthBar&&) noexcept = default;

private:
	inline static ObjectRegistrar<HealthBar> g_Registrar { "HealthBar" };
};

class ArmorBar final : public Canvas {
public:
	ArmorBar();
	ArmorBar(const ArmorBar&) noexcept = default;
	ArmorBar(ArmorBar&&) noexcept = default;
	~ArmorBar() noexcept override = default;

	ArmorBar& operator = (const ArmorBar&) noexcept = default;
	ArmorBar& operator = (ArmorBar&&) noexcept = default;

private:
	inline static ObjectRegistrar<ArmorBar> g_Registrar { "ArmorBar" };
};