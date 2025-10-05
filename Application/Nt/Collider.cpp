#include <GL/GLEW.h>
#include <GL/GL.h>

#include <Nt/Collider.h>
#include <Objects/GameObject.h>

namespace Nt {
	void Collider::Render(NotNull<Renderer*> pRenderer) const {
		if (!m_IsVisible)
			return;

		pRenderer->SetDepthMode(DepthMode::ALWAYS);
		pRenderer->SetLineWidth(10);
		pRenderer->SetDrawingMode(Renderer::DrawingMode::LINE_STRIP);

		pRenderer->Render(&m_Mesh);

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

	void Collider::SetShape(const Shape& shape) {
		m_Points.clear();
		if (shape.Indices.empty()) {
			m_Points.reserve(shape.Vertices.size());
			for (const Vertex& vertex : shape.Vertices)
				m_Points.emplace_back(vertex.Position.xyz);
		}
		else {
			m_Points.reserve(shape.Indices.size());
			for (const Index_t index : shape.Indices)
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

	Matrix4x4 Collider::GetLocalToWorld() const noexcept {
		return m_LocalToWorld;
	}

	void Collider::SetLocalToWorld(const Matrix4x4& matrix) noexcept {
		m_LocalToWorld = matrix;
	}
}
