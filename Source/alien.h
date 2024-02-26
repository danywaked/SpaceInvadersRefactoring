#pragma once
#include "raylib.h"
#include "Resources.h"

class Alien
{
	int speed = 2;
	bool moveRight = true;

public:
	Vector2 position = { 0, 0 };
	bool active = true;
	float radius = 30;

	void Update() noexcept;
	void Render(Resource& texture) const noexcept;
};