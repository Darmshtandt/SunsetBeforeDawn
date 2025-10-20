#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	class RigidBody final {
	public:
		RigidBody() noexcept = default;
		RigidBody(const RigidBody&) noexcept = default;
		RigidBody(RigidBody&&) noexcept = default;
		~RigidBody() noexcept = default;

		RigidBody& operator = (const RigidBody&) noexcept = default;
		RigidBody& operator = (RigidBody&&) noexcept = default;

		void ApplyForces(const Float& deltaTime);
		void AddForce(const Float3D& force) noexcept;

		[[nodiscard]] Float3D GetLinearVelocity() const noexcept;
		[[nodiscard]] Float3D GetForce() const noexcept;

		[[nodiscard]] Float GetMass() const noexcept;
		[[nodiscard]] Float GetRestitution() const noexcept;
		[[nodiscard]] Float GetLinearDamping() const noexcept;
		[[nodiscard]] Bool HasForce() const noexcept;
		[[nodiscard]] Bool IsStatic() const noexcept;

		void SetLinearVelocity(const Float3D& linearVelocity) noexcept;
		void SetForce(const Float3D& force) noexcept;
		void SetMass(const Float& mass) noexcept;
		void SetRestitution(const Float& restitution) noexcept;
		void SetLinearDamping(const Float& linearDamping) noexcept;

	private:
		Float3D m_Force;
		Float3D m_LinearVelocity;
		Float m_Mass = 1.f;
		Float m_Restitution = 0.f;
		Float m_LinearDamping = 0.3f;
	};
}