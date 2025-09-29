#pragma once

#include <World/Terrain.h>
#include <World/Biomes/BiomeFactory.h>
#include <Nt/Core/KDTree.h>

class Landscape final : public Terrain {
private:
	struct Site : Nt::HiperVector {
		Site(const Nt::Float2D& point, const uInt& biomeIndex);
		Site(const Nt::Float2D& point);

		uInt BiomeIndex;
	};

public:
	Landscape() noexcept;
	Landscape(const Landscape&) = default;
	Landscape(Landscape&&) noexcept = default;
	~Landscape() override = default;

	Landscape& operator = (const Landscape&) = delete;
	Landscape& operator = (Landscape&&) noexcept = default;

	void Generate(const uInt& biomeCount);

	void Render(NotNull<Nt::Renderer*> pRenderer) const override;

	[[nodiscard]]
	const Nt::Texture& GetBiomeMap() const noexcept;

	[[nodiscard]]
	const std::vector<std::unique_ptr<Biome>>& GetBiomes() const noexcept;

private:
	std::vector<std::unique_ptr<Biome>> m_Biomes;
	std::unordered_map<uInt, std::shared_ptr<Nt::Texture>> m_Textures;
	Nt::Mesh m_Mesh;
	Nt::Texture m_BiomeMapTexture;

private:
	std::unique_ptr<Nt::KDTree> _CreateKDTree(const uInt& biomeCount);

	Nt::Double2D _GenerateSite(const Double& minDistance) const noexcept;
};