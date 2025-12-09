#pragma once

#include <World/Components/IAttackComponent.h>

struct MeleeComponent final : public IAttackComponent {
	~MeleeComponent() noexcept override = default;
};

struct MultiComponent final : public IAttackComponent {
	~MultiComponent() noexcept override = default;
};

struct SplashComponent final : public IAttackComponent {
	~SplashComponent() noexcept override = default;
};

struct RangeComponent final : public IAttackComponent {
	~RangeComponent() noexcept override = default;
};

struct SpawnComponent final : public IAttackComponent {
	~SpawnComponent() noexcept override = default;
};