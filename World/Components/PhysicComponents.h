#pragma once

#include <World/Components/BasicComponents.h>
#include <Engine/Physics/Base/RigidBody.h>
#include <Core/Nt/BoundingBox.h>
#include <Engine/Physics/Base/PhysicsInterfaces.h>
#include <Nt/Core/Colors.h>

class Collider final : public IComponent {
public:
	enum Axis {
		X, Y, Z
	};

public:
	explicit Collider(GameObject* pOwner) :
		IComponent(pOwner, Class<Collider>::ID()),
		m_LocalTransform(pOwner)
	{
		m_pTransform = pOwner->GetComponent<Transform3D>();
		if (m_pTransform == nullptr)
			m_pTransform = pOwner->AddComponent<Transform3D>();
		m_LocalTransform.SetParent(m_pTransform);
	}

	[[nodiscard]] Nt::BoundingBox GetBoundingBox() const noexcept {
		if (m_IsDirty) {
			_UpdateData();
			m_IsDirty = false;
		}

		const Nt::Float3D position = m_pTransform->Position();

		return {
			.Min = m_BoundingBox.Min + position,
			.Max = m_BoundingBox.Max + position
		};
	}
	[[nodiscard]] const Nt::Mesh& GetForwardLineMesh() const noexcept {
		if (m_IsDirty) {
			_UpdateData();
			m_IsDirty = false;
		}

		return m_ForwardLine;
	}

	[[nodiscard]] const Transform3D& LocalTransform() const noexcept {
		return m_LocalTransform;
	}
	[[nodiscard]] Transform3D& LocalTransform() noexcept {
		return m_LocalTransform;
	}

	[[nodiscard]] const Nt::HitBox& GetHitBox() const noexcept {
		return m_HitBox;
	}
	[[nodiscard]] const Nt::Matrix4x4& LocalToWorld() const noexcept {
		return m_LocalTransform.LocalToWorld();
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
	mutable Nt::BoundingBox m_BoundingBox;
	mutable Nt::Mesh m_ForwardLine;
	Nt::HitBox m_HitBox;
	Transform3D* m_pTransform;
	Transform3D m_LocalTransform;
	mutable Bool m_IsDirty = true;

private:
	void _UpdateData() const {
		m_BoundingBox = Nt::CalculateBoundingBox(m_HitBox.GetPoints());

		const Nt::Float3D eye = { 0.f, m_BoundingBox.Max.y * 0.75f, 0.f };
		const Nt::Float3D forward = Nt::Float3D(0.f, 0.f, m_BoundingBox.Max.z + 1.f);

		Nt::Shape forwardLine;
		forwardLine.Indices = { 0, 1 };
		forwardLine.Vertices = {
			Nt::Vertex(eye, { }, { }, Nt::Colors::White),
			Nt::Vertex(eye + forward, { }, { }, Nt::Colors::White)
		};

		m_ForwardLine = forwardLine;
	}
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