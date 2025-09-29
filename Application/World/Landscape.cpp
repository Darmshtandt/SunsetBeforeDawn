#include <World/Landscape.h>
#include <Helpers/ProgressBar.h>
#include <Helpers/Profiler.h>

#include <random>

Landscape::Site::Site(const Nt::Float2D& point, const uInt& biomeIndex) :
	HiperVector(point),
	BiomeIndex(biomeIndex)
{
}

Landscape::Site::Site(const Nt::Float2D& point) :
	HiperVector(point),
	BiomeIndex(0)
{
}

Landscape::Landscape() noexcept :
	Terrain(Class<Landscape>::ID())
{
	SetMeshByPtr(&m_Mesh);
}

void Landscape::Generate(const uInt& biomeCount) {
	if (biomeCount == 0)
		return;

	Profiler::Instance().Start();
	const std::unique_ptr<Nt::KDTree> pSitesTree = _CreateKDTree(biomeCount);
	Profiler::Instance().Stop("Biome sites creation time");

	Profiler::Instance().Start();
	const Nt::uInt2D& vertexDensity = GetVertexDensity();
	const uInt totalVertices = vertexDensity.x * vertexDensity.y;
	uInt* pBiomeMap = new uInt[totalVertices];

	const Nt::Double2D basePosition(m_Position.x, m_Position.z);

	Nt::Vertices_t& vertices = GetShape().Vertices;
	std::vector<uInt> indices(totalVertices);
	std::iota(indices.begin(), indices.end(), 0);

	constexpr Biome::Property baseProperty = {
		.Offset = { },
		.AdditionHeight = 15.0,
		.Frequency = 0.0001,
		.Amplitude = 120.0,
		.Lacunarity = 2.0,
		.Gain = 0.45,
		.Min = -100000.0,
		.Max = 1800000.0,
		.NumOctaves = 4
	};

	// Min - O(n * ((log d + e) + c + b + 2a))
	// Max - O(n * (2d + c + b + 2a))
	ProgressBar::Instance().AddTask("Generation", indices.size());
	std::for_each(std::execution::par, indices.begin(), indices.end(), [&](uInt i) {
		Nt::Vertex& vertex = vertices[i];

		const Nt::Float3D& vertexPosition = vertex.Position.xyz;
		const Nt::Float2D point(vertexPosition.x, vertexPosition.z);

		constexpr uInt queueSize = 3;

		// Min - O(log d + e)
		// Max - O(2d)
		Nt::KDTree::PriorityQueueData queueData = pSitesTree->FindKNearest(point, queueSize);
		uInt biomeID = 0;
		Biome::Property properties[queueSize];
		Double weight[queueSize] = { };
		Double weightSum = 0.0;
		uInt biomesCount = 0;

		// O(c)
		for (uInt j = 0; j < queueSize; ++j) {
			const Nt::KDTree::Neighbor& biomeData = queueData.top();
			queueData.pop();

			weight[j] += 1.0 / Double(std::max(std::sqrt(biomeData.SquareDistance), 0.0001f));
			weightSum += weight[j];

			Site* pSite = reinterpret_cast<Site*>(biomeData.pData);
			properties[j] = m_Biomes[pSite->BiomeIndex]->GetProperty();

			++biomesCount;
			if (queueData.empty()) {
				biomeID = pSite->BiomeIndex;
				break;
			}
		}

		//const uInt biomeID = pSitesTree->FindNearest<Site>(point)->BiomeIndex;
		Biome* pBiome = m_Biomes[biomeID].get();

		Biome::Property property = pBiome->GetProperty();

		// O(b)
		for (uInt j = 0; j < biomesCount; ++j) {
			weight[j] /= weightSum;

			property *= 1.0 - weight[j];
			property += properties[j] * weight[j];
		}

		property.Max = 4789465132;

		// O(2a)
		vertex.Position.y = 
			(Float)(_FractalBrownianMotion(
				basePosition + Nt::Double2D(point), baseProperty) * 0
			+ _FractalBrownianMotion(
				basePosition + Nt::Double2D(point), property));

		pBiome->AddVertex(&vertex);
		pBiomeMap[i] = pBiome->GetID();

		ProgressBar::Instance().Update("Generation", 1.0);
		});

	_ComputeNormals();
	m_Mesh.SetShape(GetShape());

	GetBodyPtr()->SetColliderShape(GetShape());
	Profiler::Instance().Stop("Generation Terrain time");

	Profiler::Instance().Start();
	for (std::unique_ptr<Biome>& biome : m_Biomes)
		biome->Construct();
	Profiler::Instance().Stop("Construction Biomes time");

	std::unique_ptr<Byte[]> biomeData(reinterpret_cast<Byte*>(pBiomeMap));
	m_BiomeMapTexture.Create(4, vertexDensity, std::move(biomeData));
}

void Landscape::Render(NotNull<Nt::Renderer*> pRenderer) const {
	m_BiomeMapTexture.BindUnit(1);
	for (auto& [biomeID, texture] : m_Textures)
		texture->BindUnit(2 + biomeID);

	Terrain::Render(pRenderer);
}

const Nt::Texture& Landscape::GetBiomeMap() const noexcept {
	return m_BiomeMapTexture;
}

const std::vector<std::unique_ptr<Biome>>& Landscape::GetBiomes() const noexcept {
	return m_Biomes;
}

std::unique_ptr<Nt::KDTree> Landscape::_CreateKDTree(const uInt& biomeCount) {
	const std::vector<std::string> biomeNames = BiomeFactory::Instance().GetBiomeNames();

	std::vector<Site> sites;
	for (uInt i = 0; i < biomeCount; ++i) {
		const Nt::Double2D point = _GenerateSite(50.0);

		const uInt biomeID = (biomeNames.empty()) ? 0 : (i % biomeNames.size());
		const std::string& name = biomeNames[biomeID];
		m_Biomes.push_back(BiomeFactory::Instance().CreateBiome(name, biomeID));

		sites.emplace_back(point, sites.size());

		if (!m_Textures.contains(biomeID))
			m_Textures[biomeID] = std::make_shared<Nt::Texture>(m_Biomes.back()->GetTexturePath());
	}

	return std::make_unique<Nt::KDTree>(sites);
}

Nt::Double2D Landscape::_GenerateSite(const Double& minDistance) const noexcept {
	const Nt::Double2D& terrainHalfSize = GetSize() / 2.0 / minDistance;
	const Nt::Double2D borderSize = Nt::Double2D(120.0, 120.0) / minDistance;
	const Nt::DoubleRect terrainRect(borderSize - terrainHalfSize, terrainHalfSize - borderSize);

	std::uniform_real_distribution<Double> distX(terrainRect.Left, terrainRect.Right);
	std::uniform_real_distribution<Double> distY(terrainRect.Top, terrainRect.Bottom);

	static thread_local std::mt19937 rng(std::random_device { } ());
	return Nt::Double2D(distX(rng), distY(rng)) * minDistance;
}