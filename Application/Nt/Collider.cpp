#include <GL/GLEW.h>
#include <GL/GL.h>

#include <Nt/Collider.h>
#include <Nt/CollisionAlgorithms/GJK_EPA.h>
#include <Objects/GameObject.h>

namespace Nt {
	Collider::Collider(NotNull<GameObject*> pOwner) :
		m_pOwner(pOwner),
		m_Model(&m_Mesh),
		m_pAlgorithm(std::make_unique<GJK_EPA>(this))
	{
	}

	void Collider::Render(NotNull<Renderer*> pRenderer) const {
		if (!m_IsVisible)
			return;

		pRenderer->SetDepthMode(DepthMode::ALWAYS);
		pRenderer->SetLineWidth(10);
		pRenderer->SetDrawingMode(Renderer::DrawingMode::LINE_STRIP);

		pRenderer->Translate(m_pOwner->GetPosition());
		pRenderer->Render(&m_Mesh);
		pRenderer->Translate(-m_pOwner->GetPosition());

		pRenderer->SetDrawingMode(Renderer::DrawingMode::TRIANGLES);
		pRenderer->SetLineWidth(1);
		pRenderer->SetDepthMode(DepthMode::LESS);
	}

	void Collider::Show() noexcept {
		if (!m_IsVisible)
			m_IsVisible = true;
	}

	void Collider::Hide() noexcept {
		if (m_IsVisible)
			m_IsVisible = false;
	}

	void Collider::ToggleVisible(const Bool& enabled) noexcept {
		if (m_IsVisible != enabled)
			m_IsVisible = enabled;
	}

	void Collider::ToggleVisible() noexcept {
		m_IsVisible = !m_IsVisible;
	}

	Bool Collider::CheckCollision(const Collider& collider, CollisionPoint* pPoint) {
		if (m_pAlgorithm)
			return m_pAlgorithm->CheckCollision(collider, pPoint);
		return false;
	}

	Int Collider::RayCastTest(const Ray& ray, Float3D* pResultIntersectionPoint /*= nullptr*/) const {
		Float3D intersectionPoint;
		for (uInt i = 2; i < m_Points.size(); i += 3) {
			const Double3D face[3] = {
				GetPointRealPosition(-m_Points[i - 2]),
				GetPointRealPosition(-m_Points[i - 1]),
				GetPointRealPosition(-m_Points[i - 0]),
			};

			if (ray.IntersectTriangleTest(face, intersectionPoint)) {
				if (pResultIntersectionPoint != nullptr)
					(*pResultIntersectionPoint) = intersectionPoint;
				return (i / 3);
			}
		}
		return -1;
	}

	void Collider::SetShape(const Shape& shape) {
		m_Points.clear();
		if (shape.Indices.empty()) {
			m_Points.reserve(shape.Vertices.size());
			for (const Vertex& vertex : shape.Vertices)
				m_Points.emplace_back(vertex.Position.xyz);
		}
		else {
			m_Points.reserve(shape.Indices.size());
			for (Index_t index : shape.Indices)
				m_Points.emplace_back(shape.Vertices[index].Position.xyz);
		}

		m_Mesh.SetShape(shape);
	}

	void Collider::SetPointContainer(const PointContainer& points) {
		m_Points = points;

		Shape shape;
		for (const Float3D& point : m_Points) {
			Vertex vertex = { };
			vertex.Position = point;
			vertex.Color = Colors::White;

			shape.Vertices.push_back(vertex);
		}

		m_Mesh.SetShape(shape);
	}

	const PointContainer& Collider::GetPoints() const {
		return m_Points;
	}

	Bool Collider::IsVisible() const noexcept {
		return m_IsVisible;
	}

	Double3D Collider::GetPointRealPosition(const Float3D& point) const noexcept {
		const Matrix4x4& localToWorld = m_pOwner->LocalToWorld();

		Double3D position = Float3D(localToWorld.Rows[3]);
		//position.x -= Double((point.x * localToWorld._11) + (point.y * localToWorld._12) + (point.z * localToWorld._13));
		//position.y -= Double((point.x * localToWorld._21) + (point.y * localToWorld._22) + (point.z * localToWorld._23));
		//position.z -= Double((point.x * localToWorld._31) + (point.y * localToWorld._32) + (point.z * localToWorld._33));
		return position;
	}

	Matrix4x4 Collider::LocalToWorld() const noexcept {
		return m_pOwner->LocalToWorld();
	}
}