#pragma once

#include <Nt/Core/Math/Matrix4x4.h>

namespace Nt {
	struct Quaternion {
		using Type = Float;
		using Type3D = Vector3D<Type>;
		using Type4D = Vector4D<Type>;

		static constexpr Type Epsilon = std::numeric_limits<Type>::epsilon();

		NT_CONSTEXPR Quaternion() noexcept = default;
		NT_CONSTEXPR Quaternion(Type X, Type Y, Type Z, Type W) noexcept :
			x(X), y(Y), z(Z), w(W)
		{
		}
		explicit Quaternion(const Matrix3x3& matrix) noexcept {
			*this = FromMatrix(matrix);
		}
		explicit Quaternion(const Matrix4x4& matrix4x4) noexcept {
			Matrix3x3 matrix3x3;
			for (uInt i = 0; i < 3; ++i)
				matrix3x3.Rows[i] = matrix4x4.Columns[i].xyz;

			*this = FromMatrix(matrix3x3);
		}

		NT_NODISCARD_CONSTEXPR
		static Quaternion Identity() noexcept {
			return Quaternion(0, 0, 0, 1);
		}
		NT_NODISCARD static Quaternion FromAxisAngleLH(const Type3D& axis, Type angle) noexcept {
			const Type halfAngle = angle / static_cast<Type>(2);

			const Type s = -std::sin(halfAngle);
			const Type c = static_cast<Type>(std::cos(halfAngle));

			return Quaternion(axis.x * s, axis.y * s, axis.z * s, c);
		}
		NT_NODISCARD static Quaternion FromAxisAngleRH(const Type3D& axis, Type angle) noexcept {
			const Type halfAngle = angle / static_cast<Type>(2);

			const Type s = std::sin(halfAngle);
			const Type c = static_cast<Type>(std::cos(halfAngle));

			return Quaternion(axis.x * s, axis.y * s, axis.z * s, c);
		}
		NT_NODISCARD static Quaternion FromEulerLH(const Type3D& angle) noexcept {
			const Type3D halfAngle = angle * static_cast<Type>(0.5);

			const Type sx = std::sin(halfAngle.x), cx = std::cos(halfAngle.x);
			const Type sy = std::sin(halfAngle.y), cy = std::cos(halfAngle.y);
			const Type sz = std::sin(halfAngle.z), cz = std::cos(halfAngle.z);

			const Quaternion qx(sx, 0, 0, cx);
			const Quaternion qy(0, sy, 0, cy);
			const Quaternion qz(0, 0, sz, cz);

			return qz * qy * qx;
		}
		NT_NODISCARD static Quaternion FromEulerRH(const Type3D& angle) noexcept {
			const Type3D halfAngle = angle * static_cast<Type>(0.5);

			const Type sx = -std::sin(halfAngle.x), cx = std::cos(halfAngle.x);
			const Type sy = -std::sin(halfAngle.y), cy = std::cos(halfAngle.y);
			const Type sz = -std::sin(halfAngle.z), cz = std::cos(halfAngle.z);

			const Quaternion qx(sx, 0, 0, cx);
			const Quaternion qy(0, sy, 0, cy);
			const Quaternion qz(0, 0, sz, cz);

			return qz * qy * qx;
		}

		NT_NODISCARD_CONSTEXPR static Quaternion FromMatrix(const Matrix3x3& matrix) noexcept;
		NT_NODISCARD_CONSTEXPR static Quaternion LookRotation(const Type3D& forward, const Type3D& up);

		NT_NODISCARD_CONSTEXPR Quaternion operator * (const Quaternion& quat) const {
			return Quaternion(
				w * quat.x + x * quat.w + y * quat.z - z * quat.y,
				w * quat.y - x * quat.z + y * quat.w + z * quat.x,
				w * quat.z + x * quat.y - y * quat.x + z * quat.w,
				w * quat.w - x * quat.x - y * quat.y - z * quat.z
			);
		}
		NT_NODISCARD_CONSTEXPR Type3D operator * (const Type3D& vector) const {
			const Type3D qVector(x, y, z);
			const Type3D t = qVector.GetCross(vector) * static_cast<Type>(2);
			return vector + t * w + qVector.GetCross(t);
		}

		NT_NODISCARD_CONSTEXPR Quaternion operator + (const Quaternion& quat) const noexcept {
			return {
				x + quat.x,
				y + quat.y,
				z + quat.z,
				w + quat.w
			};
		}
		NT_NODISCARD_CONSTEXPR Quaternion operator - (const Quaternion& quat) const noexcept {
			return {
				x - quat.x,
				y - quat.y,
				z - quat.z,
				w - quat.w
			};
		}
		NT_NODISCARD_CONSTEXPR Quaternion operator * (Type scalar) const noexcept {
			return {
				x * scalar,
				y * scalar,
				z * scalar,
				w * scalar
			};
		}
		NT_NODISCARD_CONSTEXPR Quaternion operator / (Type scalar) const noexcept {
			return {
				x / scalar,
				y / scalar,
				z / scalar,
				w / scalar
			};
		}

