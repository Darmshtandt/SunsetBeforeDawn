#include <Nt/IBody.h>

IBody::IBody(NotNull<GameObject*> pOwner, const ClassID& id) noexcept :
	Identifier(id),
	m_pOwner(pOwner),
	m_Collider(pOwner)
{
}

GameObject* IBody::GetOwnerPtr() noexcept {
	return m_pOwner;
}

Nt::Collider& IBody::GetCollider() noexcept {
	return m_Collider;
}

const Nt::Collider& IBody::GetCollider() const noexcept {
	return m_Collider;
}

void IBody::SetColliderShape(const Nt::Shape& shape) {
	m_Collider.SetShape(shape);
}