#pragma once

#include <Nt/Core/Math/Matrix4x4.h>

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
Nt::Matrix4x4 MatrixRotateYXZ(const Nt::Vector<_Ty, dimension>& vector) {
	Nt::Matrix4x4 result;

	if constexpr (dimension >= 2)
		result *= Nt::Matrix4x4::GetRotateY(vector.y);
	if constexpr (dimension >= 1)
		result *= Nt::Matrix4x4::GetRotateX(vector.x);
	if constexpr (dimension >= 3)
		result *= Nt::Matrix4x4::GetRotateZ(vector.z);

	return result;
}

template <typename _Ty, uInt dimension>
[[nodiscard]]
constexpr Nt::Matrix4x4 MatrixRotateZXY(const Nt::Vector<_Ty, dimension>& vector) {
	Nt::Matrix4x4 result;

	if constexpr (dimension >= 3)
		result *= Nt::Matrix4x4::GetRotateZ(vector.z);
	if constexpr (dimension >= 1)
		result *= Nt::Matrix4x4::GetRotateX(vector.x);
	if constexpr (dimension >= 2)
		result *= Nt::Matrix4x4::GetRotateY(vector.y);

	return result;
}

template <typename _Ty, uInt dimension>
[[nodiscard]]
constexpr Nt::Matrix4x4 MatrixRotateXYZ(const Nt::Vector<_Ty, dimension>& vector) {
	Nt::Matrix4x4 result;
	if constexpr (dimension >= 1)
		result *= Nt::Matrix4x4::GetRotateX(vector.x);
	if constexpr (dimension >= 2)
		result *= Nt::Matrix4x4::GetRotateY(vector.y);
	if constexpr (dimension >= 3)
		result *= Nt::Matrix4x4::GetRotateZ(vector.z);
	return result;
}

[[nodiscard]]
constexpr Nt::Matrix4x4& TranslateWorld(Nt::Matrix4x4& matrix, const Nt::Float3D& vector) noexcept {
	if (vector.LengthSquare() != 0.f)
		matrix.Rows[3].xyz += vector;
	return matrix;
}

template <typename _Ty, uInt Dimension>
struct Matrix {
	using Vector = Nt::Vector<_Ty, Dimension>;

	[[nodiscard]] static
	Nt::Matrix4x4 LocalToWorld(const Vector& position, const Vector& rotation, const Vector& size) noexcept;

	[[nodiscard]] static
	Nt::Matrix4x4 WorldToLocal(const Vector& position, const Vector& rotation, const Vector& size) noexcept;
};

template <typename _Ty, uInt Dimension>
Nt::Matrix4x4 Matrix<_Ty, Dimension>::LocalToWorld(const Vector& position, const Vector& rotation, const Vector& size) noexcept {
	const Nt::Vector<_Ty, 3> correctPosition =
		ConvertVector<_Ty, Dimension, 3>(position);
	const Nt::Vector<_Ty, 3> correctSize =
		ConvertVector<_Ty, Dimension, 3>(size);

	Nt::Matrix4x4 result = Nt::Matrix4x4::GetTranslate(correctPosition);
	if constexpr (Dimension >= 2)
		result *= Nt::Matrix4x4::GetRotateY(rotation.y);
	if constexpr (Dimension >= 1)
		result *= Nt::Matrix4x4::GetRotateX(rotation.x);
	if constexpr (Dimension >= 3)
		result *= Nt::Matrix4x4::GetRotateZ(rotation.z);

	return result * Nt::Matrix4x4::GetScale(correctSize);
}

template <typename _Ty, uInt Dimension>
Nt::Matrix4x4 Matrix<_Ty, Dimension>::WorldToLocal(const Vector& position, const Vector& rotation, const Vector& size) noexcept {
	const Nt::Vector<_Ty, 3> correctPosition =
		ConvertVector<_Ty, Dimension, 3>(position);
	const Nt::Vector<_Ty, 3> correctSize =
		ConvertVector<_Ty, Dimension, 3>(size);

	const Nt::Matrix4x4 matScale = Nt::Matrix4x4::GetScale(_Ty(1) / correctSize);
	const Nt::Matrix4x4 matRotation = MatrixRotateXYZ(rotation);
	const Nt::Matrix4x4 matTranslate = Nt::Matrix4x4::GetTranslate(-correctPosition);

	return matScale * matRotation * matTranslate;
}