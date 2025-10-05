#pragma once

#include <Objects/Components/BasicComponents.h>

template <typename _Ty, uInt Dimension>
class Scale final : public IComponent {
	using Vector = Nt::Vector<_Ty, Dimension>;
	using Transform = Transform<_Ty, Dimension>;

public:
	explicit Scale(GameObject* pOwner) :
		IComponent(pOwner) {
		m_pTransform = pOwner->GetComponent<Transform>();
		if (m_pTransform == nullptr)
			m_pTransform = pOwner->AddComponent<Transform>();

		m_LocalSize.Fill(_Ty(1));
	}

	void ContextSize(const Vector& contextSize) noexcept {
		if (m_ContextSize == contextSize)
			return;

		m_ContextSize = contextSize;
		_ComputeTransform();
	}
	void LocalContextSize(const Vector& localContextSize) noexcept {
		if (m_LocalContextSize == localContextSize)
			return;

		m_LocalContextSize = localContextSize;
		_ComputeTransform();
	}

	void Position(const Vector& position) noexcept {
		if (m_Position == position)
			return;

		const Vector different = position - m_Position;
		m_pTransform->Translate(different * m_ContextSize);
		m_Position = position;
	}
	void Rotation(const Vector& rotation) noexcept {
		if (m_Rotation == rotation)
			return;

		const Vector different = rotation - m_Rotation;
		m_pTransform->Rotation(different * m_ContextSize);
		m_Rotation = rotation;
	}
	void Size(const Vector& size) noexcept {
		if (m_Size == size)
			return;

		m_Size = size;
		_ComputeTransform();
	}

	void LocalPosition(const Vector& localPosition) noexcept {
		if (m_LocalPosition == localPosition)
			return;

		const Vector different = localPosition - m_LocalPosition;
		m_pTransform->Translate(different * m_pTransform->Size());
		m_LocalPosition = localPosition;
	}
	void LocalRotation(const Vector& localRotation) noexcept {
		if (m_LocalRotation == localRotation)
			return;

		const Vector different = localRotation - m_LocalRotation;
		m_pTransform->Rotate(different * m_pTransform->Size());
		m_LocalRotation = localRotation;
	}
	void LocalSize(const Vector& localSize) noexcept {
		if (m_LocalSize == localSize)
			return;

		m_LocalSize = localSize;
		_ComputeTransform();
	}

	[[nodiscard]] const Vector& ContextSize() const noexcept {
		return m_ContextSize;
	}
	[[nodiscard]] const Vector& LocalContextSize() const noexcept {
		return m_LocalContextSize;
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

	[[nodiscard]] const Vector& LocalPosition() const noexcept {
		return m_LocalPosition;
	}
	[[nodiscard]] const Vector& LocalRotation() const noexcept {
		return m_LocalRotation;
	}
	[[nodiscard]] const Vector& LocalSize() const noexcept {
		return m_LocalSize;
	}

private:
	Transform* m_pTransform = nullptr;
	Vector m_ContextSize;
	Vector m_LocalContextSize;

	Vector m_Position;
	Vector m_Rotation;
	Vector m_Size;

	Vector m_LocalPosition;
	Vector m_LocalRotation;
	Vector m_LocalSize;

private:
	void _ComputeTransform() noexcept {
		m_pTransform->Position(m_Position * m_ContextSize);
		m_pTransform->Rotation(m_Rotation * m_ContextSize);
		m_pTransform->Size(m_Size * m_ContextSize);

		m_pTransform->Resize(m_LocalSize * m_LocalContextSize);
		m_pTransform->Rotate(m_LocalRotation * m_pTransform->Size());
		m_pTransform->Translate(m_LocalPosition * m_pTransform->Size());
	}
};

template <typename _Ty, uInt Dimension>
class Layout final : public IComponent {
public:
	using Vector = Nt::Vector<_Ty, Dimension>;

public:
	explicit Layout(GameObject* pOwner) :
		IComponent(pOwner) {
	}

	Vector Padding;
};

using Scale2D = Scale<Float, 2>;
using Layout2D = Layout<Float, 2>;

using Scale3D = Scale<Float, 3>;
using Layout3D = Layout<Float, 3>;