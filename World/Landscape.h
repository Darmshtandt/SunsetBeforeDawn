#pragma once

#include <World/Terrain.h>
#include <World/BiomeFactory.h>
#include <Nt/Core/KDTree.h>

class TextureMap;

class Landscape final : public Terrain {
private:
	struct Site : Nt::HiperVector {
		Site(const Nt::Float2D& point, const uInt& biomeIndex);
		Site(const Nt::Float2D& point);

		uInt BiomeIndex;
	};

public:
	Landscape();
	Landscape(const Landscape&) = default;
	Landscape(Landscape&&) noexcept = default;
	~Landscape() override = default;

	Landscape& operator = (const Landscape&) = delete;
	Landscape& operator = (Landscape&&) noexcept = default;

	void Generate(const uInt& biomeCount);

	[[nodiscard]]
	const std::vector<std::unique_ptr<Biome>>& GetBiomes() const noexcept;

private:
	std::vector<std::unique_ptr<Biome>> m_Biomes;
	Transform3D* m_pTransform;
	TextureMap* m_pTextureMap;
	Nt::Mesh m_Mesh;

private:
	std::unique_ptr<Nt::KDTree> _CreateKDTree(const uInt& biomeCount);

	Nt::Double2D _GenerateSite(const Double& minDistance) const noexcept;
};