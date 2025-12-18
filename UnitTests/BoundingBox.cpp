#include "pch.h"

#include <Core/Nt/BoundingBox.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BoundingBoxTests {
	using namespace Nt;

	TEST_CLASS(BoundingBoxIntersectionTests) {
public:

	TEST_METHOD(AABB_Intersect_FullOverlap) {
		BoundingBox a;
		a.Min = { 0, 0, 0 };
		a.Max = { 10, 10, 10 };

		BoundingBox b;
		b.Min = { 5, 5, 5 };
		b.Max = { 15, 15, 15 };

		Assert::IsTrue(a.HasIntersection(b));
		Assert::IsTrue(b.HasIntersection(a));
	}

	TEST_METHOD(AABB_NoIntersection) {
		BoundingBox a;
		a.Min = { 0, 0, 0 };
		a.Max = { 5, 5, 5 };

		BoundingBox b;
		b.Min = { 10, 10, 10 };
		b.Max = { 20, 20, 20 };

		Assert::IsFalse(a.HasIntersection(b));
		Assert::IsFalse(b.HasIntersection(a));
	}

	TEST_METHOD(AABB_TouchingFaces_ShouldIntersect) {
		BoundingBox a;
		a.Min = { 0, 0, 0 };
		a.Max = { 5, 5, 5 };

		BoundingBox b;
		b.Min = { 5, 0, 0 }; // касается по X
		b.Max = { 10, 5, 5 };

		// "Касание" считается пересечением
		Assert::IsTrue(a.HasIntersection(b));
		Assert::IsTrue(b.HasIntersection(a));
	}

	TEST_METHOD(Sphere_InsideAABB) {
		BoundingBox box;
		box.Min = { -5, -5, -5 };
		box.Max = { 5,  5,  5 };

		Float3D center = { 0, 0, 0 };
		float radius = 1.0f;

		Assert::IsTrue(box.HasSphereIntersection(center, radius));
	}

	TEST_METHOD(Sphere_OutsideAABB) {
		BoundingBox box;
		box.Min = { -5, -5, -5 };
		box.Max = { 5,  5,  5 };

		Float3D center = { 20, 0, 0 };
		float radius = 2.0f;

		Assert::IsFalse(box.HasSphereIntersection(center, radius));
	}

	TEST_METHOD(Sphere_TouchingAABB) {
		BoundingBox box;
		box.Min = { 0, 0, 0 };
		box.Max = { 10, 10, 10 };

		Float3D center = { 10, 5, 5 }; // центр на грани
		float radius = 1.0f;

		// Точка "closest" = (10,5,5). Расстояние = 0. 
		// Сфера касается — это пересечение
		Assert::IsTrue(box.HasSphereIntersection(center, radius));
	}

	TEST_METHOD(Sphere_JustOutsideAABB) {
		BoundingBox box;
		box.Min = { 0, 0, 0 };
		box.Max = { 10, 10, 10 };

		Nt::Float3D center = { 11.1f, 5, 5 };
		float radius = 1.0f;

		Assert::IsFalse(box.HasSphereIntersection(center, radius));
	}

	};
}