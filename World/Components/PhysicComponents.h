#pragma once

#include <World/Components/BasicComponents.h>
#include <Engine/Physics/Base/RigidBody.h>
#include <Core/Nt/HitBox.h>

class Collider final : public IComponent {
public:
	explicit Collider(GameObject* pOwner) :
		IComponent(pOwner)
	{
		m_pTransform = pOwner->GetComponent<Transform3D>();
		if (m_pTransform == nullptr)
			m_pTransform = pOwner->AddComponent<Transform3D>();
	}

	void SetShape(const Nt::Shape& shape) {
		m_HitBox.SetShape(shape);
	}

	[[nodiscard]] const Nt::Matrix4x4& LocalToWorld() const noexcept {
		return m_pTransform->LocalToWorld();
	}
	[[nodiscard]] const Nt::PointContainer& GetPoints() const noexcept {
		return m_HitBox.GetPoints();
	}
	[[nodiscard]] uInt GetPointCount() const noexcept {
		return m_HitBox.GetPoints().size();
	}

private:
	Transform3D* m_pTransform;
	Nt::HitBox m_HitBox;
};

class RigidBody final : public IComponent {
public:
	explicit RigidBody(GameObject* pOwner) :
		IComponent(pOwner)
	{
	}

	Nt::RigidBody Body;
};

using ColliderPair = std::pair<Collider, Collider>;