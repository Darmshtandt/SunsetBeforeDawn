#include <Engine/Physics/PhysicsSystem.h>
#include <Engine/Physics/IntegratorEuler.h>
#include <Engine/Physics/DebugBroadPhaseDetector.h>
#include <Engine/Physics/NarrowPhaseDetectorGJK.h>
#include <Engine/Physics/CollisionResolverImpulse.h>
#include <Engine/Physics/Base/CollisionEventDispatcher.h>
#include <Engine/Physics/Base/PhysicUtilities.h>
#include <Core/BusLocator.h>
#include <Core/Nt/Algorithms/RayCast.h>

PhysicsSystem::PhysicsSystem() {
	SetPhysicsIntegrator<IntegratorEuler>();
	SetBroadPhaseDetector<DebugBroadPhaseDetector>();
	SetNarrowPhaseDetector<NarrowPhaseDetectorGJK>();
	SetCollisionResolver<CollisionResolverImpulse>();

	m_pCollisionDispatcher = std::make_unique<CollisionEventDispatcher>();

	BusLocator::PhysicsDispatcher()
		.Subscribe<OnCollisionEnter>([this](const OnCollisionEnter& event) {
		m_pCollisionResolver->Resolve(event.Contact);
		});
	BusLocator::PhysicsDispatcher()
		.Subscribe<OnCollisionStay>([this](const OnCollisionStay& event) {
		m_pCollisionResolver->Resolve(event.Contact);
		});
}

PhysicsSystem::PhysicsSystem(PhysicsSystem&&) noexcept = default;
PhysicsSystem::~PhysicsSystem() noexcept = default;

PhysicsSystem& PhysicsSystem::operator=(PhysicsSystem&&) noexcept = default;


void PhysicsSystem::Update(const Float& deltaTime) {
	for (const PhysicObject& object : m_Bodies)
		m_pPhysicsIntegrator->Integrate(object, deltaTime);

	std::vector<PhysicObjectPair> objectPairs =
		m_pBroadPhaseDetector->ComputePairs(m_Bodies);

	std::vector<ObjectContactPair> contacts =
		m_pNarrowPhaseDetector->GenerateContacts(objectPairs);

	m_pCollisionDispatcher->ProcessFrameContacts(contacts);
}

void PhysicsSystem::RegisterObject(GameObject& object) {
	auto* pTransform = RequireNotNull(object.GetComponent<Transform3D>());
	auto* pRigidBody = object.GetComponent<RigidBody>();
	auto* pCollider = object.GetComponent<Collider>();

	if (!pRigidBody && !pCollider)
		return;

	m_Bodies.emplace_back(PhysicObject {
		.pTransform = pTransform,
		.pRigidBody = pRigidBody,
		.pCollider = pCollider,
		.pObject = &object
	});
}

void PhysicsSystem::UnregisterObject(const GameObject& object) {
	std::erase_if(m_Bodies, [&object] (const PhysicObject& other) {
		return other.pObject == &object;
	});
}

void PhysicsSystem::Clear() {
	m_Bodies.clear();
}

std::vector<PhysicObject> PhysicsSystem::OverlapSphere(const Nt::Float3D& position, const Float& range, const Int& layerMask) {
	std::vector<PhysicObject> result;
	for (const PhysicObject& object : m_Bodies) {
		if (object.pCollider == nullptr)
			continue;

		if (HasColliderInRange(*object.pCollider, position, range))
			result.emplace_back(object);
	}

	return result;
}

std::vector<PhysicObject> PhysicsSystem::OverlapAABB(const Nt::Float3D& min, const Nt::Float3D& max, const Int& layerMask) {
	const AABBBox bBox = { min, max };

	std::vector<PhysicObject> result;
	for (const PhysicObject& object : m_Bodies) {
		if (object.pCollider == nullptr)
			continue;

		if (CheckAABBCollision(*object.pCollider, bBox))
			result.emplace_back(object);
	}

	return result;
}

std::vector<PhysicObject> PhysicsSystem::OverlapRayCast(const Nt::Ray& ray, const Int& layerMask) {
	std::vector<PhysicObject> result;
	for (const PhysicObject& object : m_Bodies) {
		if (object.pCollider == nullptr)
			continue;

		const Nt::RayCastResult testResult =
			Nt::RayCast::RayCastTest(*object.pCollider, ray);

		if (testResult.FaceIndex != -1)
			result.emplace_back(object);
	}

	return result;
}