#pragma once

#include <World/Biomes/BiomeRegistrar.h>

class HilsBiome : public Biome {
public:
	HilsBiome(const uInt& id) : 
		Biome(id, {
			.Offset = { },
			.AdditionHeight = 80.0,
			.Frequency = 0.0012,
			.Amplitude = 180.0,
			.Lacunarity = 2.0,
			.Gain = 0.4,
			.Min = 30.0,
			.Max = 500.0,
			.NumOctaves = 5
			})
	{
	}

	void Construct() const override {

	}

	[[nodiscard]]
	std::string GetName() const override {
		return "Hils";
	}

	[[nodiscard]]
	std::string GetTexturePath() const override {
		return PathManager::GetTextures() + "World\\Stone.tga";
	}

private:
	inline static BiomeRegistrar<HilsBiome> m_Registrar{ "Hils" };
};