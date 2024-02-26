#pragma once
#include "raylib.h"
#include "vector"
#include <stdexcept>
#include <string_view>
#include <format>

using namespace std::literals::string_view_literals;
struct Resource 
{
	Texture2D texture;
	explicit Resource(std::string_view path) {
		texture = LoadTexture(path.data());
		if (texture.id <= 0) {
			throw(std::runtime_error(std::format("Unable to load texture: {}"sv, path)));
		}
	}
	~Resource() {
		UnloadTexture(texture);	
	}
	Resource(const Resource & other) noexcept = delete;
	Resource& operator=(const Resource & other) noexcept = delete;
	Resource(Resource && other) noexcept{
		std::swap(other.texture, texture);
	}
	Resource& operator=(Resource&& other) noexcept{
		std::swap(other.texture, texture);
		return *this;
	}
};

struct ResourceManager
{
	std::vector<Resource> shipTextures;
	Resource alienTexture{ "./Assets/Alien.png"sv };
	Resource barrierTexture{ "./Assets/Barrier.png"sv };
	Resource laserTexture{ "./Assets/Laser.png"sv };

	ResourceManager() {
		shipTextures.emplace_back("./Assets/Ship1.png"sv);
		shipTextures.emplace_back("./Assets/Ship2.png"sv);
		shipTextures.emplace_back("./Assets/Ship3.png"sv);
	}
};