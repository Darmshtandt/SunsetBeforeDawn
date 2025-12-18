#pragma once

#include <World/Components/BasicComponents.h>

#if 0
namespace Base {
	template <typename _Ty, uInt Dimension>
	class ScaleBase {
		using Vector = Nt::Vector<_Ty, Dimension>;
		using Transform = TransformSelector<_Ty, Dimension>::Type;

	public:
		explicit ScaleBase(NotNull<GameObject*> pOwner) {
			m_pTransform = pOwner->GetComponent<Transform>();
			if (m_pTransform == nullptr)
				m_pTransform = pOwner->AddComponent<Transform>();

			m_LocalSize.Fill(_Ty(1));
		}
		virtual ~ScaleBase() noexcept = default;

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

		void Padding(const Vector& padding) noexcept {
			if (m_Padding == padding)
				return;

			const Vector different = padding - m_Padding;
			m_pTransform->Translate(different);
			m_Padding = padding;
		}
		void Position(const Vector& position) noexcept {
			if (m_Position == position)
				return;

			const Vector different = position - m_Position;
			m_pTransform->Translate(m_Padding + different * m_ContextSize);
			m_Position = position;
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

		[[nodiscard]] const Vector& Padding() const noexcept {
			return m_Padding;
		}
		[[nodiscard]] const Vector& Position() const noexcept {
			return m_Position;
		}
		[[nodiscard]] const Vector& Size() const noexcept {
			return m_Size;
		}

		[[nodiscard]] const Vector& LocalPosition() const noexcept {
			return m_LocalPosition;
		}
		[[nodiscard]] const Vector& LocalSize() const noexcept {
			return m_LocalSize;
		}

	protected:
		Transform* m_pTransform = nullptr;
		Vector m_ContextSize;
		Vector m_LocalContextSize;

		Vector m_Padding;
		Vector m_Position;
		Vector m_Size;

		Vector m_LocalPosition;
		Vector m_LocalSize;

	protected:
		virtual void _ComputeTransform() noexcept = 0;
	};

	template <typename _Ty, uInt Dimension>
	class Scale final : public IComponent {
		using Vector = Nt::Vector<_Ty, Dimension>;
		using Transform = TransformSelector<_Ty, Dimension>::Type;

	public:
		explicit Scale(GameObject* pOwner) :
			IComponent(pOwner, Class<Scale>::ID()) {
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

		void Padding(const Vector& padding) noexcept {
			if (m_Padding == padding)
				return;

			const Vector different = padding - m_Padding;
			m_pTransform->Translate(different);
			m_Padding = padding;
		}
		void Position(const Vector& position) noexcept {
			if (m_Position == position)
				return;

			const Vector different = position - m_Position;
			m_pTransform->Translate(m_Padding + different * m_ContextSize);
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

		[[nodiscard]] const Vector& Padding() const noexcept {
			return m_Padding;
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

		Vector m_Padding;
		Vector m_Position;
		Vector m_Rotation;
		Vector m_Size;

		Vector m_LocalPosition;
		Vector m_LocalRotation;
		Vector m_LocalSize;

	private:
		void _ComputeTransform() noexcept {
			m_pTransform->LocalPosition(m_Padding + m_Position * m_ContextSize);
			m_pTransform->LocalRotation(m_Rotation * m_ContextSize);
			m_pTransform->Size(m_Size * m_ContextSize);

			m_pTransform->Resize(m_LocalSize * m_LocalContextSize);
			m_pTransform->Rotate(m_LocalRotation * m_pTransform->Size());
			m_pTransform->Translate(m_LocalPosition * m_pTransform->Size());
		}
	};
}
#endif

template <typename _Ty, uInt Dimension>
struct RotationTypeSelector;

template <typename _Ty>
struct RotationTypeSelector<_Ty, 2> final {
	using Type = _Ty;
};

template <typename _Ty>
struct RotationTypeSelector<_Ty, 3> final {
	using Type = Nt::Vector3D<_Ty>;
};

template <typename _Ty, uInt Dimension>
struct RotationTypeSelector final {
	using Type = Nt::Vector<_Ty, Dimension>;
};

template <typename _Ty, uInt Dimension>
using RotationType = typename RotationTypeSelector<_Ty, Dimension>::Type;


template <typename _Ty, uInt Dimension>
class Scale final : public IComponent {
	using RotationType = RotationType<_Ty, Dimension>;
	using Vector = Nt::Vector<_Ty, Dimension>;
	using Transform = typename TransformSelector<_Ty, Dimension>::Type;

public:
	explicit Scale(GameObject* pOwner) :
		IComponent(pOwner, Class<Scale>::ID())
	{
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

	void Padding(const Vector& padding) noexcept {
		if (m_Padding == padding)
			return;

		const Vector different = padding - m_Padding;
		m_pTransform->Translate(different);
		m_Padding = padding;
	}
	void Position(const Vector& position) noexcept {
		if (m_Position == position)
			return;

		const Vector different = position - m_Position;
		m_pTransform->Translate(m_Padding + different * m_ContextSize);
		m_Position = position;
	}
	void Rotation(const RotationType& rotation) noexcept {
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
	void LocalRotation(const RotationType& localRotation) noexcept {
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

	[[nodiscard]] const Vector& Padding() const noexcept {
		return m_Padding;
	}
	[[nodiscard]] const Vector& Position() const noexcept {
		return m_Position;
	}
	[[nodiscard]] const RotationType& Rotation() const noexcept {
		return m_Rotation;
	}
	[[nodiscard]] const Vector& Size() const noexcept {
		return m_Size;
	}

	[[nodiscard]] const Vector& LocalPosition() const noexcept {
		return m_LocalPosition;
	}
	[[nodiscard]] const RotationType& LocalRotation() const noexcept {
		return m_LocalRotation;
	}
	[[nodiscard]] const Vector& LocalSize() const noexcept {
		return m_LocalSize;
	}

private:
	Transform* m_pTransform = nullptr;
	Vector m_ContextSize;
	Vector m_LocalContextSize;

	Vector m_Padding;
	Vector m_Position;
	RotationType m_Rotation;
	Vector m_Size;

	Vector m_LocalPosition;
	RotationType m_LocalRotation;
	Vector m_LocalSize;

private:
	void _ComputeTransform() noexcept {
		m_pTransform->LocalPosition(m_Padding + m_Position * m_ContextSize);
		m_pTransform->LocalRotation(m_Rotation);
		m_pTransform->Size(m_Size * m_ContextSize);

		m_pTransform->Resize(m_LocalSize * m_LocalContextSize);
		m_pTransform->Rotate(m_LocalRotation);
		m_pTransform->Translate(m_LocalPosition * m_pTransform->Size());
	}
};

using Scale2D = Scale<Float, 2>;