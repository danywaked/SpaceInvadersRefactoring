#pragma once
#include "raylib.h"
#include "Resources.h"


class Wall
{

public:
	Vector2 position{};
	Rectangle rec{};
	float radius = 60.0f;
	
	bool active;
	int health = 50;

	void Render(Resource& texture) const noexcept;
	void Update() noexcept;
};