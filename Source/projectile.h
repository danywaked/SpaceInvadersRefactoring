#pragma once
#include "raylib.h"
#include "Resources.h"

class Projectile
{
public:
	Vector2 position = { 0,0 };
	Vector2 lineStart = { 0, 0 };
	Vector2 lineEnd = { 0, 0 };
	bool active = true;
	int speed = 15;
	bool enemyBullet = false;
	void Update() noexcept;
	void Render(const Resource& texture) const noexcept;
};