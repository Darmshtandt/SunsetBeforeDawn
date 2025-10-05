#pragma once

#include <Nt/Core/NtTypes.h>
#include <filesystem>

namespace std {
	using namespace filesystem;
}

enum TextureID : Byte {
	TEXTURE_PLAYER,
	TEXTURE_ENEMY,
	TEXTURE_ELF,
	TEXTURE_TEST,
	TEXTURE_WATER,
	TEXTURE_HP_BAR,
	TEXTURE_ARMOR_BAR,
};

enum MeshID : Byte {
	MESH_CUBE,
	MESH_CHARASTER,
	MESH_QUAD
};

class PathManager final {
public:
	PathManager() noexcept = default;
	PathManager(const PathManager&) noexcept = default;
	PathManager(PathManager&&) noexcept = default;
	~PathManager() noexcept = default;

	PathManager& operator = (const PathManager&) noexcept = default;
	PathManager& operator = (PathManager&&) noexcept = default;

	[[nodiscard]] __forceinline static std::string GetRoot() {
		return std::current_path().string() + "\\..\\";
	}
	[[nodiscard]] __forceinline static std::string GetTextures() {
		return GetRoot() + "Textures\\";
	}
	[[nodiscard]] __forceinline static std::string GetModels() {
		return GetRoot() + "Models\\";
	}
};