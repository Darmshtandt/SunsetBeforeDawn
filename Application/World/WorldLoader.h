#pragma once

#include <World/Scene.h>

class WorldLoader {
public:
	WorldLoader() = default;

	std::unique_ptr<Scene> Load(const std::string& filename) {
		return std::make_unique<Scene>();
	}
};
