#pragma once

#include <Core/Input/Commands.h>
#include <Core/Nt/MatrixPlus.h>
#include <World/Objects/GameObject.h>
#include <unordered_set>

template <typename _Ty, uInt Dimension>
class Transform final : public IComponent {
public:
	using Vector = Nt::Vector<_Ty, Dimension>;

public:
	explicit Transform(GameObject* pOwner) : IComponent(pOwner) {
		m_Size.Fill(_Ty(1));
	}

	[[nodiscard]]
	_Ty Distance(const Transform& other) const noexcept {
		return (m_Position - other.m_Position).Length();
	}
	[[nodiscard]]
	_Ty Distance(const Vector& position) const noexcept {
		return (m_Position - position).Length();
	}

	[[nodiscard]]
	_Ty DistanceSquare(const Transform& other) const noexcept {
		return (m_Position - other.m_Position).LengthSquare();
	}
	[[nodiscard]]
	_Ty DistanceSquare(const Vector& position) const noexcept {
		return (m_Position - position).LengthSquare();
	}

	[[nodiscard]]
	Vector CalculateAngle(const Transform& other) const noexcept {
		return (m_Position - other.m_Position).GetNormalize();
	}
	[[nodiscard]]
	Vector CalculateAngle(const Vector& position) const noexcept {
		return (m_Position - position).GetNormalize();
	}

	void Position(const Vector& size) noexcept {
		if (m_Position == size)
			return;

		m_Position = size;
		m_IsDirty = true;
	}
	void Rotation(const Vector& rotation) noexcept {
		if (m_Rotation == rotation)
			return;

		m_Rotation = rotation;
		m_IsDirty = true;
	}
	void Size(const Vector& size) noexcept {
		if (m_Size == size)
			return;

		m_Size = size;
		m_IsDirty = true;
	}

	void Translate(const Vector& offset) noexcept {
		if (offset.LengthSquare() == 0.f)
			return;

		m_Position += offset;
		m_IsDirty = true;
	}
	void Rotate(const Vector& rotation) noexcept {
		if (rotation.LengthSquare() == 0.f)
			return;

		m_Rotation += rotation;
		m_IsDirty = true;
	}
	void Resize(const Vector& size) noexcept {
		if (size.LengthSquare() == 0.f)
			return;

		m_Size += size;
		m_IsDirty = true;
	}

	[[nodiscard]] const Nt::Matrix4x4& LocalToWorld() const noexcept {
		if (m_IsDirty) {
			m_LocalToWorld = Matrix<_Ty, Dimension>::LocalToWorld(
				m_Position, m_Rotation, m_Size);
			m_WorldToLocal = Matrix<_Ty, Dimension>::WorldToLocal(
				m_Position, m_Rotation, m_Size);
			m_IsDirty = false;
		}
		return m_LocalToWorld;
	}
	[[nodiscard]] const Nt::Matrix4x4& WorldToLocal() const noexcept {
		if (m_IsDirty) {
			m_LocalToWorld = Matrix<_Ty, Dimension>::LocalToWorld(
				m_Position, m_Rotation, m_Size);
			m_WorldToLocal = Matrix<_Ty, Dimension>::WorldToLocal(
				m_Position, m_Rotation, m_Size);
			m_IsDirty = false;
		}
		return m_WorldToLocal;
	}

	[[nodiscard]] const Vector& Position() const noexcept {
		return m_Position;
	}
	[[nodiscard]] const Vector& Rotation() const noexcept {
		return m_Rotation;
	}
	[[nodiscard]] const Vector& Size() const noexcept {
		return m_Size;
	}
	[[nodiscard]] Bool IsDirty() const noexcept {
		return m_IsDirty;
	}

private:
	mutable Nt::Matrix4x4 m_LocalToWorld;
	mutable Nt::Matrix4x4 m_WorldToLocal;
	mutable Bool m_IsDirty = false;

	Vector m_Position;
	Vector m_Rotation;
	Vector m_Size;
};

template <typename _Ty, uInt Dimension>
class Movement final : public IComponent {
public:
	using Vector = Nt::Vector<_Ty, Dimension>;

public:
	explicit Movement(GameObject* pOwner) : IComponent(pOwner)
	{
	}

	[[nodiscard]] Vector Velocity() const noexcept {
		return Direction * Speed;
	}

	Vector Direction;
	Vector DesiredRotation;
	_Ty Speed = 0.f;
};

struct Handler final : public IComponent {
	explicit Handler(GameObject* pOwner) : IComponent(pOwner)
	{
	}
	~Handler() noexcept override = default;

	std::function<void(const Float& deltaTime)> Function;
};

using Transform2D = Transform<Float, 2>;
using Movement2D = Movement<Float, 2>;

using Transform3D = Transform<Float, 3>;
using Movement3D = Movement<Float, 3>;