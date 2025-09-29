#pragma once

#include <World/Biomes/BiomeRegistrar.h>

class ForestBiome : public Biome {
public:
	ForestBiome(const uInt& id) :
		Biome(id, {
			.Offset = { },
			.AdditionHeight = 10.0,
			.Frequency = 0.002,
			.Amplitude = 15.0,
			.Lacunarity = 2.0,
			.Gain = 0.5,
			.Min = 5.0,
			.Max = 100.0,
			.NumOctaves = 3
			})
	{
	}

	void Construct() const override {

	}

	[[nodiscard]]
	std::string GetName() const override { 
		return "Forest"; 
	}

	[[nodiscard]]
	std::string GetTexturePath() const override {
		return PathManager::GetTextures() + "World\\StoneGrass.tga";
	}

private:
	inline static BiomeRegistrar<ForestBiome> m_Registrar{ "Forest" };
};