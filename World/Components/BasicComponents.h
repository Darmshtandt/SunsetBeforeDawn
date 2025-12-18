#pragma once

#include <Core/Nt/MatrixPlus.h>
#include <Core/Nt/Quaternion.h>
#include <World/Objects/GameObject.h>

namespace Base {
	template <typename _Ty, uInt Dimension>
	class TransformBase {
	public:
		using Vector = Nt::Vector<_Ty, Dimension>;

	public:
		TransformBase() noexcept {
			m_Size.Fill(_Ty(1));
		}
		virtual ~TransformBase() noexcept = default;

		[[nodiscard]]
		_Ty Distance(const TransformBase& other) const noexcept {
			return (Position() - other.Position()).Length();
		}
		[[nodiscard]]
		_Ty Distance(const Vector& position) const noexcept {
			return (Position() - position).Length();
		}

		[[nodiscard]]
		_Ty DistanceSquare(const TransformBase& other) const noexcept {
			return (Position() - other.Position()).LengthSquare();
		}
		[[nodiscard]]
		_Ty DistanceSquare(const Vector& position) const noexcept {
			return (Position() - position).LengthSquare();
		}

		[[nodiscard]]
		Vector CalculateAngle(const TransformBase& other) const noexcept {
			return (Position() - other.Position()).GetNormalize();
		}
		[[nodiscard]]
		Vector CalculateAngle(const Vector& position) const noexcept {
			return (m_LocalPosition - position).GetNormalize();
		}

		[[nodiscard]] Vector Right() const noexcept {
			return Matrix<_Ty, Dimension>::Right(LocalToWorld());
		}
		[[nodiscard]] Vector Up() const noexcept {
			return Matrix<_Ty, Dimension>::Up(LocalToWorld());
		}
		[[nodiscard]] Vector Forward() const noexcept {
			return Matrix<_Ty, Dimension>::Forward(LocalToWorld());
		}

		void LocalPosition(const Vector& size) noexcept {
			if (m_LocalPosition == size)
				return;

			m_LocalPosition = size;
			_MarkDirty();
		}
		void Size(const Vector& size) noexcept {
			if (m_Size == size)
				return;

			m_Size = size;
			_MarkDirty();
		}

		void Translate(const Vector& offset) noexcept {
			if (offset.LengthSquare() == 0.f)
				return;

			m_LocalPosition += offset;
			_MarkDirty();
		}
		void Resize(const Vector& size) noexcept {
			if (size.LengthSquare() == 0.f)
				return;

			m_Size += size;
			_MarkDirty();
		}
		void SetParent(TransformBase* pParent) {
			if (m_pParent == pParent)
				return;

			if (m_pParent && pParent == nullptr) {
				auto& childs = m_pParent->m_Childs;
				childs.erase(std::remove(childs.begin(), childs.end(), this), childs.end());
			}
			else {
				pParent->m_Childs.push_back(this);
			}

			m_pParent = pParent;
			_MarkDirty();
		}

		[[nodiscard]] const Nt::Matrix4x4& LocalToWorld() const noexcept {
			if (m_IsDirty) {
				_ComputeMatrix();
				m_IsDirty = false;
			}
			return m_LocalToWorld;
		}
		[[nodiscard]] const Nt::Matrix4x4& WorldToLocal() const noexcept {
			if (m_IsDirty) {
				_ComputeMatrix();
				m_IsDirty = false;
			}
			return m_WorldToLocal;
		}

		[[nodiscard]] const Vector& LocalPosition() const noexcept {
			return m_LocalPosition;
		}
		[[nodiscard]] const Vector& Size() const noexcept {
			return m_Size;
		}

		[[nodiscard]] Vector Position() const noexcept {
			return Matrix<_Ty, Dimension>::Translation(LocalToWorld());
		}
		[[nodiscard]] Vector Rotation() const noexcept {
			return Matrix<_Ty, Dimension>::ExtractRotation(LocalToWorld());
		}

		[[nodiscard]] Bool IsDirty() const noexcept {
			return m_IsDirty;
		}

	protected:
		mutable Nt::Matrix4x4 m_LocalToWorld;
		mutable Nt::Matrix4x4 m_WorldToLocal;
		mutable Bool m_IsDirty = false;

		std::vector<TransformBase*> m_Childs;
		TransformBase* m_pParent = nullptr;
		Vector m_LocalPosition;
		Nt::Quaternion m_LocalRotation;
		Vector m_Size;

