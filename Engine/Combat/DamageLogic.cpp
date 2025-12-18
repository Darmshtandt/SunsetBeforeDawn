#include <Engine/Combat/DamageLogic.h>
#include <Engine/Physics/Base/PhysicsInterfaces.h>
#include <Nt/Graphics/Geometry/RayCast.h>
#include <Core/Nt/Quaternion.h>
#include <World/Components/GameComponents.h>

std::vector<PhysicObject> SphereDamage::HitDetect(const Nt::Float3D& position, const Nt::Float3D& rotation, NotNull<IPhysicsOverlapper*> pOverlapper) const noexcept {
	return pOverlapper->OverlapSphere(position, Radius, 0);
}

void SphereDamage::Apply(const std::vector<GameObject*>& targets) const {
	if (Amount == 0.f)
		return;

	for (GameObject* pObject : targets) {
		Health* pHealth = pObject->GetComponent<Health>();
		pHealth->Drain(Amount);
	}
}

Float SphereDamage::GetAmount() const noexcept {
	return Amount;
}

std::vector<PhysicObject> RayDamage::HitDetect(const Nt::Float3D& position, const Nt::Float3D& rotation, NotNull<IPhysicsOverlapper*> pOverlapper) const noexcept {
	const Nt::Quaternion quaternion = Nt::Quaternion::FromEulerLH(rotation);
	const Nt::Float3D forward = quaternion * Nt::Float3D(0.f, 0.f, 1.f);

	Nt::Ray ray;
 	ray.Start = Origin + position;
	ray.End = ray.Start + forward * Distance;

	return pOverlapper->OverlapRayCast(ray, 0);
}

void RayDamage::Apply(const std::vector<GameObject*>& targets) const {
	if (Amount == 0.f || targets.empty())
		return;

	for (GameObject* pObject : targets) {
		Health* pHealth = pObject->GetComponent<Health>();
		pHealth->Drain(Amount);

		if (!pHealth->IsAlive())
			pObject->UnmarkValid();
	}
}

Float RayDamage::GetAmount() const noexcept {
	return Amount;
}