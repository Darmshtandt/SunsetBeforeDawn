#pragma once

#include <Nt/Core/Math/Matrix4x4.h>
#include <Nt/Core/Math/Projections.h>

#include "Quaternion.h"

template <typename _Ty, uInt fromDimension, uInt toDimension>
[[nodiscard]]
Nt::Vector<_Ty, toDimension> ConvertVector(const Nt::Vector<_Ty, fromDimension>& vector) {
	Nt::Vector<_Ty, toDimension> result;

	const uInt minDimension = std::min(fromDimension, toDimension);
	for (uInt i = 0; i < minDimension; ++i)
		result.Array[i] = vector.Array[i];

	for (uInt i = minDimension; i < toDimension; ++i)
		result.Array[i] = _Ty(0);

	if (toDimension == 4)
		result.Array[3] = _Ty(1);

	return result;
}

template <typename _Ty, uInt dimension>
[[nodiscard]]
constexpr Nt::Matrix4x4 MatrixRotateXYZ(const Nt::Vector<_Ty, dimension>& vector) {
	Nt::Matrix4x4 result;
	if constexpr (dimension >= 1)
		result *= Nt::MatrixRotateLHX(vector.x);
	if constexpr (dimension >= 2)
		result *= Nt::MatrixRotateLHY(vector.y);
	if constexpr (dimension >= 3)
		result *= Nt::MatrixRotateLHZ(vector.z);
	return result;
}

template <typename _Ty, uInt dimension>
[[nodiscard]]
constexpr Nt::Matrix4x4 MatrixRotateZYX(const Nt::Vector<_Ty, dimension>& vector) {
	Nt::Matrix4x4 result;
	if constexpr (dimension >= 3)
		result *= Nt::MatrixRotateLHZ(vector.z);
	if constexpr (dimension >= 2)
		result *= Nt::MatrixRotateLHY(vector.y);
	if constexpr (dimension >= 1)
		result *= Nt::MatrixRotateLHX(vector.x);
	return result;
}

[[nodiscard]]
constexpr Nt::Matrix4x4& TranslateWorld(Nt::Matrix4x4& matrix, const Nt::Float3D& vector) noexcept {
	if (vector.LengthSquare() != 0.f)
		matrix.Columns[3].xyz += vector;
	return matrix;
}

template <typename _Ty, uInt Dimension>
struct Matrix {
	using Vector = Nt::Vector<_Ty, Dimension>;

	[[nodiscard]] static
	Nt::Matrix4x4 LocalToWorldLH(const Vector& position, const Vector& rotation, const Vector& size) noexcept;
	[[nodiscard]] static
	Nt::Matrix4x4 LocalToWorldRH(const Vector& position, const Vector& rotation, const Vector& size) noexcept;

	[[nodiscard]] static
	Nt::Matrix4x4 WorldToLocalLH(const Vector& position, const Vector& rotation, const Vector& size) noexcept;
	[[nodiscard]] static
	Nt::Matrix4x4 WorldToLocalRH(const Vector& position, const Vector& rotation, const Vector& size) noexcept;


	[[nodiscard]] static
		Nt::Matrix4x4 LocalToWorldLH(const Vector& position, const Nt::Quaternion& rotation, const Vector& size) noexcept;
	[[nodiscard]] static
		Nt::Matrix4x4 LocalToWorldRH(const Vector& position, const Nt::Quaternion& rotation, const Vector& size) noexcept;

	[[nodiscard]] static
		Nt::Matrix4x4 WorldToLocalLH(const Vector& position, const Nt::Quaternion& rotation, const Vector& size) noexcept;
	[[nodiscard]] static
		Nt::Matrix4x4 WorldToLocalRH(const Vector& position, const Nt::Quaternion& rotation, const Vector& size) noexcept;


	[[nodiscard]] __inline static Nt::Float3D Right(const Nt::Matrix4x4& mat) noexcept {
		return mat.Columns[0].xyz;
	}
	[[nodiscard]] __inline static Nt::Float3D Up(const Nt::Matrix4x4& mat) noexcept {
		return mat.Columns[1].xyz;
	}
	[[nodiscard]] __inline static Nt::Float3D Forward(const Nt::Matrix4x4& mat) noexcept {
		return mat.Columns[2].xyz;
	}
	[[nodiscard]] __inline static Nt::Float3D Translation(const Nt::Matrix4x4& mat) noexcept {
		return mat.Columns[3].xyz;
	}

	[[nodiscard]] __inline static Nt::Float3D ExtractRotation(const Nt::Matrix4x4& mat) noexcept {
		const Nt::Float3D right = mat.Columns[0].xyz.GetNormalize();
		const Nt::Float3D up = mat.Columns[1].xyz.GetNormalize();
		const Nt::Float3D forward = mat.Columns[2].xyz.GetNormalize();

		return {
			atan2(up.z, forward.z),
			-asin(right.z),
			atan2(right.y, right.x)
		};
	}
};