	protected:
		void _ComputeMatrix() const noexcept {
			m_LocalToWorld = Matrix<_Ty, Dimension>::LocalToWorldLH(
				m_LocalPosition, m_LocalRotation, m_Size);

			if (m_pParent != nullptr)
				m_LocalToWorld = m_pParent->LocalToWorld() * m_LocalToWorld;

			m_WorldToLocal = m_LocalToWorld.GetInverse();
		}
		void _MarkDirty() const noexcept {
			m_IsDirty = true;
			for (TransformBase* pChild : m_Childs)
				pChild->_MarkDirty();
		}
	};

	template <typename _Ty>
	class Transform2D final : public TransformBase<_Ty, 2>, public IComponent {
	public:
		using Vector = Nt::Vector2D<_Ty>;

	public:
		explicit Transform2D(GameObject* pOwner) :
			IComponent(pOwner, Class<Transform2D>::ID())
		{
		}
		~Transform2D() noexcept override = default;

		void LocalRotation(const _Ty& angle) noexcept {
			if (m_LocalAngle == angle)
				return;

			m_LocalAngle = angle;
			this->m_LocalRotation = Nt::Quaternion::FromEulerLH(Nt::Float3D(0.f, 0.f, m_LocalAngle));
			this->_MarkDirty();
		}

		void Rotate(const _Ty& angle) noexcept {
			if (angle == static_cast<_Ty>(0))
				return;

			m_LocalAngle += angle;
			this->m_LocalRotation = Nt::Quaternion::FromEulerLH(Nt::Float3D(0.f, 0.f, m_LocalAngle));
			this->_MarkDirty();
		}

		[[nodiscard]] const _Ty& LocalRotation() const noexcept {
			return m_LocalAngle;
		}

	private:
		_Ty m_LocalAngle = static_cast<_Ty>(0);
	};

	template <typename _Ty>
	class Transform3D final : public TransformBase<_Ty, 3>, public IComponent {
	public:
		using Vector = Nt::Vector3D<_Ty>;

	public:
		explicit Transform3D(GameObject* pOwner) :
			IComponent(pOwner, Class<Transform3D>::ID()) {
		}
		~Transform3D() noexcept override = default;

		void LocalRotation(const Nt::Quaternion& quaternion) noexcept {
			if (this->m_LocalRotation == quaternion)
				return;

			this->m_LocalRotation = quaternion.GetNormalize();
			this->_MarkDirty();
		}
		void LocalRotationEuler(const Vector& euler) noexcept {
			LocalRotation(Nt::Quaternion::FromEulerLH(euler));
		}

		void Rotate(const Vector& deltaEuler) noexcept {
			if (deltaEuler.LengthSquare() == 0.f)
				return;

			const Nt::Quaternion quaternion = Nt::Quaternion::FromEulerLH(deltaEuler);
			this->m_LocalRotation = (quaternion * this->m_LocalRotation).GetNormalize();
			this->_MarkDirty();
		}

		[[nodiscard]] const Nt::Quaternion& LocalRotation() const noexcept {
			return this->m_LocalRotation;
		}
		[[nodiscard]] Vector LocalRotationEuler() const noexcept {
			return this->m_LocalRotation.ToEuler();
		}
	};
}

template<typename _Ty, uInt Dimension>
struct TransformSelector;

template<typename _Ty>
struct TransformSelector<_Ty, 2> {
	using Type = Base::Transform2D<_Ty>;
};

template<typename _Ty>
struct TransformSelector<_Ty, 3> {
	using Type = Base::Transform3D<_Ty>;
};

template<typename _Ty, uInt Dimension>
struct TransformSelector {
	using Type = Base::TransformBase<_Ty, Dimension>;
};

template <typename _Ty, uInt Dimension>
class Movement final : public IComponent {
public:
	using Vector = Nt::Vector<_Ty, Dimension>;

public:
	explicit Movement(GameObject* pOwner) : IComponent(pOwner, Class<Movement>::ID())
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
	explicit Handler(GameObject* pOwner) : IComponent(pOwner, Class<Handler>::ID())
	{
	}
	~Handler() noexcept override = default;

	std::function<void(const Float& deltaTime)> Function;
};

using Transform2D = Base::Transform2D<Float>;
using Movement2D = Movement<Float, 2>;

using Transform3D = Base::Transform3D<Float>;
using Movement3D = Movement<Float, 3>;