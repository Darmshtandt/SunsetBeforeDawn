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
	TEXTURE_HP_BAR_LINE,
	TEXTURE_ARMOR_BAR,
};

enum MeshID : Byte {
	MESH_CUBE,
	MESH_QUAD,
	MESH_CHARACTER,
	MESH_BOMB,
};

class PathManager final {
public:
	PathManager() noexcept = default;
	PathManager(const PathManager&) noexcept = default;
	PathManager(PathManager&&) noexcept = default;
	~PathManager() noexcept = default;

	PathManager& operator = (const PathManager&) noexcept = default;
	PathManager& operator = (PathManager&&) noexcept = default;

	[[nodiscard]] __forceinline static std::string Root() {
		return std::current_path().string() + "\\..\\";
	}
	[[nodiscard]] __forceinline static std::string Resources() {
		return Root() + "Resources\\";
	}

	[[nodiscard]] __forceinline static std::string Textures() {
		return Resources() + "Textures\\";
	}
	[[nodiscard]] __forceinline static std::string Models() {
		return Resources() + "Models\\";
	}
	[[nodiscard]] __forceinline static std::string Shaders() {
		return Resources() + "Shaders\\";
	}
	[[nodiscard]] __forceinline static std::string Fonts() {
		return Resources() + "Fonts\\";
	}
};