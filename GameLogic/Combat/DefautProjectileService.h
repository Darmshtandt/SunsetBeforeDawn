#pragma once

#include <Engine/Combat/CombatInterfaces.h>

class DefautProjectileService final : public IProjectileService {
public:
	~DefautProjectileService() noexcept override = default;

	void SpawnProjectile(const ClassID& id) override;
	void Update(const Float& deltaTime) override;
};