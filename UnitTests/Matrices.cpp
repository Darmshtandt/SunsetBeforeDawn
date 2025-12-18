#include "pch.h"

#include "CppUnitTest.h"
#include <Nt/Core/Math/Matrix4x4.h>
#include <Core/Nt/MatrixPlus.h>
#include <Core/Nt/Quaternion.h>
#include <Nt/Core/Math/Projections.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MatricesTests {
	TEST_CLASS(Matrices) {
public:
	TEST_METHOD(Addition) {
		Nt::Matrix4x4 A = {
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16
		};

		Nt::Matrix4x4 B = {
			10, 20, 30, 40,
			50, 60, 70, 80,
			90, 100, 110, 120,
			130, 140, 150, 160
		};

		Nt::Matrix4x4 C = A + B;

		Assert::AreEqual(11.f, C.Matrix2D[0][0]);
		Assert::AreEqual(22.f, C.Matrix2D[0][1]);
		Assert::AreEqual(33.f, C.Matrix2D[0][2]);
		Assert::AreEqual(44.f, C.Matrix2D[0][3]);

		Assert::AreEqual(55.f, C.Matrix2D[1][0]);
		Assert::AreEqual(66.f, C.Matrix2D[1][1]);
		Assert::AreEqual(77.f, C.Matrix2D[1][2]);
		Assert::AreEqual(88.f, C.Matrix2D[1][3]);

		Assert::AreEqual(99.f, C.Matrix2D[2][0]);
		Assert::AreEqual(110.f, C.Matrix2D[2][1]);
		Assert::AreEqual(121.f, C.Matrix2D[2][2]);
		Assert::AreEqual(132.f, C.Matrix2D[2][3]);

		Assert::AreEqual(143.f, C.Matrix2D[3][0]);
		Assert::AreEqual(154.f, C.Matrix2D[3][1]);
		Assert::AreEqual(165.f, C.Matrix2D[3][2]);
		Assert::AreEqual(176.f, C.Matrix2D[3][3]);
	}

	TEST_METHOD(Subtraction) {
		Nt::Matrix4x4 A = {
			10, 20, 30, 40,
			50, 60, 70, 80,
			90, 100, 110, 120,
			130, 140, 150, 160
		};

		Nt::Matrix4x4 B = {
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16
		};

		Nt::Matrix4x4 C = A - B;

		Assert::AreEqual(9.f, C.Matrix2D[0][0]);
		Assert::AreEqual(18.f, C.Matrix2D[0][1]);
		Assert::AreEqual(27.f, C.Matrix2D[0][2]);
		Assert::AreEqual(36.f, C.Matrix2D[0][3]);

		Assert::AreEqual(45.f, C.Matrix2D[1][0]);
		Assert::AreEqual(54.f, C.Matrix2D[1][1]);
		Assert::AreEqual(63.f, C.Matrix2D[1][2]);
		Assert::AreEqual(72.f, C.Matrix2D[1][3]);
	}

	TEST_METHOD(MatrixMultiplication) {
		Nt::Matrix4x4 A = {
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16
		};

		Nt::Matrix4x4 B = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		Nt::Matrix4x4 I = A * B;

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				Assert::AreEqual(A.Matrix2D[i][j], I.Matrix2D[i][j]);
	}

	TEST_METHOD(MatrixMultiplicationKnown) {
		Nt::Matrix4x4 A = {
			1, 2, 3, 4,
			2, 3, 4, 5,
			3, 4, 5, 6,
			4, 5, 6, 7
		};

		Nt::Matrix4x4 B = {
			7, 6, 5, 4,
			6, 5, 4, 3,
			5, 4, 3, 2,
			4, 3, 2, 1
		};

		Nt::Matrix4x4 C = A * B;

		Assert::AreEqual(50.f, C.Matrix2D[0][0]);
		Assert::AreEqual(40.f, C.Matrix2D[1][0]);
		Assert::AreEqual(30.f, C.Matrix2D[2][0]);
		Assert::AreEqual(20.f, C.Matrix2D[3][0]);

		Assert::AreEqual(72.f, C.Matrix2D[0][1]);
		Assert::AreEqual(58.f, C.Matrix2D[1][1]);
		Assert::AreEqual(44.f, C.Matrix2D[2][1]);
		Assert::AreEqual(30.f, C.Matrix2D[3][1]);
	}

	TEST_METHOD(MatrixTimesVector4) {
		Nt::Matrix4x4 M = {
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16
		};

		Nt::Float4D v = { 1, 2, 3, 4 };

		Nt::Float4D r = M * v;

		Assert::AreEqual(1.f * 1 + 2 * 2 + 3 * 3 + 4 * 4, r.x);
		Assert::AreEqual(5.f * 1 + 6 * 2 + 7 * 3 + 8 * 4, r.y);
		Assert::AreEqual(9.f * 1 + 10 * 2 + 11 * 3 + 12 * 4, r.z);
		Assert::AreEqual(13.f * 1 + 14 * 2 + 15 * 3 + 16 * 4, r.w);
	}

	TEST_METHOD(MatrixTimesVector3) {
		Nt::Matrix4x4 M = {
			1, 2, 3, 10,
			4, 5, 6, 20,
			7, 8, 9, 30,
			1, 1, 1, 1
		};

		Nt::Float3D v = { 1, 2, 3 };

		Nt::Float4D r = M * v;

		Assert::AreEqual(1.f * 1 + 2 * 2 + 3 * 3 + 10, r.x);
		Assert::AreEqual(4.f * 1 + 5 * 2 + 6 * 3 + 20, r.y);
		Assert::AreEqual(7.f * 1 + 8 * 2 + 9 * 3 + 30, r.z);
		Assert::AreEqual(1.f * 1 + 1 * 2 + 1 * 3 + 1, r.w);
	}

	TEST_METHOD(InverseIdentity) {
		Nt::Matrix4x4 I = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		Nt::Matrix4x4 Inv = I.GetInverse();

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				Assert::AreEqual(I.Matrix2D[i][j], Inv.Matrix2D[i][j]);
	}

	TEST_METHOD(InverseKnown) {
		Nt::Matrix4x4 M = {
			4, 7, 2, 3,
			0, 5, 0, 1,
			0, 0, 3, 0,
			0, 0, 0, 2
		};

		Nt::Matrix4x4 Inv = M.GetInverse();

		// проверка M * Inv = Identity
		Nt::Matrix4x4 I = M * Inv;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				float expected = (i == j ? 1.f : 0.f);
				Assert::AreEqual(expected, I.Matrix2D[i][j], 0.0001f);
			}
		}
	}
	};

	TEST_CLASS(MatrixTransformTests) {
public:

	TEST_METHOD(LocalToWorldLH_Identity) {
		Nt::Float3D pos { 0,0,0 };
		Nt::Float3D rot { 0,0,0 };
		Nt::Float3D scale { 1,1,1 };

		auto m = Matrix<float, 3>::LocalToWorldLH(pos, rot, scale);

		// Проверяем единичную матрицу
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				Assert::AreEqual((i == j ? 1.f : 0.f), m.Matrix2D[i][j]);
	}

	TEST_METHOD(LocalToWorldRH_Identity) {
		Nt::Float3D pos { 0,0,0 };
		Nt::Float3D rot { 0,0,0 };
		Nt::Float3D scale { 1,1,1 };

		auto m = Matrix<float, 3>::LocalToWorldRH(pos, rot, scale);

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				Assert::AreEqual((i == j ? 1.f : 0.f), m.Matrix2D[i][j]);
	}

	TEST_METHOD(LocalToWorldLH_Translation) {
		Nt::Float3D pos { 1,2,3 };
		Nt::Float3D rot { 0,0,0 };
		Nt::Float3D scale { 1,1,1 };

		auto m = Matrix<float, 3>::LocalToWorldLH(pos, rot, scale);

		// Трансляция должна быть в последнем столбце
		Assert::AreEqual(1.f, m._14);
		Assert::AreEqual(2.f, m._24);
		Assert::AreEqual(3.f, m._34);
	}

	TEST_METHOD(WorldToLocalLH_InverseTranslation) {
		Nt::Float3D pos { 1,2,3 };
		Nt::Float3D rot { 0,0,0 };
		Nt::Float3D scale { 1,1,1 };

		auto mLocalToWorld = Matrix<float, 3>::LocalToWorldLH(pos, rot, scale);
		auto mWorldToLocal = Matrix<float, 3>::WorldToLocalLH(pos, rot, scale);

		Nt::Matrix4x4 identity = mWorldToLocal * mLocalToWorld;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				Assert::AreEqual((i == j ? 1.f : 0.f), identity.Matrix2D[i][j]);
	}

	TEST_METHOD(LocalToWorldRH_Rotation) {
		Nt::Float3D pos { 0,0,0 };
		Nt::Float3D rot { 0,0,90.f }; // Поворот 90° вокруг Z
		Nt::Float3D scale { 1,1,1 };

		auto m = Matrix<float, 3>::LocalToWorldRH(pos, rot, scale);

		// Ожидаем, что X' = -Y, Y' = X, Z' = Z
		Assert::AreEqual(0.f, m._11);
		Assert::AreEqual(-1.f, m._12);
		Assert::AreEqual(1.f, m._21);
		Assert::AreEqual(0.f, m._22);
	}

	TEST_METHOD(WorldToLocalRH_InverseCombined) {
		Nt::Float3D pos { 1,2,3 };
		Nt::Float3D rot { 45.f,30.f,60.f };
		Nt::Float3D scale { 2,3,4 };

		auto mLocalToWorld = Matrix<float, 3>::LocalToWorldRH(pos, rot, scale);
		auto mWorldToLocal = Matrix<float, 3>::WorldToLocalRH(pos, rot, scale);

		Nt::Matrix4x4 identity = mWorldToLocal * mLocalToWorld;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				Assert::IsTrue(fabs(identity.Matrix2D[i][j] - (i == j ? 1.f : 0.f)) < 1e-5f);
	}

	void AssertMatrixEqual(const Nt::Matrix4x4& a, const Nt::Matrix4x4& b, float eps) {
		for (int i = 0; i < 4 * 4; ++i)
			Assert::AreEqual(a.Matrix[i], b.Matrix[i], eps);
	}

	TEST_METHOD(TestQuaternionMatrixConsistency) {
		const float eps = 1e-5f;
		Nt::Float3D angles = { 0.3f, -0.6f, 1.2f };

		//
		// LH path
		//
		Nt::Matrix4x4 mLH =
			Nt::MatrixRotateLHX(angles.x) *
			Nt::MatrixRotateLHY(angles.y) *
			Nt::MatrixRotateLHZ(angles.z);

		auto qLH = Nt::Quaternion::FromEulerLH(angles);
		Nt::Matrix4x4 mFromQLH = qLH.ToMatrix4x4();

		AssertMatrixEqual(mLH, mFromQLH, eps);


		//
		// RH path
		//
		Nt::Matrix4x4 mRH =
			Nt::MatrixRotateRHX(angles.x) *
			Nt::MatrixRotateRHY(angles.y) *
			Nt::MatrixRotateRHZ(angles.z);

		auto qRH = Nt::Quaternion::FromEulerRH(angles);
		Nt::Matrix4x4 mFromQRH = qRH.ToMatrix4x4();

		AssertMatrixEqual(mRH, mFromQRH, eps);


		//
		// Forward vector test
		//
		Nt::Float3D forward = { 0,0,1 };

		auto f1 = mLH * forward;
		auto f2 = qLH * forward;

		Assert::AreEqual(f1.x, f2.x, eps);
		Assert::AreEqual(f1.y, f2.y, eps);
		Assert::AreEqual(f1.z, f2.z, eps);
	}

	};
}