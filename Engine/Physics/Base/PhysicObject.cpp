#include <Engine/Physics/Base/PhysicObject.h>
#include <World/Components/PhysicComponents.h>

void ObjectContactPair::OnEnter() const {
	Assert(pA != nullptr, "Object A is null");
	Assert(pA->pCollider != nullptr, "Collider A is null");

	Assert(pB != nullptr, "Object B is null");
	Assert(pB->pCollider != nullptr, "Collide B is null");

	if (pA->pCollider->OnStay)
		pA->pCollider->OnStay(*pB, Contact);

	if (pB->pCollider->OnStay)
		pB->pCollider->OnStay(*pA, Contact);
}
void ObjectContactPair::OnStay() const {

}
void ObjectContactPair::OnExit() const {

}