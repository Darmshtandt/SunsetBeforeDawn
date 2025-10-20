#pragma once

#include <World/BiomeRegistrar.h>
#include <Core/StdH.h>

class OceanBiome : public Biome {
public:
	OceanBiome(const uInt& id) :
		Biome(id, {
			.Offset = { },
			.AdditionHeight = -60.0,
			.Frequency = 0.0003,
			.Amplitude = 3.0,
			.Lacunarity = 1.8,
			.Gain = 0.5,
			.Min = -10.0,
			.Max = 5.0,
			.NumOctaves = 2
			})
	{
	}

	void Construct() const override {

	}

	[[nodiscard]]
	std::string GetName() const override {
		return "Ocean";
	}

	[[nodiscard]]
	std::string GetTexturePath() const override {
		return PathManager::Textures() + "World\\Stone.tga";
	}

private:
	inline static BiomeRegistrar<OceanBiome> m_Registrar{ "Ocean" };
};