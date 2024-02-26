#pragma once
#include "raylib.h"
#include "Resources.h"


class Wall
{

public:
	Vector2 position;
	Rectangle rec;
	int radius = 60;
	
	bool active;
	int health = 50;

	void Render(Resource& texture) const noexcept;
	void Update() noexcept;
};