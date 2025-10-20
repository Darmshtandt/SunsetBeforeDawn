#pragma once

#include <World/BiomeRegistrar.h>
#include <Core/StdH.h>

class SwampBiome : public Biome {
public:
	SwampBiome(const uInt& id) :
		Biome(id, {
			.Offset = { },
			.AdditionHeight = -5.0,
			.Frequency = 0.0008,
			.Amplitude = 5.0,
			.Lacunarity = 1.5,
			.Gain = 0.4,
			.Min = 0.0,
			.Max = 30.0,
			.NumOctaves = 2
			})
	{
	}

	void Construct() const override {

	}

	[[nodiscard]]
	std::string GetName() const override {
		return "Swamp";
	}

	[[nodiscard]]
	std::string GetTexturePath() const override {
		return PathManager::Textures() + "World\\StoneGrass.tga";
	}

private:
	inline static BiomeRegistrar<SwampBiome> m_Registrar{ "Swamp" };
};