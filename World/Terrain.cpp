#include <World/Terrain.h>
#include <World/Components/RenderComponents.h>
#include <World/Components/PhysicComponents.h>
#include <Nt/Core/Colors.h>

Terrain::Config::Property& Terrain::Config::Property::operator+=(const Property& other) {
	return Apply<Nt::BinOpAdd<Double>>(other);
}

Terrain::Config::Property& Terrain::Config::Property::operator*=(const Property& other) {
	return Apply<Nt::BinOpMul<Double>>(other);
}

Terrain::Config::Property& Terrain::Config::Property::operator+=(const Double& value) {
	return Apply<Nt::BinOpAdd<Double>>(value);
}

Terrain::Config::Property& Terrain::Config::Property::operator*=(const Double& value) {
	return Apply<Nt::BinOpMul<Double>>(value);
}

Terrain::Config::Property Terrain::Config::Property::operator*(const Double& value) {
	return Property(*this).Apply<Nt::BinOpMul<Double>>(value);
}


Terrain::Terrain(const ClassID& id) : GameObject(id, ObjectType::Static) {
	AddComponent<Transform3D>();
	AddComponent<Collider>();
	AddComponent<MeshRenderer>()->SetByPointer(&m_Mesh);
}

void Terrain::Create(const Nt::Float2D& size, const Nt::uInt2D& vertexDensity) {
	m_Size = size;
	m_VertexDensity = vertexDensity;

	const Nt::uInt2D squareDensity = m_VertexDensity - 1;
	const uInt totalVertices = m_VertexDensity.x * m_VertexDensity.y;
	const uInt totalIndices = squareDensity.x * squareDensity.y * 6;

	m_Shape.Vertices.resize(totalVertices);
	m_Shape.Indices.resize(totalIndices);

#ifdef _HAS_CXX20
	std::span<Nt::Vertex> vertices(m_Shape.Vertices);
	std::span<Nt::Index_t> indices(m_Shape.Indices);
#else
		Nt::Vertices_t& vertices = m_Shape.Vertices;
		Nt::Indices_t& indices = m_Shape.Indices;
#endif

	const Nt::Float2D step2D = m_Size / Nt::Float2D(m_VertexDensity);
	const Nt::Float2D halfSize = m_Size * 0.5f;
	const uInt& row = m_VertexDensity.y;

	uInt faceIndex = 0;
	for (uInt x = 0; x < m_VertexDensity.x; ++x) {
		for (uInt z = 0; z < m_VertexDensity.y; ++z) {
			const uInt vertexIndex = row * z + x;

			Nt::Vertex& vertex = vertices[vertexIndex];
			vertex.Position.x = step2D.x * Float(x) - halfSize.x;
			vertex.Position.z = step2D.y * Float(z) - halfSize.y;
			vertex.Position.w = 1.f;
			vertex.Normal.w = 1.f;
			vertex.TexCoord.x = x;
			vertex.TexCoord.y = z;
			vertex.Color = Nt::Colors::White;
				
			if (x < squareDensity.x && z < squareDensity.y) {
				indices[faceIndex] = vertexIndex;
				indices[faceIndex + 1] = vertexIndex + row;
				indices[faceIndex + 2] = vertexIndex + row + 1;

				indices[faceIndex + 3] = vertexIndex + row + 1;
				indices[faceIndex + 4] = vertexIndex + 1;
				indices[faceIndex + 5] = vertexIndex;

				faceIndex += 6;
			}
		}
	}

	m_Mesh.SetShape(m_Shape);
	GetComponent<Collider>()->SetShape(m_Shape);
}

Nt::Shape& Terrain::GetShape() noexcept {
	return m_Shape;
}

Nt::uInt2D Terrain::GetVertexDensity() const noexcept {
	return m_VertexDensity;
}

Nt::Double2D Terrain::GetSize() const noexcept {
	return m_Size;
}

Double Terrain::_Perlin(const Nt::Double2D& point) {
	Nt::Double4D pi = Nt::Double4D(point.x, point.y, point.x, point.y).Floor() + Nt::Double4D(0.0, 0.0, 1.0, 1.0);
	Nt::Double4D pf = Nt::Double4D(point.x, point.y, point.x, point.y).Fract() - Nt::Double4D(0.0, 0.0, 1.0, 1.0);

	for (uInt i = 0; i < 4; ++i)
		pi.Array[i] = fmod(pi.Array[i], 289.0);

	Nt::Double4D ix(pi.x, pi.z, pi.x, pi.z);
	Nt::Double4D iy(pi.y, pi.y, pi.w, pi.w);
	Nt::Double4D fx(pf.x, pf.z, pf.x, pf.z);
	Nt::Double4D fy(pf.y, pf.y, pf.w, pf.w);

	Nt::Double4D i = (ix.Permute() + iy).Permute();

	Nt::Double4D gx = (i / 41).Fract() * 2.0 - 1;
	Nt::Double4D gy = gx.Abs() - 0.5;
	Nt::Double4D tx = (gx + 0.5).Floor();
	gx = gx - tx;

	Nt::Double2D g00(gx.x, gy.x);
	Nt::Double2D g10(gx.y, gy.y);
	Nt::Double2D g01(gx.z, gy.z);
	Nt::Double2D g11(gx.w, gy.w);

	Nt::Double4D vectors(
		g00.Dot(g00),
		g01.Dot(g01),
		g10.Dot(g10),
		g11.Dot(g11));

	Nt::Double4D norm;
	for (uInt i = 0; i < 4; ++i)
		norm.Array[i] = 1.0 / sqrt(vectors[i]);

	g00 *= norm.x;
	g01 *= norm.y;
	g10 *= norm.z;
	g11 *= norm.w;

	Double n00 = g00.Dot({ fx.x, fy.x });
	Double n10 = g10.Dot({ fx.y, fy.y });
	Double n01 = g01.Dot({ fx.z, fy.z });
	Double n11 = g11.Dot({ fx.w, fy.w });

	Nt::Double2D fade_xy = pf.xy.Fade();
	Nt::Double2D n_x = Nt::Double2D(fade_xy.x, fade_xy.x).Lerp(Nt::Double2D(n00, n01), Nt::Double2D(n10, n11));
	Double n_xy = Nt::Lerp(n_x.x, n_x.y, fade_xy.y);

	return 2.3 * n_xy;
}