template <typename _Ty, uInt Dimension>
Nt::Matrix4x4 Matrix<_Ty, Dimension>::LocalToWorldLH(const Vector& position, const Vector& rotation, const Vector& size) noexcept {
	const Nt::Vector<_Ty, 3> correctPosition =
		ConvertVector<_Ty, Dimension, 3>(position);
	const Nt::Vector<_Ty, 3> correctSize =
		ConvertVector<_Ty, Dimension, 3>(size);

	const Nt::Matrix4x4 matScale = Nt::Matrix4x4::GetScale(correctSize);
	const Nt::Matrix4x4 matRotation = MatrixRotateXYZ(rotation);
	const Nt::Matrix4x4 matTranslate = Nt::Matrix4x4::GetTranslate(correctPosition);

	return matTranslate * matRotation * matScale;
}
template <typename _Ty, uInt Dimension>
Nt::Matrix4x4 Matrix<_Ty, Dimension>::WorldToLocalLH(const Vector& position, const Vector& rotation, const Vector& size) noexcept {
	const Nt::Vector<_Ty, 3> correctPosition =
		ConvertVector<_Ty, Dimension, 3>(position);
	const Nt::Vector<_Ty, 3> correctSize =
		ConvertVector<_Ty, Dimension, 3>(size);

	const Nt::Matrix4x4 invScale = Nt::Matrix4x4::GetScale(_Ty(1) / correctSize);
	const Nt::Matrix4x4 invRotation = MatrixRotateXYZ(-rotation);
	const Nt::Matrix4x4 invTranslate = Nt::Matrix4x4::GetTranslate(-correctPosition);

	return invScale * invRotation * invTranslate;
}

template <typename _Ty, uInt Dimension>
Nt::Matrix4x4 Matrix<_Ty, Dimension>::LocalToWorldRH(const Vector& position, const Vector& rotation, const Vector& size) noexcept {
	const Nt::Vector<_Ty, 3> correctPosition =
		ConvertVector<_Ty, Dimension, 3>(position);
	const Nt::Vector<_Ty, 3> correctedRotation =
		ConvertVector<_Ty, Dimension, 3>(rotation) * Nt::Vector<_Ty, 3>(1, 1, -1);
	const Nt::Vector<_Ty, 3> correctSize =
		ConvertVector<_Ty, Dimension, 3>(size);

	const Nt::Matrix4x4 matScale = Nt::Matrix4x4::GetScale(correctSize);
	const Nt::Matrix4x4 matRotation = MatrixRotateXYZ(correctedRotation);
	const Nt::Matrix4x4 matTranslate = Nt::Matrix4x4::GetTranslate(correctPosition);

	return matTranslate * matRotation * matScale;
}
template <typename _Ty, uInt Dimension>
Nt::Matrix4x4 Matrix<_Ty, Dimension>::WorldToLocalRH(const Vector& position, const Vector& rotation, const Vector& size) noexcept {
	const Nt::Vector<_Ty, 3> correctPosition =
		ConvertVector<_Ty, Dimension, 3>(position);
	const Nt::Vector<_Ty, 3> correctRotation =
		ConvertVector<_Ty, Dimension, 3>(rotation) * Nt::Vector<_Ty, 3>(1, 1, -1);
	const Nt::Vector<_Ty, 3> correctSize =
		ConvertVector<_Ty, Dimension, 3>(size);

	const Nt::Matrix4x4 invScale = Nt::Matrix4x4::GetScale(_Ty(1) / correctSize);
	const Nt::Matrix4x4 invRotation = MatrixRotateXYZ(-correctRotation);
	const Nt::Matrix4x4 invTranslate = Nt::Matrix4x4::GetTranslate(-correctPosition);

	return invScale * invRotation * invTranslate;
}


template <typename _Ty, uInt Dimension>
Nt::Matrix4x4 Matrix<_Ty, Dimension>::LocalToWorldLH(const Vector& position, const Nt::Quaternion& rotation, const Vector& size) noexcept {
	const Nt::Vector<_Ty, 3> correctPosition =
		ConvertVector<_Ty, Dimension, 3>(position);
	const Nt::Vector<_Ty, 3> correctSize =
		ConvertVector<_Ty, Dimension, 3>(size);

	const Nt::Matrix4x4 matScale = Nt::Matrix4x4::GetScale(correctSize);
	const Nt::Matrix4x4 matRotation = rotation.ToMatrix4x4();
	const Nt::Matrix4x4 matTranslate = Nt::Matrix4x4::GetTranslate(correctPosition);

	return matTranslate * matRotation * matScale;
}
template <typename _Ty, uInt Dimension>
Nt::Matrix4x4 Matrix<_Ty, Dimension>::WorldToLocalLH(const Vector& position, const Nt::Quaternion& rotation, const Vector& size) noexcept {
	const Nt::Vector<_Ty, 3> correctPosition =
		ConvertVector<_Ty, Dimension, 3>(position);
	const Nt::Vector<_Ty, 3> correctSize =
		ConvertVector<_Ty, Dimension, 3>(size);

	const Nt::Matrix4x4 invScale = Nt::Matrix4x4::GetScale(_Ty(1) / correctSize);
	const Nt::Matrix4x4 invRotation = rotation.GetInverse().ToMatrix4x4();
	const Nt::Matrix4x4 invTranslate = Nt::Matrix4x4::GetTranslate(-correctPosition);

	return invScale * invRotation * invTranslate;
}