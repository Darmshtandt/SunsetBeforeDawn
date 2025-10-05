#pragma once

#include <Objects/GameObject.h>
#include <Nt/Graphics/Resources/Mesh.h>

#include <functional>
#include <execution>
#include <span>

class Terrain : public GameObject {
public:
	struct Config {
		struct Property {
			template <class _Op>
			Property& Apply(const Property& other) {
				AdditionHeight = _Op::Apply(AdditionHeight, other.AdditionHeight);
				Frequency = _Op::Apply(Frequency, other.Frequency);
				Amplitude = _Op::Apply(Amplitude, other.Amplitude);
				Lacunarity = _Op::Apply(Lacunarity, other.Lacunarity);
				Gain = _Op::Apply(Gain, other.Gain);
				Min = _Op::Apply(Min, other.Min);
				Max = _Op::Apply(Max, other.Max);
				NumOctaves = _Op::Apply(NumOctaves, other.NumOctaves);

				return *this;
			}

			template <class _Op>
			Property& Apply(const Double& value) {
				AdditionHeight = _Op::Apply(AdditionHeight, value);
				Frequency = _Op::Apply(Frequency, value);
				Amplitude = _Op::Apply(Amplitude, value);
				Lacunarity = _Op::Apply(Lacunarity, value);
				Gain = _Op::Apply(Gain, value);
				Min = _Op::Apply(Min, value);
				Max = _Op::Apply(Max, value);
				NumOctaves = _Op::Apply(NumOctaves, value);

				return *this;
			}

			Property& operator += (const Property& other);
			Property& operator *= (const Property& other);
			Property& operator += (const Double& value);
			Property& operator *= (const Double& value);
			Property operator * (const Double& value);

			Nt::Double2D Offset = { };
			Double AdditionHeight = 0.0;
			Double Frequency = 1.0;
			Double Amplitude = 1.0;
			Double Lacunarity = 1.0;
			Double Gain = 1.0;
			Double Min = 0.0;
			Double Max = 1.0;
			uInt NumOctaves = 1;
		};

		std::function<Double(const Nt::Double2D& point, const Double& noise)> Mask;
		std::vector<std::vector<Property>> Properties;
		Nt::DoubleRect BorderRect = { };
		Double BorderHeight = 0.0;
	};

public:
	explicit Terrain(const ClassID& id);

	Terrain() = delete;
	Terrain(const Terrain&) = default;
	Terrain(Terrain&&) noexcept = default;
	~Terrain() noexcept override = default;

	Terrain& operator = (const Terrain&) = default;
	Terrain& operator = (Terrain&&) noexcept = default;

	void Create(const Nt::Float2D& size, const Nt::uInt2D& vertexDensity);

	void Update(const Float& deltaTime) override;
#if 0
	void PerlinNoise() {
		for (uInt x = 0; x < m_VertexDensity.x; ++x) {
			for (uInt y = 0; y < m_VertexDensity.y; ++y) {
				Nt::Float3D& vertexPosition = m_Shape.Vertices[x + y * m_VertexDensity.x].Position.xyz;

				const Double noise = _Perlin({
						vertexPosition.x + FLT_EPSILON,
						vertexPosition.z + FLT_EPSILON
					});

				vertexPosition.y = Float(fmod(noise, 1));
			}
		}

		m_Mesh.SetShape(m_Shape);
	}
	void SimplexNoise(const Double& frequency, const Double& amplitude) {
		for (uInt x = 0; x < m_VertexDensity.x; ++x) {
			for (uInt y = 0; y < m_VertexDensity.y; ++y) {
				Nt::Float3D& vertexPosition = m_Shape.Vertices[x + y * m_VertexDensity.x].Position.xyz;

				const Nt::Double2D point = {
					vertexPosition.x, vertexPosition.z
				};

				vertexPosition.y = Float(
					_Simplex(
						(Nt::Double2D(m_Position.x, m_Position.z) 
							+ point) * frequency) * amplitude);
			}
		}

		m_Mesh.SetShape(m_Shape);
	}
	void FractalBrownianMotionNoise(const Config& config) {
		if (m_VertexDensity.x == 0 || m_VertexDensity.y == 0)
			return;

		if (config.Properties.size() != m_VertexDensity.x / 4)
			Raise("Incorrect config");
		if (config.Properties[0].size() != m_VertexDensity.y / 4)
			Raise("Incorrect config");

		for (uInt x = 0; x < m_VertexDensity.x; ++x) {
			for (uInt y = 0; y < m_VertexDensity.y; ++y) {
				Nt::Vertex& vertex = m_Shape.Vertices[x + y * m_VertexDensity.x];

				const Nt::Double2D point = {
					vertex.Position.x, vertex.Position.z
				};

				const Config::Property& property = config.Properties[x / 4][y / 4];

				Double noise = _FractalBrownianMotion(
					-(Nt::Double2D(m_Position.x, m_Position.z) + point), property);
				if (noise < property.Min)
					noise = property.Min + Nt::Fract(noise);
				else if (noise > property.Max)
					noise = property.Max + Nt::Fract(noise) - 1.0;

				vertex.Position.y = Float(noise);
				if (config.Mask)
					vertex.Position.y = Float(config.Mask(point, noise));
			}
		}

		_ComputeNormals();

		m_Mesh.SetShape(m_Shape);
	}
#endif

	[[nodiscard]] Nt::Shape& GetShape() noexcept;
	[[nodiscard]] Nt::uInt2D GetVertexDensity() const noexcept;
	[[nodiscard]] Nt::Double2D GetSize() const noexcept;

private:
	Nt::Shape m_Shape;
	Nt::Mesh m_Mesh;
	Nt::Float2D m_Size;
	Nt::uInt2D m_VertexDensity;

protected:
	Double _Perlin(const Nt::Double2D& point);
	Double _Simplex(const Nt::Double2D& point) const noexcept;
	Double _FractalBrownianMotion(const Nt::Double2D& point, const Config::Property& property) const noexcept;
	void _ComputeNormals() noexcept;
};