Double Terrain::_Simplex(const Nt::Double2D& point) const noexcept {
	constexpr Nt::Double4D constants = {
		0.211324865405187,	// (3.0 - sqrt(3.0)) / 6.0
		0.366025403784439,	//  0.5 * (sqrt(3.0) - 1.0)
		-0.577350269189626,	// -1.0 + 2.0 * C.x
		0.024390243902439	//  1.0 / 41.0
	};

	// First corner
	Nt::Double2D gridCell = (point + point.Dot({ constants.y, constants.y })).Floor();
	Nt::Double2D localPosition = point - gridCell + gridCell.Dot({ constants.x, constants.x });

	// Other corners
	Nt::Double2D offset = (localPosition.x > localPosition.y) ? Nt::Double2D(1.0, 0.0) : Nt::Double2D(0.0, 1.0);
	Nt::Double4D simplexOffsets = {
		localPosition.x + constants.x - offset.x,
		localPosition.y + constants.x - offset.y,
		localPosition.x + constants.z,
		localPosition.y + constants.z
	};

	// Permutations
	gridCell = Nt::Double2D(fmod(gridCell.x, 289.0), fmod(gridCell.y, 289.0));

	Nt::Double3D hashedIndices = Nt::Double3D(
		Nt::Permute(gridCell.y) + gridCell.x,
		Nt::Permute(gridCell.y + offset.y) + gridCell.x + offset.x,
		Nt::Permute(gridCell.y + 1.0) + gridCell.x + 1.0
	).Permute();

	Nt::Double3D attenuation = {
		std::max(0.5 - localPosition.LengthSquare(), 0.0),
		std::max(0.5 - simplexOffsets.xy.LengthSquare(), 0.0),
		std::max(0.5 - simplexOffsets.zw.LengthSquare(), 0.0)
	};

	attenuation *= attenuation * attenuation;

	Nt::Double3D gradientVector = 2.0 * (hashedIndices * constants.w).Fract() - 1.0;
	Nt::Double3D gradientOffset = gradientVector.Abs() - 0.5;
	Nt::Double3D offsetFloor = (gradientVector + 0.5).Floor();
	Nt::Double3D finalGradient = gradientVector - offsetFloor;

	attenuation *= 1.79284291400159 - 0.85373472095314 * (finalGradient * finalGradient + gradientOffset * gradientOffset);

	Nt::Double3D contribution = {
		finalGradient.x * localPosition.x + gradientOffset.x * localPosition.y,
		finalGradient.y * simplexOffsets.x + gradientOffset.y * simplexOffsets.y,
		finalGradient.z * simplexOffsets.z + gradientOffset.z * simplexOffsets.w
	};

	return 130.0 * attenuation.Dot(contribution);
}

Double Terrain::_FractalBrownianMotion(const Nt::Double2D& point, const Config::Property& property) const noexcept {
	Double noise = property.AdditionHeight;
	Double frequency = property.Frequency;
	Double amplitude = property.Amplitude;

	// O(a)
	for (uInt i = 0; i < property.NumOctaves; ++i) {
		// O(1)
		noise += _Simplex(point * frequency) * amplitude;
		frequency *= property.Lacunarity;
		amplitude *= property.Gain;
	}

	if (noise < property.Min)
		noise = property.Min + Nt::Fract(noise);
	else if (noise > property.Max)
		noise = property.Max - Nt::Fract(noise);

	return noise;
}

void Terrain::_ComputeNormals() noexcept {
	std::vector<Nt::Float3D> accumulatedNormals(m_Shape.Vertices.size(), Nt::Float3D(0.f));

	for (uInt i = 0; i < m_Shape.Indices.size(); i += 3) {
		const Nt::Index_t& i0 = m_Shape.Indices[i + 0];
		const Nt::Index_t& i1 = m_Shape.Indices[i + 1];
		const Nt::Index_t& i2 = m_Shape.Indices[i + 2];

		const Nt::Float3D& p0 = m_Shape.Vertices[i0].Position.xyz;
		const Nt::Float3D& p1 = m_Shape.Vertices[i1].Position.xyz;
		const Nt::Float3D& p2 = m_Shape.Vertices[i2].Position.xyz;

		const Nt::Float3D faceNormal = (p1 - p0).GetCross(p2 - p0).GetNormalize();

		accumulatedNormals[i0] += faceNormal;
		accumulatedNormals[i1] += faceNormal;
		accumulatedNormals[i2] += faceNormal;
	}

	std::for_each(std::execution::par, m_Shape.Vertices.begin(), m_Shape.Vertices.end(),
	              [&](Nt::Vertex& vertex) {
		              const uInt i = &vertex - m_Shape.Vertices.data();

		              vertex.Normal.xyz = accumulatedNormals[i].GetNormalize();
		              vertex.Normal.w = 1.f;
	              });
}