		NT_NODISCARD_CONSTEXPR Bool operator == (const Quaternion& quat) const noexcept {
			return Abs(Dot(quat)) > (static_cast<Type>(1) - Epsilon);
		}

		NT_NODISCARD Type Length() const noexcept {
			return std::sqrt(LengthSquared());
		}
		NT_NODISCARD_CONSTEXPR Type LengthSquared() const  noexcept {
			return x * x + y * y + z * z + w * w;
		}
		NT_NODISCARD Quaternion GetNormalize() const noexcept {
			const Type lengthSq = LengthSquared();
			if (std::abs(lengthSq) <= Epsilon)
				return Identity();

			const Type inverseLength = static_cast<Type>(1) / std::sqrt(lengthSq);
			return {
				x * inverseLength,
				y * inverseLength,
				z * inverseLength,
				w * inverseLength
			};
		}
		NT_NODISCARD_CONSTEXPR Type Dot(const Quaternion& quat) const noexcept {
			return x * quat.x + y * quat.y + z * quat.z + w * quat.w;
		}

		NT_NODISCARD_CONSTEXPR Quaternion GetConjugate() const noexcept {
			return { -x, -y, -z, w };
		}
		NT_NODISCARD Quaternion GetInverse() const noexcept {
			const Type lengthSq = LengthSquared();
			if (std::abs(lengthSq) <= Epsilon)
				return Identity();

			const Type inverseLength = static_cast<Type>(1) / std::sqrt(lengthSq);
			return {
				-x * inverseLength,
				-y * inverseLength,
				-z * inverseLength,
				 w * inverseLength
			};
		}

		static Quaternion Lerp(const Quaternion& a, const Quaternion& b, Type t);
		static Quaternion NLerp(const Quaternion& a, const Quaternion& b, Type t);
		static Quaternion SLerp(const Quaternion& a, const Quaternion& b, Type t);

		Matrix3x3 ToMatrix3x3() const noexcept {
			const Type xx = x * x, yy = y * y, zz = z * z;
			const Type xy = x * y, xz = x * z, yz = y * z;
			const Type wx = w * x, wy = w * y, wz = w * z;

			Matrix3x3 m;
			m._11 = static_cast<Type>(1) - static_cast<Type>(2) * (yy + zz);
			m._12 = static_cast<Type>(2) * (xy + wz);
			m._13 = static_cast<Type>(2) * (xz - wy);

			m._21 = static_cast<Type>(2) * (xy - wz);
			m._22 = static_cast<Type>(1) - static_cast<Type>(2) * (xx + zz);
			m._23 = static_cast<Type>(2) * (yz + wx);

			m._31 = static_cast<Type>(2) * (xz + wy);
			m._32 = static_cast<Type>(2) * (yz - wx);
			m._33 = static_cast<Type>(1) - static_cast<Type>(2) * (xx + yy);

			return m;
		}

		Matrix4x4 ToMatrix4x4() const noexcept {
			Matrix3x3 r = ToMatrix3x3();
			Matrix4x4 m;
			m._11 = r._11; m._21 = r._12; m._31 = r._13; m._41 = 0;
			m._12 = r._21; m._22 = r._22; m._32 = r._23; m._42 = 0;
			m._13 = r._31; m._23 = r._32; m._33 = r._33; m._43 = 0;
			m._14 = 0;     m._24 = 0;     m._34 = 0;     m._44 = 1;
			return m;
		}

		Type3D ToEuler() const;
		void ToAxisAngle(Type3D& outAxis, Type& outAngle) const;

		Quaternion RotatedBy(const Quaternion& rhs) const;
		Quaternion RotateX(Type angle) const;
		Quaternion RotateY(Type angle) const;
		Quaternion RotateZ(Type angle) const;

		Quaternion EnsureNonZero() const;
		Quaternion EnsureShortestPath(const Quaternion& rhs) const;
		Bool IsNormalized(Type eps = 1e-6f) const;

		static Type AngleBetween(const Quaternion& a, const Quaternion& b);
		static Quaternion LookAt(const Type3D& pos, const Type3D& target, const Type3D& up);
		static Quaternion FromTo(const Type3D& from, const Type3D& to);

		Type x = Type(0);
		Type y = Type(0);
		Type z = Type(0);
		Type w = Type(1);
	};
}