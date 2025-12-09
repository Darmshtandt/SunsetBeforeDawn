#pragma once

#include <World/Components/BasicComponents.h>
#include <Engine/Physics/Base/RigidBody.h>
#include <Core/Nt/HitBox.h>
#include <Engine/Physics/Base/PhysicsInterfaces.h>

struct AABBBox final {
	Nt::Float3D Min = { FLT_MAX, FLT_MAX, FLT_MAX };
	Nt::Float3D Max = { FLT_MIN, FLT_MIN, FLT_MIN };
};

class Collider final : public IComponent {
public:
	explicit Collider(GameObject* pOwner) :
		IComponent(pOwner, Class<Collider>::ID())
	{
		m_pTransform = pOwner->GetComponent<Transform3D>();
		if (m_pTransform == nullptr)
			m_pTransform = pOwner->AddComponent<Transform3D>();
	}

	[[nodiscard]] AABBBox GetAABBBox() const noexcept {
		if (m_IsDirty) {
			m_ShapeBox = AABBBox();

			const Nt::PointContainer& points = m_HitBox.GetPoints();
			for (const Nt::Float4D& point : points) {
				m_ShapeBox.Min = m_ShapeBox.Min.Min(point.xyz);
				m_ShapeBox.Max = m_ShapeBox.Max.Max(point.xyz);
			}

			m_IsDirty = false;
		}

		const Nt::Float3D& position = m_pTransform->Position();

		return {
			m_ShapeBox.Min + position,
			m_ShapeBox.Max + position
		};
	}

	[[nodiscard]] const Nt::HitBox& GetHitBox() const noexcept {
		return m_HitBox;
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

	void SetShape(const Nt::Shape& shape) {
		m_HitBox.SetShape(shape);
		m_IsDirty = true;
	}

public:
	std::function<void(const PhysicObject& object, const CollisionContact& contact)> OnEnter;
	std::function<void(const PhysicObject& object, const CollisionContact& contact)> OnStay;
	std::function<void(const PhysicObject& object)> OnExit;

private:
	mutable AABBBox m_ShapeBox;
	Nt::HitBox m_HitBox;
	Transform3D* m_pTransform;
	mutable Bool m_IsDirty = true;

};

class RigidBody final : public IComponent {
public:
	explicit RigidBody(GameObject* pOwner) :
		IComponent(pOwner, Class<RigidBody>::ID())
	{
	}

	Nt::RigidBody Body;
};

struct ITouchField {
	ITouchField(GameObject& object) {
		m_pTransform = object.GetComponent<Transform3D>();
		Assert(m_pTransform != nullptr, "The Transform3D component is missing");
	}
	virtual ~ITouchField() noexcept = default;

	[[nodiscard]] virtual Bool BroadPhaseTest(const Collider& collider) const noexcept = 0;
	[[nodiscard]] virtual Bool NarrowPhaseTest(const Collider& collider) const noexcept = 0;

	std::function<void(GameObject& object)> OnEnter;
	std::function<void(GameObject& object)> OnIn;
	std::function<void(GameObject& object)> OnExit;

protected:
	Transform3D* m_pTransform;
};

struct TouchSphere final : ITouchField {
	TouchSphere(GameObject& object) : ITouchField(object)
	{
	}
	~TouchSphere() noexcept override = default;

	[[nodiscard]] Bool BroadPhaseTest(const Collider& collider) const noexcept override {
		return true;
	}
	[[nodiscard]] Bool NarrowPhaseTest(const Collider& collider) const noexcept override {
		const Nt::PointContainer& colliderPoints = collider.GetPoints();
		for (const Nt::Float4D& point : colliderPoints) {
			const Nt::Float3D relativePosition = 
				point.xyz - m_pTransform->Position();
			if (relativePosition.LengthSquare() < Radius * Radius)
				return true;
		}

		return false;
	}

	Float Radius = 0.f;
};

class TouchTrigger final : public IComponent {
public:
	explicit TouchTrigger(GameObject* pOwner) :
		IComponent(pOwner, Class<TouchTrigger>::ID())
	{
	}

public:
	std::vector<ITouchField> m_Fields;
};

using ColliderPair = std::pair<Collider, Collider>;