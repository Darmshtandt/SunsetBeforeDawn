#pragma once

#include <World/Entities.h>
#include <Nt/Core/Math/VectorND.h>

struct IAttackStrategy {
	virtual ~IAttackStrategy() noexcept = default;
};

struct AttackCommand final {
	GameObject* pObject = nullptr;
	IAttackStrategy* pStrategy = nullptr;
	Nt::Float3D Direction;
};

struct IAttackDispatcher {
	virtual ~IAttackDispatcher() noexcept = default;
	virtual void Dispatch(const AttackCommand&) = 0;
};

struct IDamageResolver {
	virtual ~IDamageResolver() noexcept = default;
	virtual void Resolve() = 0;
};

struct IProjectileService {
	virtual ~IProjectileService() noexcept = default;
	virtual void SpawnProjectile(const ClassID& id) = 0;
	virtual void Update(const Float& deltaTime) = 0;
};