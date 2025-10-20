#pragma once

#include <string>
#include <World/BiomeFactory.h>

template <class _Ty>
class BiomeRegistrar {
public:
	BiomeRegistrar(const std::string& name) {
		BiomeFactory::Instance().RegisterBiome(name, [](const uInt& id) -> std::unique_ptr<Biome> {
			return std::make_unique<_Ty>(id);
			});
	}
};