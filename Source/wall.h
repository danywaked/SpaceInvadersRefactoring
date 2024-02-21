#pragma once
#include "raylib.h"


class Wall
{

public:
	Vector2 position;
	Rectangle rec;
	int radius = 60;
	
	bool active;
	int health = 50;

	void Render(Texture2D texture) const noexcept;
	void Update() noexcept;
};