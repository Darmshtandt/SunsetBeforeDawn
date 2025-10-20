#pragma once

#include <World/Objects/GameObject.h>
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
