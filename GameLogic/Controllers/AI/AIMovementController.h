#pragma once

#include <World/Components/BasicComponents.h>
#include <Engine/AI/AIContext.h>

class AIMovementController : public IMovementController {
public:
	explicit AIMovementController(GameObject& object) :
		m_pTransform(RequireNotNull(object.GetComponent<Transform3D>())),
		m_pMovement(RequireNotNull(object.GetComponent<Movement3D>())),
		m_pRoute(RequireNotNull(object.GetComponent<Route3D>()))
	{
	}
	~AIMovementController() noexcept override = default;

	void MoveTo(const Nt::Float3D& point) override {
		const Float distanceSq = m_pTransform->DistanceSquare(point);
		if (distanceSq <= FLT_EPSILON)
			return;

		const Nt::Float3D direction = m_pTransform->CalculateAngle(point);
		const Float speed = m_pMovement->Speed;

		if (distanceSq >= speed * speed)
			m_pMovement->Direction = direction;
		else
			m_pMovement->Direction = direction * (std::sqrtf(distanceSq) / speed);
	}

	void MoveInDirection(const Nt::Float3D& direction) override {
		m_pMovement->Direction = direction;
	}

	void LookInDirection(const Nt::Float3D& direction, Float time) override {
		m_pMovement->DesiredRotation.y = -std::atan2(direction.x, direction.z);
		m_pMovement->DesiredRotation.y -= m_pTransform->Rotation().y;
		m_pMovement->DesiredRotation.y /= time;

		if (!std::isfinite(m_pMovement->DesiredRotation.y))
			m_pMovement->DesiredRotation.y = 0.f;
	}

	[[nodiscard]] Bool IsAt(const Nt::Float3D& point) const override {
		return m_pTransform->DistanceSquare(point) <= FLT_EPSILON;
	}

	[[nodiscard]] Route3D* GetRoute() override {
		return m_pRoute;
	}

private:
	Transform3D* m_pTransform;
	Movement3D* m_pMovement;
	Route3D* m_pRoute;
};