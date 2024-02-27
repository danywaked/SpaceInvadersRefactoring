#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)

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