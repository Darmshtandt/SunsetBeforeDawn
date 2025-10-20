#include <Engine/Physics/PhysicsSystem.h>
#include <Engine/Physics/IntegratorEuler.h>
#include <Engine/Physics/DebugBroadPhaseDetector.h>
#include <Engine/Physics/NarrowPhaseDetectorGJK.h>
#include <Engine/Physics/CollisionResolverImpulse.h>
#include <Engine/Physics/Base/CollisionEventDispatcher.h>
#include <World/Components/PhysicComponents.h>
#include <Core/BusLocator.h>

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


void PhysicsSystem::Update(const Float& deltaTime) const noexcept {
	for (const PhysicObject& object : m_Bodies) {
		m_pPhysicsIntegrator->Integrate(object, deltaTime);
	}

	std::vector<PhysicObjectPair> objectPairs =
		m_pBroadPhaseDetector->ComputePairs(m_Bodies);

	std::vector<CollisionContact> contacts =
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