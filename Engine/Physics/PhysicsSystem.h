#pragma once

#include <Engine/Physics/Base/PhysicsInterfaces.h>
#include <Engine/ISystem.h>
#include <vector>

class CollisionEventDispatcher;

class PhysicsSystem final : public IPhysicsOverlapper, public ISystem {
public:
	PhysicsSystem();
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem(PhysicsSystem&&) noexcept;
	~PhysicsSystem() noexcept override;

	PhysicsSystem& operator = (const PhysicsSystem&) = delete;
	PhysicsSystem& operator = (PhysicsSystem&&) noexcept;

	void Update(const Float& deltaTime) override;

	void RegisterObject(GameObject& object) override;
	void UnregisterObject(const GameObject& object) override;
	void Clear();

	[[nodiscard]]
	std::vector<PhysicObject> OverlapSphere(const Nt::Float3D& position, const Float& radius, const Int& layerMask) override;

	[[nodiscard]]
	std::vector<PhysicObject> OverlapAABB(const Nt::Float3D& min, const Nt::Float3D& max, const Int& layerMask) override;

	[[nodiscard]]
	std::vector<PhysicObject> OverlapRayCast(const Nt::Ray& ray, const Int& layerMask) override;

	template <class _Ty> requires std::is_base_of_v<IPhysicsIntegrator, _Ty>
	void SetPhysicsIntegrator() {
		m_pPhysicsIntegrator = std::make_unique<_Ty>();
		m_pPhysicsIntegrator->SetGravityVelocity(m_GravityDirection * m_G);
	}

	template <class _Ty> requires std::is_base_of_v<IBroadPhaseDetector, _Ty>
	void SetBroadPhaseDetector() {
		m_pBroadPhaseDetector = std::make_unique<_Ty>();
	}

	template <class _Ty> requires std::is_base_of_v<INarrowPhaseDetector, _Ty>
	void SetNarrowPhaseDetector() {
		m_pNarrowPhaseDetector = std::make_unique<_Ty>();
	}

	template <class _Ty> requires std::is_base_of_v<ICollisionResolver, _Ty>
	void SetCollisionResolver() {
		m_pCollisionResolver = std::make_unique<_Ty>();
	}

private:
	std::unique_ptr<IPhysicsIntegrator> m_pPhysicsIntegrator;
	std::unique_ptr<IBroadPhaseDetector> m_pBroadPhaseDetector;
	std::unique_ptr<INarrowPhaseDetector> m_pNarrowPhaseDetector;
	std::unique_ptr<ICollisionResolver> m_pCollisionResolver;
	std::unique_ptr<CollisionEventDispatcher> m_pCollisionDispatcher;

	std::vector<PhysicObject> m_Bodies;
	Nt::Float3D m_GravityDirection = { 0.f, -1.f, 0.f };
	Float m_G = 9.81f;
	Bool m_EnabledGravitation = true;
};