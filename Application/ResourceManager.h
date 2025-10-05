#pragma once

#include <Nt/Graphics/Resources/ResourceManager.h>
#include <Nt/Core/NtTypes.h>
#include <unordered_map>
#include <filesystem>

namespace std {
	using namespace filesystem;
}

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

constexpr const Char* TEXTURE_TEST = "Test";
constexpr const Char* TEXTURE_WATER = "Water";
constexpr const Char* TEXTURE_HEALTH_BAR = "HealthBar";
constexpr const Char* TEXTURE_ARMOR_BAR = "ArmorBar";

constexpr const Char* MESH_CHARASTER = "Charaster";
constexpr const Char* MESH_CUBE = "Cube";
constexpr const Char* MESH_QUAD = "Quad";

class ResourceManager final {
public:
	enum ResourceType {
		TEXTURE,
		MESH
	};

	struct ResourcePath {
		std::string Path;
		const Char* Name;
	};

	using ResourceTypeMap = std::unordered_map<const Char*, Nt::IResource*>;

public:
	ResourceManager() noexcept {
		m_ResourceMap[TEXTURE] = {
			{ "Test.tga", TEXTURE_TEST },
			{ "Water.tga", TEXTURE_WATER },
			{ "UI\\HealthBar.tga", TEXTURE_HEALTH_BAR },
			{ "UI\\ArmorBar.tga", TEXTURE_ARMOR_BAR }
		};

		m_ResourceMap[MESH] = {
			{ "Charaster.obj", MESH_CHARASTER  }
		};
	}
	ResourceManager(const ResourceManager&) noexcept = default;
	ResourceManager(ResourceManager&&) noexcept = default;
	~ResourceManager() noexcept = default;

	[[nodiscard]]static ResourceManager* Instance() noexcept {
		static ResourceManager manager;
		return manager;
	}

	void LoadAll() {
		ResourceTypeMap textureMap;
		for (const ResourcePath& path : m_ResourceMap[TEXTURE]) {
			textureMap[path.Name] = ;
		}
	}

private:
	std::unordered_map<ResourceType, std::vector<ResourcePath>> m_PathMap;
	std::unordered_map<ResourceType, ResourceTypeMap> m_ResourceMap;
};